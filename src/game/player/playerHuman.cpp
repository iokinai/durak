#include "playerHuman.hpp"

namespace durak {

std::unique_ptr<Card> PlayerHuman::moveCard( Card *c ) {
  auto iter =
      std::find_if( cards.begin(), cards.end(),
                    [c]( const auto &card ) { return card.get() == c; } );

  if ( iter == cards.end() ) {
    return nullptr;
  }

  std::unique_ptr<Card> card = std::move( *iter );
  cards.erase( iter );
  return card;
}

PlayerHuman::PlayerHuman( HostPlayerWidget *playerWidget )
    : playerWidget( playerWidget ) {

  connect( this, &PlayerHuman::pw_attackTurn, playerWidget,
           &HostPlayerWidget::onAttackTurn );

  connect( this, &PlayerHuman::pw_defenceTurn, playerWidget,
           &HostPlayerWidget::onDefenceTurn );
}

void PlayerHuman::pw_onAttacked( Card *attackCard ) noexcept {
  emit gc_attacked( attackCard );
}
void PlayerHuman::pw_onDefended( Card *defenceCard ) noexcept {
  emit gc_defended( defenceCard );
}

void PlayerHuman::gc_onAttackTurn() noexcept {
  connect( playerWidget, &PlayerWidget::attack, this,
           &PlayerHuman::pw_onAttacked );

  emit pw_attackTurn();
}

void PlayerHuman::gc_onDefenceTurn( const Card &attackCard ) noexcept {
  connect( playerWidget, &PlayerWidget::defence, this,
           &PlayerHuman::pw_onDefended );

  emit pw_defenceTurn( attackCard );
}

} // namespace durak