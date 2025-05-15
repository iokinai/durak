#include "gamecontroller.hpp"
#include <QCoreApplication>
#include <memory>
#include <overloads.hpp>
#include <random>
#include <variant>
#include <vector>

namespace durak {

GameController::GameController(
    PlayerBuffer &&b, std::unique_ptr<FSM> fsm,
    std::vector<std::unique_ptr<Card>> heap ) noexcept
    : b( std::move( b ) ), fsm( std::move( fsm ) ), heap( std::move( heap ) ),
      random_device(), random_engine( random_device() ) {
  connect( qApp, &QCoreApplication::aboutToQuit, this,
           [this]( auto ) { wait.quit(); } );
}

void GameController::start() noexcept {
  std::optional<Action> act = fsm->onEvent( Event::GameStarted );

  if ( !act.has_value() ) {
    return;
  }

  doStartActions( act.value() );

  gameLoop();
}

void GameController::doStartActions( Action act ) noexcept {
  // switch ( act ) {

  // case Action::ShuffleTheCards :
  //   shuffleCards();
  //   break;
  // case Action::DealTheCards :
  //   dealCards();
  // default :
  //   return;
  // }
}

void GameController::shuffleCards() noexcept {
  emit uiShuffleCards();
}

void GameController::dealCards() noexcept {
  emit uiDealCards();
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
    player->takeCards( randomFromHeap() );
  }
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

  if ( card && !card->beats( *attackCard, CardSuit::DM ) ) {
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
}

void GameController::gameLoop() noexcept {
  CardSuit currentTrump;

  while ( true ) {
    auto currentPlayer                = b.next();
    std::unique_ptr<Card> currentCard = nullptr;

    Event lastEvent = Event::GameStarted;

    while ( true ) {
      if ( lastEvent == Event::RoundEnded ) {
        break;
      }

      auto action_opt = fsm->onEvent( lastEvent );

      if ( !action_opt.has_value() ) {
        return;
      }

      auto action = action_opt.value();

      switch ( action ) {
      case Action::GiveCards : {
        formatTable();
        lastEvent = Event::RoundStarted;
        break;
      }
      case Action::PlayerAttack : {
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
        currentPlayer   = b.next();
        auto waitResult = defenceRequest( currentPlayer, currentCard.get() );

        auto resultVariant = std::get_if<DefenceResult>( &waitResult );

        if ( !resultVariant ) {
          return;
        }

        std::visit(
            overloads {
                [&currentCard, &lastEvent]( DefenceResultNoCard ) {
                  lastEvent = Event::PlayerCantDefend;
                  QMessageBox::information( nullptr, "Defence",
                                            "Player can't defend" );
                },
                [&currentCard, &lastEvent]( DefenceResultAccepted &result ) {
                  currentCard = std::move( result.card );
                  lastEvent   = Event::PlayerDefended;
                  QMessageBox::information(
                      nullptr, "Defence",
                      QString( "Player defended with card" ) );
                },
                [&currentCard, &currentPlayer, this]( DefenceResultRejected ) {
                  currentPlayer = b.prev();
                  QMessageBox::information(
                      nullptr, "Defence",
                      QString( "Player can't defend with this card" ) );
                  emit uiGameLogicError();
                },
            },
            *resultVariant );
        break;
      }
      case Action::PlayerTakeCards :
      case Action::DrawCards :
        break;
      case Action::RoundEnd :
        lastEvent = Event::RoundEnded;
        break;
      default :
        break;
      }
    }
  }
}

} // namespace durak