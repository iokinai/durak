#pragma once

namespace durak {

enum class Action {
  StartRound,
  GiveCards,
  CurrPlayerAttack,
  PrevPlayerAttack,
  NextPlayerDefend,
  RoundEnd,
  DefenderPlayerTakeCards,
  DrawCards,
  None,
  EndGame,
};

}