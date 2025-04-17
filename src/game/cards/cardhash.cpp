#include "cardhash.hpp"

#include <functional>

namespace durak {

size_t CardHash::operator() ( const Card &card ) const noexcept {
  size_t h1 = std::hash<int> {}( static_cast<int>( card.getSuit() ) );
  size_t h2 = std::hash<int> {}( static_cast<int>( card.getRank() ) );
  return h1 ^ ( h2 << 1 );
}

size_t CardHash::operator() ( std::unique_ptr<Card> card ) const noexcept {
  return this->operator() ( *card );
}

} // namespace durak