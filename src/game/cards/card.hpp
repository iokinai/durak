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

  bool operator== ( const Card &other ) const noexcept;

  bool operator!= ( const Card &other ) const noexcept;

  bool operator> ( const Card &other ) const noexcept;

  bool operator>= ( const Card &other ) const noexcept;

  bool operator< ( const Card &other ) const noexcept;

  bool operator<= ( const Card &other ) const noexcept;

  operator std::string () const noexcept;
};

} // namespace durak