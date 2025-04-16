#pragma once

namespace durak {

enum class Action {
  StartRound,
  PlayerAttack,
  NextPlayerDefend,
  RoundEnd,
  PlayerTakeCards,
  DrawCards,
};

}