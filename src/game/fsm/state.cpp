#include "state.hpp"

namespace durak {
State::State(
    Action onEnter,
    const std::unordered_map<Event, std::shared_ptr<State>> &transitions )
    : onEnter( onEnter ), transitions( transitions ) { }
} // namespace durak