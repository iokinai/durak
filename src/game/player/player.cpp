#include "player.hpp"

namespace durak {

std::unique_ptr<Card> Player::moveCard( Card *c ) {
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

void Player::takeCards( std::vector<std::unique_ptr<Card>> cards ) {
  this->cards = std::move( cards );
  emit pw_takeCards( toQVectorRaw( this->cards ) );
}

void Player::gc_cardThrowResult( CardThrowResult result,
                                 Card *thrown_card ) noexcept {
  if ( !thrown_card ) {
    return;
  }

  switch ( result ) {
  case CardThrowResult::Accepted : {
    emit pw_handleThrowResult( result, moveCard( thrown_card ).release() );
    break;
  }
  default :
    emit pw_handleThrowResult( result, nullptr );
    break;
  }
}

} // namespace durak