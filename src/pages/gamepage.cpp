#include "gamepage.hpp"
#include "./ui_gamepage.h"
#include <QTimer>
#include <clearlayout.hpp>
#include <game/fsm/state.hpp>
#include <game/gamecontroller.hpp>
#include <game/player/playerAI.hpp>
#include <game/player/playerHuman.hpp>
#include <widgets/playerwidget/aiplayerwidget.hpp>
#include <widgets/playerwidget/hostplayerwidget.hpp>

using namespace std::chrono_literals;

namespace durak {

std::unique_ptr<FSM> createTestFSM() {
  auto null_state    = std::make_shared<State>( Action::None );
  auto def           = std::make_shared<State>( Action::StartRound );
  auto prepare_round = std::make_shared<State>( Action::GiveCards );
  auto attack        = std::make_shared<State>( Action::PlayerAttack );
  auto defend        = std::make_shared<State>( Action::NextPlayerDefend );

  def->setTransitions( { { Event::GameStarted, prepare_round } } );
  null_state->setTransitions( { { Event::GameStarted, def } } );
  prepare_round->setTransitions( { { Event::RoundStarted, attack } } );
  attack->setTransitions( { { Event::PlayerAttacked, defend } } );
  defend->setTransitions( { { Event::PlayerDefended, attack },
                            { Event::PlayerCantDefend, attack } } );

  std::vector<std::shared_ptr<State>> allStates = { def, prepare_round, attack,
                                                    defend, null_state };

  return std::make_unique<FSM>( null_state, allStates );
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
      hpw( new HostPlayerWidget( this->dw, this ) ),
      apw( new AIPlayerWidget( this ) ), dw( new DeckWidget( this ) ) {
  ui->setupUi( this );
  dw->setFixedSize( 100, 140 );

  PlayerBuffer pb = { std::make_shared<PlayerHuman>( hpw ),
                      std::make_shared<PlayerAI>( apw ) };

  auto fsm   = createTestFSM();
  auto cards = createTestCards();
  // TODO: memory leak
  GameController *ctrl = new GameController( std::move( pb ), std::move( fsm ),
                                             std::move( cards ), dw );

  connect( ctrl, &GameController::putCardOnTable, this,
           &GamePage::onPutCardOnTable );

  QTimer *t = new QTimer( this );

  connect( t, &QTimer::timeout, this, [ctrl] { ctrl->start(); } );

  t->start( 5s );

  ui->hostPlayerLayout->addWidget( hpw );
  ui->aiPlayerLayout->addWidget( apw );
  ui->deckLayout->addWidget( dw );
}

void GamePage::onPutCardOnTable( Card *card ) noexcept {
  clearLayout( ui->horizontalLayout_3 );

  if ( !card )
    return;

  auto setWidget = new CardWidget( card, this, true );
  setWidget->setMinimumSize( 100, 140 );
  setWidget->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );

  ui->horizontalLayout_3->addStretch();
  ui->horizontalLayout_3->addWidget( setWidget, 0, Qt::AlignCenter );
  ui->horizontalLayout_3->addStretch();
}

GamePage::~GamePage() {
  delete ui;
}

} // namespace durak