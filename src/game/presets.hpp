#pragma once

#include <game/cards/card.hpp>
#include <game/fsm/fsm.hpp>

namespace durak {

inline std::unique_ptr<FSM> createTestFSM() {
  auto null_state    = std::make_shared<State>( Action::None );
  auto def           = std::make_shared<State>( Action::StartRound );
  auto prepare_round = std::make_shared<State>( Action::GiveCards );
  auto attack        = std::make_shared<State>( Action::CurrPlayerAttack );
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

inline std::unique_ptr<FSM> createRegularDurakFSM() {
  // We call the attacker current player
  // The defender - next player

  auto null_state    = std::make_shared<State>( Action::None );
  auto def           = std::make_shared<State>( Action::StartRound );
  auto prepare_round = std::make_shared<State>( Action::GiveCards );
  auto attack        = std::make_shared<State>( Action::CurrPlayerAttack );
  auto prev_attack   = std::make_shared<State>( Action::PrevPlayerAttack );
  auto defend        = std::make_shared<State>( Action::NextPlayerDefend );
  auto take_all_cards =
      std::make_shared<State>( Action::DefenderPlayerTakeCards );
  auto end        = std::make_shared<State>( Action::RoundEnd );
  auto deal_cards = std::make_shared<State>( Action::DrawCards );
  auto beat       = std::make_shared<State>( Action::DrawCards );

  def->setTransitions( { { Event::GameStarted, prepare_round } } );
  null_state->setTransitions( { { Event::GameStarted, def } } );
  prepare_round->setTransitions( { { Event::RoundStarted, attack } } );
  attack->setTransitions( { { Event::PlayerAttacked, defend },
                            { Event::RoundEnded, end },
                            { Event::Beat, beat },
                            { Event::AttackFailed, attack } } );

  defend->setTransitions( { { Event::PlayerDefended, prev_attack },
                            { Event::PlayerCantDefend, take_all_cards },
                            { Event::RoundEnded, end },
                            { Event::Beat, beat },
                            { Event::AttackFailed, attack },
                            { Event::DefenceFailed, defend } } );
  take_all_cards->setTransitions(
      { { Event::NextPlayerTookCards, deal_cards } } );
  prev_attack->setTransitions( { { Event::PlayerAttacked, defend },
                                 { Event::Beat, beat },
                                 { Event::AttackFailed, attack } } );
  end->setTransitions( { { Event::NextRoundStarted, attack } } );
  deal_cards->setTransitions( { { Event::NextPlayerTookCards, end } } );
  beat->setTransitions( { { Event::Beat, end } } );

  std::vector<std::shared_ptr<State>> states = {
      null_state,     def, prepare_round, attack, prev_attack, defend,
      take_all_cards, end, deal_cards,    beat };

  return std::move( std::make_unique<FSM>( null_state, states ) );
}

inline std::vector<std::unique_ptr<Card>> createTestCards() {

  auto v = std::vector<std::unique_ptr<Card>>();

  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 1 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 2 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 3 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 4 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 5 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 6 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 7 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 8 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 9 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 10 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 11 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 12 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 13 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Club, 14 ) ) );

  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 1 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 2 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 3 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 4 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 5 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 6 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 7 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 8 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 9 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 10 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 11 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 12 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 13 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Diamond, 14 ) ) );

  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 1 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 2 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 3 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 4 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 5 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 6 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 7 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 8 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 9 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 10 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 11 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 12 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 13 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Heart, 14 ) ) );

  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 1 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 2 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 3 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 4 ) ) );
  // v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 5 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 6 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 7 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 8 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 9 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 10 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 11 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 12 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 13 ) ) );
  v.push_back( std::move( std::make_unique<Card>( CardSuit::Spade, 14 ) ) );

  return std::move( v );
}

} // namespace durak