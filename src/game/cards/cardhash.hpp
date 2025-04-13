#pragma once

#include <game/cards/card.hpp>

namespace durak {

struct CardHash {
  size_t operator() ( const Card &card ) const noexcept;
};

} // namespace durak