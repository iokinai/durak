#pragma once

#include <game/fsm/action.hpp>
#include <game/fsm/event.hpp>
#include <memory>
#include <unordered_map>

#include <QMessageBox>

namespace durak {

class FSM;

class State {
  Action onEnter;
  std::unordered_map<Event, std::weak_ptr<State>> transitions;

public:
  State( Action onEnter );

  // State( State &&other ) {
  //   onEnter     = std::move( other.onEnter );
  //   transitions = std::move( other.transitions );
  // }

  void setTransitions( const std::unordered_map<Event, std::weak_ptr<State>>
                           &transitions ) noexcept;

  friend class FSM;

  // inline ~State() {
  //   QMessageBox::information( nullptr, "123", "123" );
  // }
};

} // namespace durak