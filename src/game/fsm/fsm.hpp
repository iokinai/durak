#pragma once

#include <game/fsm/action.hpp>
#include <game/fsm/state.hpp>
#include <memory>
#include <optional>

namespace durak {

class FSM {
  std::shared_ptr<State> current;

public:
  FSM( std::shared_ptr<State> current ) noexcept;
  std::optional<Action> onEvent( Event ev ) noexcept;
};

} // namespace durak