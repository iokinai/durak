#include "playerAI.hpp"

#include <algorithm>

namespace durak {

void PlayerAI::onPickCard( const Card &c, CardSuit trump ) {
  auto found = std::find_if(
      cards.begin(), cards.end(),
      [&c, trump]( const Card &card ) { return card.beats( c, trump ); } );

  if ( found == cards.end() ) {
    emit cantDefend();
    return;
  }

  cards.erase( found );

  emit cardPicked( *found );
}

} // namespace durak