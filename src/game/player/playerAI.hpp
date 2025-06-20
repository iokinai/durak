#pragma once

#include <game/player/player.hpp>
#include <widgets/playerwidget/aiplayerwidget.hpp>

namespace durak {

class PlayerAI : public Player {
  Q_OBJECT

  AIPlayerWidget *playerWidget;

  Card *selectCardToAttack() const noexcept;

protected slots:
  virtual void gc_onAttackTurn() noexcept override;
  virtual void gc_onDefenceTurn( Card *attackCard ) noexcept override;
  virtual void pw_takeCardFromDeck( Card *card ) noexcept override;
  virtual void gc_setCurrentTrump( CardSuit suit ) noexcept override;

public:
  explicit PlayerAI( AIPlayerWidget *playerWidget );
};

} // namespace durak