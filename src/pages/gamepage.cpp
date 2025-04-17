#include "gamepage.hpp"
#include "./ui_gamepage.h"
#include <QTimer>
#include <game/fsm/state.hpp>
#include <game/gamecontroller.hpp>
#include <game/player/playerHuman.hpp>

using namespace std::chrono_literals;

namespace durak {

std::unique_ptr<FSM> createTestFSM() {
  auto def           = std::make_shared<State>( Action::StartRound );
  auto prepare_round = std::make_shared<State>( Action::GiveCards );
  auto attack        = std::make_shared<State>( Action::PlayerAttack );
  auto defend        = std::make_shared<State>( Action::NextPlayerDefend );

  def->setTransitions( { { Event::GameStarted, prepare_round } } );
  prepare_round->setTransitions( { { Event::GameStarted, attack } } );
  attack->setTransitions( { { Event::PlayerAttacked, defend } } );
  defend->setTransitions( { { Event::PlayerDefended, attack } } );

  std::vector<std::shared_ptr<State>> allStates = { def, prepare_round, attack,
                                                    defend };

  return std::make_unique<FSM>( def, allStates );
}

std::vector<std::unique_ptr<Card>> createTestCards() {

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

GamePage::GamePage( QWidget *parent )
    : QWidget( parent ), ui( new Ui::GamePage ),
      hpw( new HostPlayerWidget( this ) ) {
  ui->setupUi( this );

  PlayerBuffer pb = {
      std::make_shared<PlayerHuman>( new HostPlayerWidget( this ) ) };

  auto fsm = createTestFSM();
  // TODO: memory leak
  GameController *ctrl = new GameController( std::move( pb ), std::move( fsm ),
                                             std::move( createTestCards() ) );

  QTimer *t = new QTimer( this );

  connect( t, &QTimer::timeout, this, [ctrl] { ctrl->start(); } );

  t->start( 5s );
}

GamePage::~GamePage() {
  delete ui;
}

} // namespace durak