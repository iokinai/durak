#pragma once

#include "cardsuit.hpp"
#include <cstdint>

namespace durak {

class Card {
  CardSuit suit;
  uint8_t rank;

public:
  Card( CardSuit suit, uint8_t rank );
  CardSuit getSuit() const noexcept;
  uint8_t getRank() const noexcept;

  bool beats( const Card &other, CardSuit trump ) const noexcept;

  template <class Self> bool operator== ( this Self &&self, Self &&other ) {
    return self.rank == other.rank && self.suit == other.suit;
  }

  template <class Self> bool operator!= ( this Self &&self, Self &&other ) {
    return !( self == other );
  }

  template <class Self> bool operator> ( this Self &&self, Self &&other ) {
    return self.rank > other.rank;
  }

  template <class Self> bool operator>= ( this Self &&self, Self &&other ) {
    return self.rank >= other.rank;
  }

  template <class Self> bool operator< ( this Self &&self, Self &&other ) {
    return self.rank < other.rank;
  }

  template <class Self> bool operator<= ( this Self &&self, Self &&other ) {
    return self.rank <= other.rank;
  }
};

} // namespace durak