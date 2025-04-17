#pragma once

namespace durak {

enum class Action {
  StartRound,
  GiveCards,
  PlayerAttack,
  NextPlayerDefend,
  RoundEnd,
  PlayerTakeCards,
  DrawCards,
};

}