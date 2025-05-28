#pragma once

#include <game/cards/card.hpp>
#include <game/fsm/fsm.hpp>

namespace durak {

inline std::unique_ptr<FSM> createTestFSM() {
  auto null_state    = std::make_shared<State>( Action::None );
  auto def           = std::make_shared<State>( Action::StartRound );
  auto prepare_round = std::make_shared<State>( Action::GiveCards );
  auto attack        = std::make_shared<State>( Action::PlayerAttack );
  auto defend        = std::make_shared<State>( Action::NextPlayerDefend );
  auto end           = std::make_shared<State>( Action::RoundEnd );

  def->setTransitions( { { Event::GameStarted, prepare_round } } );
  null_state->setTransitions( { { Event::GameStarted, def } } );
  prepare_round->setTransitions( { { Event::RoundStarted, attack } } );
  attack->setTransitions(
      { { Event::PlayerAttacked, defend }, { Event::RoundEnded, end } } );
  defend->setTransitions( { { Event::PlayerDefended, attack },
                            { Event::PlayerCantDefend, attack },
                            { Event::RoundEnded, end } } );

  std::vector<std::shared_ptr<State>> allStates = { def, prepare_round, attack,
                                                    defend, null_state };

  return std::make_unique<FSM>( null_state, allStates );
}

inline std::vector<std::unique_ptr<Card>> createTestCards() {

  auto v = std::vector<std::unique_ptr<Card>>();

  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 1 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 2 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 3 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 4 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 5 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 6 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 7 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 8 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 9 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 10 ) ) );

  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 1 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 2 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 3 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 4 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 5 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 6 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 7 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 8 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 9 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 10 ) ) );

  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 1 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 2 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 3 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 4 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 5 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 6 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 7 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 8 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 9 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 10 ) ) );

  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 1 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 2 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 3 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 4 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 5 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 6 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 7 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 8 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 9 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 10 ) ) );

  return std::move( v );
}

} // namespace durak