#pragma once

#include <game/cards/card.hpp>
#include <memory>

namespace durak {

struct DefenceResultNoCard { };

struct DefenceResultAccepted {
  std::unique_ptr<Card> card;
};

struct DefenceResultRejected { };

using DefenceResult = std::variant<DefenceResultAccepted, DefenceResultRejected,
                                   DefenceResultNoCard>;

} // namespace durak