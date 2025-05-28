#include "gamecontroller.hpp"
#include <QCoreApplication>
#include <QMessageBox>
#include <QTimer>
#include <memory>
#include <overloads.hpp>
#include <random>
#include <variant>
#include <vector>

namespace durak {

GameController::GameController( PlayerBuffer &&b, std::unique_ptr<FSM> fsm,
                                std::vector<std::unique_ptr<Card>> heap,
                                DeckWidget *deck ) noexcept
    : b( std::move( b ) ), fsm( std::move( fsm ) ), heap( std::move( heap ) ),
      random_device(), random_engine( random_device() ), deck( deck ) {
  shuffleHeap( this->heap );

  connect( qApp, &QCoreApplication::aboutToQuit, this,
           [this]( auto ) { wait.quit(); } );

  for ( auto &player : this->b ) {
    connect( player.get(), &Player::gc_player_takeCardFromDeck, this,
             &GameController::playerTakeCardFromDeck );

    connect( player.get(), &Player::gc_player_takeCurrentCard, this,
             &GameController::playerTakeCardFromTable );

    connect( this, &GameController::setCurrentTrump, player.get(),
             &Player::gc_setCurrentTrump );

    connect(
        player.get(), &Player::gc_player_noCards, this,
        [this]( Player *player ) {
          lastEvent = Event::RoundEnded;
          QTimer::singleShot( 0, &wait, &QEventLoop::quit );
          emit roundEndWithWin( player );
        },
        Qt::SingleShotConnection );
  }
}

void GameController::shuffleHeap(
    std::vector<std::unique_ptr<Card>> &heap ) noexcept {
  std::random_device rd;
  std::mt19937 g( rd() );
  std::shuffle( heap.begin(), heap.end(), g );
}

void GameController::start() noexcept {
  std::optional<Action> act = fsm->onEvent( Event::GameStarted );

  if ( !act.has_value() ) {
    return;
  }

  gameLoop();
}

void GameController::dealCards() noexcept {
  emit uiDealCards();
}

void GameController::playerTakeCardFromDeck( Card *card,
                                             Player *player ) noexcept {
  if ( currentPlayer.get() != player ) {
    return;
  }

  std::vector<std::unique_ptr<Card>> c;
  c.push_back( std::move( std::unique_ptr<Card>( card ) ) );
  currentPlayer->takeCards( std::move( c ) );
}

void GameController::playerTakeCardFromTable( Player *player ) noexcept {
  if ( !( currentPlayer.get() == player &&
          currentTurn == CurrentTurn::Defence ) ) {
    return;
  }

  Card *move = currentCard.release();

  std::vector<std::unique_ptr<Card>> cards;
  cards.push_back( std::move( std::unique_ptr<Card>( move ) ) );
  player->takeCards( std::move( cards ) );

  emit putCardOnTable( nullptr );
}

std::vector<std::unique_ptr<Card>> GameController::randomFromHeap() noexcept {
  std::vector<std::unique_ptr<Card>> res;

  for ( size_t i = 0; i < default_cards_per_player; ++i ) {
    std::uniform_int_distribution<> distr( 0, heap.size() - 1 );

    size_t idx = distr( random_engine );
    res.push_back( std::move( heap[idx] ) );
    heap.erase( heap.begin() + idx );
  }

  return res;
}

void GameController::formatTable() noexcept {
  int playersCardsCount = b.size() * default_cards_per_player;

  for ( const auto &player : b ) {
    player->takeCards( std::move( randomFromHeap() ) );
  }

  deck->putCards( heap );
  heap.clear();
}

WaitResult<std::unique_ptr<Card>>
GameController::attackRequest( std::shared_ptr<Player> player ) noexcept {
  Card *card     = nullptr;
  bool gotResult = false;

  connect(
      player.get(), &Player::gc_attacked, this,
      [&card, this, &gotResult]( Card *sent ) mutable {
        card      = sent;
        gotResult = true;
        wait.exit();
      },
      Qt::SingleShotConnection );

  connect( this, &GameController::uiAttackRequest, player.get(),
           &Player::gc_onAttackTurn, Qt::SingleShotConnection );

  connect( this, &GameController::cardThrowResult, player.get(),
           &Player::gc_cardThrowResult, Qt::SingleShotConnection );

  emit uiAttackRequest();

  if ( !gotResult ) {
    wait.exec();
  }

  if ( !gotResult ) {
    return WaitResultCritical {};
  }

  emit cardThrowResult( CardThrowResult::Accepted, card );
  emit putCardOnTable( card );

  return std::move( std::unique_ptr<Card>( card ) );
}

WaitResult<DefenceResult>
GameController::defenceRequest( std::shared_ptr<Player> player,
                                Card *attackCard ) noexcept {
  Card *card     = nullptr;
  bool gotResult = false;

  connect(
      player.get(), &Player::gc_defended, this,
      [&card, this, &gotResult]( Card *sent ) mutable {
        card      = sent;
        gotResult = true;
        wait.exit();
      },
      Qt::SingleShotConnection );

  connect( this, &GameController::uiDefenceRequest, player.get(),
           &Player::gc_onDefenceTurn, Qt::SingleShotConnection );

  connect( this, &GameController::cardThrowResult, player.get(),
           &Player::gc_cardThrowResult, Qt::SingleShotConnection );

  emit uiDefenceRequest( attackCard );

  if ( !gotResult ) {
    wait.exec();
  }

  if ( !gotResult ) {
    return WaitResultCritical {};
  }

  CardThrowResult result = CardThrowResult::Accepted;

  if ( card && !card->beats( *attackCard, currentTrump ) ) {
    result = CardThrowResult::RejectedRequiersRepeat;
  }

  emit cardThrowResult( result, card );

  emit putCardOnTable( nullptr );

  switch ( result ) {
  case CardThrowResult::Accepted : {
    if ( !card ) {
      return DefenceResultNoCard {};
    }

    return DefenceResultAccepted { std::move( std::unique_ptr<Card>( card ) ) };
  }
  case CardThrowResult::RejectedRequiersRepeat :
    return DefenceResultRejected {};
  }

  return DefenceResultNoCard {};
}

void GameController::gameLoop() noexcept {
  currentTrump = heap.back()->getSuit();
  emit setCurrentTrump( currentTrump );

  while ( true ) {
    currentPlayer = b.next();
    currentCard   = nullptr;

    lastEvent = Event::GameStarted;

    while ( true ) {
      auto action_opt = fsm->onEvent( lastEvent );

      if ( !action_opt.has_value() ) {
        return;
      }

      auto action = action_opt.value();

      switch ( action ) {
      case Action::GiveCards : {
        formatTable();
        lastEvent = Event::RoundStarted;
        emit putCardOnTable( nullptr );
        currentTurn = CurrentTurn::Idle;
        break;
      }
      case Action::PlayerAttack : {
        currentTurn     = CurrentTurn::Attack;
        auto waitResult = attackRequest( currentPlayer );
        auto result     = std::get_if<std::unique_ptr<Card>>( &waitResult );

        if ( !result ) {
          return;
        }

        currentCard = std::move( *result );
        lastEvent   = Event::PlayerAttacked;
        break;
      }
      case Action::NextPlayerDefend : {
        currentTurn     = CurrentTurn::Defence;
        currentPlayer   = b.next();
        auto waitResult = defenceRequest( currentPlayer, currentCard.get() );

        auto resultVariant = std::get_if<DefenceResult>( &waitResult );

        if ( !resultVariant ) {
          return;
        }

        std::visit( overloads {
                        [this]( DefenceResultNoCard ) {
                          if ( lastEvent != Event::RoundEnded ) {
                            lastEvent = Event::PlayerDefended;
                            currentPlayer = b.next();
                          }

                        },
                        [this]( DefenceResultAccepted &result ) {
                          auto newCard = std::move( result.card );
                          emit addCardOnTable( newCard.get() );
                          if ( lastEvent != Event::RoundEnded )
                            lastEvent = Event::PlayerDefended;

                          QTimer::singleShot(
                              500,
                              [this, newCard = std::move( newCard )] mutable {
                                emit clearTable();
                                currentCard = std::move( newCard );
                              } );
                        },
                        [this]( DefenceResultRejected ) {
                          currentPlayer = b.prev();
                          emit uiGameLogicError();
                        },
                    },
                    *resultVariant );
        break;
      }
      case Action::PlayerTakeCards :
      case Action::DrawCards :
        currentTurn = CurrentTurn::Idle;
        break;
      case Action::RoundEnd :
        return;
      default :
        break;
      }
    }
  }
}

} // namespace durak