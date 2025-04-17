#pragma once

#include <game/player/player.hpp>
#include <widgets/playerwidget/hostplayerwidget.hpp>

namespace durak {

class PlayerHuman : public Player {
private:
  HostPlayerWidget *playerWidget;

protected:
  virtual void takeCards( std::vector<std::unique_ptr<Card>> cards ) override;

protected slots:
  virtual void gc_onAttackTurn() noexcept override;
  virtual void gc_onDefenceTurn( const Card &attackCard ) noexcept override;

  virtual void pw_onAttacked( Card *attackCard ) noexcept override;
  virtual void pw_onDefended( Card *defenceCard ) noexcept override;

public:
  explicit PlayerHuman( HostPlayerWidget *playerWidget );
  virtual std::unique_ptr<Card> moveCard( Card *c ) override;
};

} // namespace durak