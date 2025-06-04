#include "playerAI.hpp"
#include <QRandomGenerator>
#include <QTimer>

namespace durak {

void PlayerAI::gc_onAttackTurn() noexcept {
  if ( cards.empty() || !table ) {
    return;
  }

  QTimer::singleShot( 1000, [&]() {
    if ( QRandomGenerator::global()->generate() % 5 >= 4 ) {
      emit gc_playerBeaten( this );
      emit gc_attacked( nullptr );
      return;
    }

    auto cardToAttack = selectCardToAttack();

    if ( !cardToAttack ) {
      emit gc_playerBeaten( this );
      emit gc_attacked( nullptr );
      return;
    }

    emit gc_attacked( cardToAttack );
  } );
}

Card *PlayerAI::selectCardToAttack() const noexcept {
  if ( !table || table->empty() ) {
    return cards.front().get();
  }

  for ( auto &c : *table ) {
    auto card = std::find_if( cards.begin(), cards.end(),
                              [&c]( const std::unique_ptr<Card> &card ) {
                                return c->getRank() == card->getRank();
                              } );

    if ( card != cards.end() ) {
      return card->get();
    }
  }

  return nullptr;
}

void PlayerAI::gc_onDefenceTurn( Card *attackCard ) noexcept {
  auto def = std::find_if( cards.begin(), cards.end(),
                           [&attackCard, this]( const auto &card ) {
                             return card->beats( *attackCard, currentTrump );
                           } );

  QTimer::singleShot( 1000, [def, this]() {
    Card *defCard = nullptr;

    if ( def != cards.end() ) {
      defCard = def->get();
    } else {
      emit gc_player_takeCurrentCard( this );
    }

    emit gc_defended( defCard );
  } );
}

PlayerAI::PlayerAI( AIPlayerWidget *playerWidget )
    : playerWidget( playerWidget ) {
  connect( this, &PlayerAI::pw_takeCards, playerWidget,
           &AIPlayerWidget::onCardsGiven );

  connect( this, &PlayerAI::pw_handleThrowResult, playerWidget,
           &AIPlayerWidget::throwResult );
}

void PlayerAI::pw_takeCardFromDeck( Card *card ) noexcept { }

void PlayerAI::gc_setCurrentTrump( CardSuit suit ) noexcept {
  currentTrump = suit;
  playerWidget->setCurrentTrump( suit );
}

} // namespace durak