#pragma once

#include <string>

namespace durak {

enum class CardSuit { Heart, Diamond, Spade, Club };

inline std::string suitToString( CardSuit suit ) noexcept {
  switch ( suit ) {
  case CardSuit::Heart :
    return "{";
  case CardSuit::Diamond :
    return "[";
  case CardSuit::Spade :
    return "}";
  case CardSuit::Club :
    return "]";
  }
}

} // namespace durak