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
  default :
    return;
  }
}

void GameController::shuffleCards() noexcept {
  emit uiShuffleCards();
}

void GameController::gameLoop() noexcept {
  while ( true ) {
    auto currentPlayer = b.next();

    //
  }
}

} // namespace durak