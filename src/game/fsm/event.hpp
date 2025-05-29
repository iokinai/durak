#pragma once

namespace durak {

enum class Event {
  GameStarted,
  RoundStarted,
  PlayerAttacked,
  PlayerDefended,
  PlayerCantDefend,
  RoundEnded,
  CardsDrawed,
  NextPlayerTookCards,
  NextRoundStarted,
  Beat,
};

}