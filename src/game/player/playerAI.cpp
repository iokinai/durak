#include "playerAI.hpp"

#include <algorithm>

namespace durak {

void PlayerAI::onPickCard( const Card &c, CardSuit trump ) {
  auto found = std::find_if( cards.begin(), cards.end(),
                             [&c, trump]( std::unique_ptr<Card> &card ) {
                               return card->beats( c, trump );
                             } );

  if ( found == cards.end() ) {
    // emit cardPickResult( nullptr );
    return;
  }

  cards.erase( found );

  // emit cardPickResult( std::move( *found ) );
}

} // namespace durak