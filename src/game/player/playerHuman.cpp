#include "playerHuman.hpp"

namespace durak {

PlayerHuman::PlayerHuman( HostPlayerWidget *playerWidget )
    : playerWidget( playerWidget ) {

  connect( this, &PlayerHuman::pw_attackTurn, playerWidget,
           &HostPlayerWidget::onAttackTurn );

  connect( this, &PlayerHuman::pw_defenceTurn, playerWidget,
           &HostPlayerWidget::onDefenceTurn );

  connect( this, &PlayerHuman::pw_takeCards, playerWidget,
           &HostPlayerWidget::onCardsGiven );

  connect( this, &PlayerHuman::pw_handleThrowResult, playerWidget,
           &HostPlayerWidget::throwResult );
}

void PlayerHuman::pw_onAttacked( Card *attackCard ) noexcept {
  emit gc_attacked( attackCard );
}

void PlayerHuman::pw_onDefended( Card *defenceCard ) noexcept {
  emit gc_defended( defenceCard );
}

void PlayerHuman::gc_onAttackTurn() noexcept {
  connect( playerWidget, &PlayerWidget::attack, this,
           &PlayerHuman::pw_onAttacked, Qt::SingleShotConnection );

  emit pw_attackTurn();
}

void PlayerHuman::gc_onDefenceTurn( Card *attackCard ) noexcept {
  connect( playerWidget, &PlayerWidget::defence, this,
           &PlayerHuman::pw_onDefended, Qt::SingleShotConnection );

  emit pw_defenceTurn( attackCard );
}

} // namespace durak