#pragma once

#include <game/cards/card.hpp>
#include <variant>

namespace durak {

struct CardWaitResultNoCard { };

struct CardWaitResultAccepted {
  Card *card;
};

struct CardWaitResultRejected { };

using CardWaitResult =
    std::variant<CardWaitResultAccepted, CardWaitResultRejected,
                 CardWaitResultNoCard>;

} // namespace durak