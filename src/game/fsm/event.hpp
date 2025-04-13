#pragma once

namespace durak {

enum class Event {
  GameStarted,
  DealingCards,
  PlayerAttack,
  NextPlayerDefend,
  PlayerCantDefend,
  PlayerTakeCards,
  DefendedSuccessfully,
  PlayerNoCards,
  PlayerWin,
  PlayerLost,
  GameEnded,
};

}