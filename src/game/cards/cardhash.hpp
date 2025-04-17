#pragma once

#include <game/cards/card.hpp>
#include <memory>

namespace durak {

struct CardHash {
  size_t operator() ( const Card &card ) const noexcept;
  size_t operator() ( std::unique_ptr<Card> card ) const noexcept;
};

} // namespace durak