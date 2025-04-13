#pragma once

#include <game/fsm/action.hpp>
#include <game/fsm/event.hpp>
#include <memory>
#include <unordered_map>

namespace durak {

class FSM;

class State {
  Action onEnter;
  std::unordered_map<Event, std::shared_ptr<State>> transitions;

public:
  State( Action onEnter,
         const std::unordered_map<Event, std::shared_ptr<State>> &transition );

  friend class FSM;
};

} // namespace durak