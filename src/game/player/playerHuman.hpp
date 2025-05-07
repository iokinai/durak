#pragma once

#include <game/player/player.hpp>
#include <widgets/playerwidget/hostplayerwidget.hpp>

namespace durak {

class PlayerHuman : public Player {
  Q_OBJECT

private:
  HostPlayerWidget *playerWidget;

protected slots:
  virtual void gc_onAttackTurn() noexcept override;
  virtual void gc_onDefenceTurn( Card *attackCard ) noexcept override;

  virtual void pw_onAttacked( Card *attackCard ) noexcept;
  virtual void pw_onDefended( Card *defenceCard ) noexcept;

signals:
  void pw_attackTurn();
  void pw_defenceTurn( Card *attackCard );

public:
  explicit PlayerHuman( HostPlayerWidget *playerWidget );
};

} // namespace durak