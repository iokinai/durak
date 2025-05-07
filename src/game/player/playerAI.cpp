#include "playerAI.hpp"

namespace durak {

void PlayerAI::gc_onAttackTurn() noexcept {
  if ( cards.empty() ) {
    return;
  }

  emit gc_attacked( cards.front().get() );
}

void PlayerAI::gc_onDefenceTurn( Card *attackCard ) noexcept {
  auto def = std::find_if( cards.begin(), cards.end(),
                           [&attackCard]( const auto &card ) {
                             return ( *card > *attackCard ) &&
                                    card->getSuit() == attackCard->getSuit();
                           } );

  Card *defCard = nullptr;

  if ( def != cards.end() ) {
    defCard = def->get();
  }

  emit gc_defended( defCard );
}

PlayerAI::PlayerAI( AIPlayerWidget *playerWidget )
    : playerWidget( playerWidget ) {

  connect( this, &PlayerAI::pw_takeCards, playerWidget,
           &AIPlayerWidget::onCardsGiven );

  connect( this, &PlayerAI::pw_handleThrowResult, playerWidget,
           &AIPlayerWidget::throwResult );
}

} // namespace durak