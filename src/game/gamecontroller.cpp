#include "gamecontroller.hpp"

namespace durak {

GameController::GameController( PlayerBuffer &&b, const FSM &fsm ) noexcept
    : b( std::move( b ) ), fsm( fsm ) {
  //
}

void GameController::start() noexcept {
  std::optional<Action> act = fsm.onEvent( Event::GameStarted );

  if ( !act.has_value() ) {
    return;
  }

  doStartActions( act.value() );

  gameLoop();
}

void GameController::doStartActions( Action act ) noexcept {
  switch ( act ) {

  case Action::ShuffleTheCards :
    shuffleCards();
    break;
  case Action::DealTheCards :
    dealCards();
  default :
    return;
  }
}

void GameController::shuffleCards() noexcept {
  emit uiShuffleCards();
}

void GameController::dealCards() noexcept {
  emit uiDealCards();
}

Card GameController::attackRequest( std::shared_ptr<Player> player ) noexcept {
  emit uiAttackRequest();
}

void GameController::gameLoop() noexcept {
  CardSuit currentTrump;

  while ( true ) {
    auto currentPlayer              = b.next();
    std::optional<Card> currentCard = std::nullopt;

    Event lastEvent = Event::GameStarted;

    while ( true ) {
      if ( lastEvent == Event::RoundEnded ) {
        break;
      }

      auto action = fsm.onEvent( lastEvent );

      switch ( action.value() ) {
      case Action::PlayerAttack : {
        currentCard = attackRequest( currentPlayer );
        lastEvent   = Event::PlayerAttacked;
        break;
      }
      case Action::NextPlayerDefend : {
        currentPlayer = b.next();
        auto result   = defenceRequest( currentPlayer );

        if ( result.has_value() ) {
          if ( !result->beats( currentCard.value(), currentTrump ) ) {
            emit uiGameLogicError();
            break;
          }

          lastEvent = Event::PlayerDefended;
          break;
        }

        lastEvent = Event::PlayerCantDefend;
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