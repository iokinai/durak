#pragma once

#include "cardsuit.hpp"
#include <cstdint>

#include <QMessageBox>

namespace durak {

class Card {
  CardSuit suit;
  uint8_t rank;

public:
  Card( CardSuit suit, uint8_t rank );

  Card( const Card &other ) = delete;
  Card( Card &&other )      = default;

  Card &operator= ( const Card &other ) = delete;
  Card &operator= ( Card &&other )      = default;

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