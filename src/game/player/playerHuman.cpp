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

  connect( this, &PlayerHuman::pw_takeCards, playerWidget,
           &HostPlayerWidget::onCardsGiven );
}

void PlayerHuman::pw_onAttacked( Card *attackCard ) noexcept {
  disconnect( playerWidget, &PlayerWidget::attack, this, nullptr );

  emit gc_attacked( moveCard( attackCard ).release() );
}

void PlayerHuman::pw_onDefended( Card *defenceCard ) noexcept {
  disconnect( playerWidget, &PlayerWidget::defence, this, nullptr );

  emit gc_defended( moveCard( defenceCard ).release() );
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

template <typename T>
QVector<T *> toQVectorRaw( const std::vector<std::unique_ptr<T>> &v ) {
  QVector<T *> result;
  result.reserve( v.size() );
  for ( const auto &item : v ) {
    result.append( item.get() );
  }
  return result;
}

void PlayerHuman::takeCards( std::vector<std::unique_ptr<Card>> cards ) {
  this->cards = std::move( cards );
  emit pw_takeCards( toQVectorRaw( this->cards ) );
}

} // namespace durak