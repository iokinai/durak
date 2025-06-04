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

  connect( qApp, &QCoreApplication::aboutToQuit, this, [this]( auto ) {
    exitReason = WaitExitReason::Exiting;
    wait.quit();
  } );

  for ( auto &player : this->b ) {
    connect( player.get(), &Player::gc_player_takeCardFromDeck, this,
             &GameController::playerTakeCardFromDeck );

    connect( player.get(), &Player::gc_player_takeCurrentCard, this,
             &GameController::playerTakeCardsFromTable );

    connect( this, &GameController::setCurrentTrump, player.get(),
             &Player::gc_setCurrentTrump );

    connect(
        player.get(), &Player::gc_player_noCards, this,
        [this]( Player *player ) {
          if ( !this->deck->deck.empty() ) {
            return;
          }

          lastEvent = Event::RoundEnded;
          QTimer::singleShot( 0, &wait, &QEventLoop::quit );
          emit roundEndWithWin( player );
        },
        Qt::SingleShotConnection );

    connect( player.get(), &Player::gc_playerBeaten, this,
             [this]( Player *player ) {
               if ( currentPlayer.get() != player ) {
                 return;
               }

               table.clear();
             } );

    player->setTable( &this->table );
  }
}

bool GameController::canCardBeAccepted( Card *card ) {
  if ( !card ) {
    return false;
  }

  switch ( currentTurn ) {
  case CurrentTurn::Attack :
    return canAttackCardBeAccepted( *card );
  case CurrentTurn::Defence :
    return canDefenceCardBeAccepted( *card );
  case CurrentTurn::Idle :
    return false;
  default :
    return false;
  }
}

bool GameController::canAttackCardBeAccepted( Card const &card ) {
  return table.empty() ||
         std::find_if( table.begin(), table.end(),
                       [&card]( const std::unique_ptr<Card> &c ) {
                         return card.getRank() == c->getRank();
                       } ) != table.end();
}

bool GameController::canDefenceCardBeAccepted( Card const &card ) {
  return card.beats( *currentCard, currentTrump );
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

void GameController::playerTakeCardsFromTable( Player *player ) noexcept {
  if ( currentTurn != CurrentTurn::Defence ) {
    return;
  }

  player->takeCards( std::move( table ) );

  // emit clearTable();
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
}

WaitResult<CardWaitResult>
GameController::attackRequest( std::shared_ptr<Player> player ) noexcept {
  Card *card     = nullptr;
  bool gotResult = false;

  connect(
      player.get(), &Player::gc_attacked, this,
      [&card, this, &gotResult]( Card *sent ) mutable {
        card       = sent;
        gotResult  = true;
        exitReason = WaitExitReason::Ok;
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

  if ( !gotResult && exitReason == WaitExitReason::Exiting ) {
    return WaitResultCritical {};
  }

  if ( !card ) {
    return CardWaitResultNoCard {};
  }

  if ( !canCardBeAccepted( card ) ) {
    emit cardThrowResult( CardThrowResult::RejectedRequiersRepeat, card );
    return CardWaitResultRejected {};
  }

  emit cardThrowResult( CardThrowResult::Accepted, card );
  table.push_back( std::unique_ptr<Card>( card ) );
  emit addCardOnTable( card );

  return CardWaitResultAccepted { card };
}

WaitResult<CardWaitResult>
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

  if ( card && !canCardBeAccepted( card ) ) {
    result = CardThrowResult::RejectedRequiersRepeat;
  }

  emit cardThrowResult( result, card );

  switch ( result ) {
  case CardThrowResult::Accepted : {
    if ( !card ) {
      return CardWaitResultNoCard {};
    }

    table.push_back( std::unique_ptr<Card>( card ) );
    emit addCardOnTable( card );
    return CardWaitResultAccepted { card };
  }
  case CardThrowResult::RejectedRequiersRepeat :
    return CardWaitResultRejected {};
  }

  return CardWaitResultNoCard {};
}

void GameController::dealCardsTo6() noexcept {
  for ( auto player : b ) {
    if ( player->cards.size() >= default_cards_per_player ) {
      continue;
    }

    auto deal_count = default_cards_per_player - player->cards.size();
    std::vector<std::unique_ptr<Card>> cards;

    for ( auto i = 0; i < deal_count; ++i ) {
      auto c = deck->takeTopCard();

      if ( c != nullptr ) {
        cards.push_back( std::move( std::move( c ) ) );
      }
    }

    player->takeCards( std::move( cards ) );
  }
}

void GameController::gameLoop() noexcept {
  currentTrump = heap.back()->getSuit();
  emit setCurrentTrump( currentTrump );

  lastEvent = Event::GameStarted;

  while ( true ) {

    currentPlayer = b.next();
    currentCard   = nullptr;

    // Round loop
    while ( true ) {
      if ( lastEvent == Event::RoundEnded ) {
        lastEvent = Event::NextRoundStarted;
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
        emit clearTable();
        currentTurn = CurrentTurn::Idle;
        break;
      }
      case Action::PrevPlayerAttack : {
        currentPlayer = b.prev();
      }
      case Action::CurrPlayerAttack : {
        currentTurn     = CurrentTurn::Attack;
        auto waitResult = attackRequest( currentPlayer );
        auto result     = std::get_if<CardWaitResult>( &waitResult );

        if ( !result ) {
          return;
        }

        std::visit( overloads { [this]( CardWaitResultNoCard ) {
                                 lastEvent = Event::Beat;
                               },
                                [this]( CardWaitResultAccepted &result ) {
                                  currentCard = result.card;
                                  lastEvent   = Event::PlayerAttacked;
                                },
                                [this]( CardWaitResultRejected ) {
                                  lastEvent = Event::AttackFailed;
                                } },
                    *result );

        break;
      }
      case Action::NextPlayerDefend : {
        currentTurn     = CurrentTurn::Defence;
        currentPlayer   = b.next();
        auto waitResult = defenceRequest( currentPlayer, currentCard );

        auto resultVariant = std::get_if<CardWaitResult>( &waitResult );

        if ( !resultVariant ) {
          return;
        }

        std::visit(
            overloads {
                [this]( CardWaitResultNoCard ) {
                  if ( lastEvent != Event::RoundEnded ) {
                    lastEvent     = Event::PlayerCantDefend;
                    currentPlayer = b.next();
                  }
                },
                [this]( CardWaitResultAccepted &result ) {
                  if ( lastEvent != Event::RoundEnded )
                    lastEvent = Event::PlayerDefended;

                  QTimer::singleShot(
                      500, [this, newCard = std::move( currentCard )] mutable {
                        currentCard = std::move( currentCard );
                      } );
                },
                [this]( CardWaitResultRejected ) {
                  currentPlayer = b.prev();
                  lastEvent     = Event::DefenceFailed;
                  emit uiGameLogicError();
                },
            },
            *resultVariant );
        break;
      }
      case Action::DefenderPlayerTakeCards :
        lastEvent = Event::NextPlayerTookCards;
        break;
      case Action::DrawCards :
        currentTurn = CurrentTurn::Idle;
        dealCardsTo6();
        break;
      case Action::RoundEnd :
        lastEvent = Event::RoundEnded;
        table.clear();
        emit clearTable();
        break;
      case Action::EndGame :
        return;
      default :
        break;
      }
    }
  }
}

} // namespace durak
