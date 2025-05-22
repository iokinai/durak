#pragma once

#include <game/player/player.hpp>
#include <widgets/playerwidget/aiplayerwidget.hpp>

namespace durak {

class PlayerAI : public Player {
  Q_OBJECT

  AIPlayerWidget *playerWidget;

protected slots:
  virtual void gc_onAttackTurn() noexcept override;
  virtual void gc_onDefenceTurn( Card *attackCard ) noexcept override;
  virtual void pw_takeCardFromDeck( Card *card ) noexcept override;

public:
  explicit PlayerAI( AIPlayerWidget *playerWidget );
};

} // namespace durak