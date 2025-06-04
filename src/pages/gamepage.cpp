#include "gamepage.hpp"
#include "./ui_gamepage.h"
#include <QTimer>
#include <clearlayout.hpp>
#include <game/fsm/state.hpp>
#include <game/gamecontroller.hpp>
#include <game/player/playerAI.hpp>
#include <game/player/playerHuman.hpp>
#include <game/presets.hpp>
#include <widgets/playerwidget/aiplayerwidget.hpp>
#include <widgets/playerwidget/hostplayerwidget.hpp>

#include <QMessageBox>

using namespace std::chrono_literals;

namespace durak {

GamePage::GamePage( QWidget *parent )
    : QWidget( parent ), ui( new Ui::GamePage ),
      hpw( new HostPlayerWidget( this->dw, this ) ),
      apw( new AIPlayerWidget( this ) ), dw( new DeckWidget( this ) ) {
  ui->setupUi( this );
  dw->setFixedSize( 100, 140 );

  configure();
}

QWidget *GamePage::prepareWidgetToPutOnTable( Card *card ) noexcept {
  QWidget *setWidget;

  if ( card ) {
    setWidget = new CardWidget( card, this, true );
  }

  return setWidget;
}

void GamePage::onAddCardOnTable( Card *card ) noexcept {
  QWidget *setWidget = prepareWidgetToPutOnTable( card );

  ui->horizontalLayout_3->addWidget( setWidget, 0, Qt::AlignCenter );
}

void GamePage::onClearTable() noexcept {
  clearLayout( ui->horizontalLayout_3 );
  ui->horizontalLayout_3->addWidget( new QWidget(this), 0, Qt::AlignCenter );
}

void GamePage::onRoundEndWithWin( Player *player ) {
  PlayerType t;

  if ( dynamic_cast<PlayerAI *>( player ) ) {
    t = PlayerType::AI;
  } else if ( dynamic_cast<PlayerHuman *>( player ) ) {
    t = PlayerType::Human;
  }

  emit showWinPage( t );
}

void GamePage::configure() {
  PlayerBuffer pb = { std::make_shared<PlayerHuman>( hpw ),
                      std::make_shared<PlayerAI>( apw ) };

  auto fsm   = createRegularDurakFSM();
  auto cards = createTestCards();
  controller = new GameController( std::move( pb ), std::move( fsm ),
                                   std::move( cards ), dw );

  connect( controller, &GameController::addCardOnTable, this,
           &GamePage::onAddCardOnTable );

  connect( controller, &GameController::clearTable, this,
           &GamePage::onClearTable );

  connect( controller, &GameController::roundEndWithWin, this,
           &GamePage::onRoundEndWithWin );

  QTimer::singleShot( 100ms, this, [this] { controller->start(); } );

  ui->hostPlayerLayout->addWidget( hpw );
  ui->aiPlayerLayout->addWidget( apw );
  ui->deckLayout->addWidget( dw );
}

void GamePage::reconfigure() {
  if ( controller ) {
    delete controller;
    controller = nullptr;
  }

  configure();
}

GamePage::~GamePage() {
  delete ui;

  if ( controller ) {
    delete controller;
  }
}

} // namespace durak