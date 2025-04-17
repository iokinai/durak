#include "fsm.hpp"

namespace durak {

FSM::FSM( std::shared_ptr<State> start,
          std::vector<std::shared_ptr<State>> states )
    : current( std::move( start ) ), allStates( std::move( states ) ) { }

std::optional<Action> FSM::onEvent( Event ev ) noexcept {
  if ( !current )
    return std::nullopt;

  auto it = current->transitions.find( ev );
  if ( it == current->transitions.end() )
    return std::nullopt;

  if ( auto next = it->second.lock() ) {
    current = std::move( next );
    return current->onEnter;
  }

  return std::nullopt;
}

} // namespace durak