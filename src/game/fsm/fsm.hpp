#pragma once

#include <game/fsm/action.hpp>
#include <game/fsm/state.hpp>
#include <memory>
#include <optional>
#include <vector>

namespace durak {

class FSM {
  std::shared_ptr<State> current;
  std::vector<std::shared_ptr<State>> allStates;

public:
  FSM( std::shared_ptr<State> start,
       std::vector<std::shared_ptr<State>> states );

  // FSM( FSM &&other ) noexcept {
  //   this->current   = std::move( other.current );
  //   this->allStates = std::move( other.allStates );
  // }

  std::optional<Action> onEvent( Event ev ) noexcept;
};

} // namespace durak