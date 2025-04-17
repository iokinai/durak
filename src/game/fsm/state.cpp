#include "state.hpp"

namespace durak {

State::State( Action onEnter ) : onEnter( onEnter ), transitions() { }

void State::setTransitions(
    const std::unordered_map<Event, std::weak_ptr<State>>
        &transitions ) noexcept {
  this->transitions = transitions;
}

} // namespace durak