#include "fsm.hpp"

namespace durak {

FSM::FSM( std::shared_ptr<State> current ) noexcept
    : current( std::move( current ) ) { }

std::optional<Action> FSM::onEvent( Event ev ) noexcept {
  if ( !current->transitions.contains( ev ) ) {
    return std::nullopt;
  }

  auto next_ptr = std::move( current->transitions[ev] );
  current       = std::move( next_ptr );
  return current->onEnter;
}

} // namespace durak