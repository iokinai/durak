#include "card.hpp"
#include <algorithm>

namespace durak {

Card::Card( CardSuit suit, uint8_t rank )
    : suit( suit ), rank( std::clamp( rank, (uint8_t) 1, (uint8_t) 14 ) ) { }

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

bool Card::operator== ( const Card &other ) const noexcept {
  return rank == other.rank && suit == other.suit;
}

bool Card::operator!= ( const Card &other ) const noexcept {
  return !( *this == other );
}

bool Card::operator> ( const Card &other ) const noexcept {
  return rank > other.rank;
}

bool Card::operator>= ( const Card &other ) const noexcept {
  return rank >= other.rank;
}

bool Card::operator< ( const Card &other ) const noexcept {
  return rank < other.rank;
}

bool Card::operator<= ( const Card &other ) const noexcept {
  return rank <= other.rank;
}

Card::operator std::string () const noexcept {
  switch ( rank ) {
  case 11 :
    return "J";
  case 12 :
    return "Q";
  case 13 :
    return "K";
  case 14 :
    return "A";
  default :
    return std::to_string( rank );
  }
}

} // namespace durak