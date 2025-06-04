#pragma once

#include <game/fsm/action.hpp>
#include <game/fsm/event.hpp>
#include <memory>
#include <unordered_map>

namespace durak {

class FSM;

class State {
  Action onEnter;
  std::unordered_map<Event, std::weak_ptr<State>> transitions;

public:
  State( Action onEnter );

  void setTransitions( const std::unordered_map<Event, std::weak_ptr<State>>
                           &transitions ) noexcept;

  friend class FSM;
};

} // namespace durak