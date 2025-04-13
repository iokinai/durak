#include "card.hpp"
#include <algorithm>

namespace durak {

Card::Card( CardSuit suit, uint8_t rank )
    : suit( suit ), rank( std::clamp( rank, 1ui8, 14ui8 ) ) { }

CardSuit Card::getSuit() const noexcept {
  return suit;
}

uint8_t Card::getRank() const noexcept {
  return rank;
}

bool Card::beats( const Card &other, CardSuit trump ) const noexcept {
  if ( suit == trump && other.suit != trump )
    return true;

  if ( suit == other.suit && *this > other )
    return true;

  return false;
}

} // namespace durak