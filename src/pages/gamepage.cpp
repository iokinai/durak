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
#include <game/presets.hpp>

using namespace std::chrono_literals;

namespace durak {

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
  controller = new GameController( std::move( pb ), std::move( fsm ),
                                             std::move( cards ), dw );

  connect( controller, &GameController::putCardOnTable, this,
           &GamePage::onPutCardOnTable );

  QTimer::singleShot( 100ms, this, [this] {
    controller->start();
  } );

  ui->hostPlayerLayout->addWidget( hpw );
  ui->aiPlayerLayout->addWidget( apw );
  ui->deckLayout->addWidget( dw );
}

QWidget *GamePage::prepareWidgetToPutOnTable( Card *card ) noexcept {
  QWidget *setWidget;

  if ( card ) {
    setWidget = new CardWidget( card, this, true );
  } else {
    setWidget = new QWidget( this );
  }

  setWidget->setMinimumSize( 100, 140 );
  setWidget->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );

  return setWidget;
}

void GamePage::onPutCardOnTable( Card *card ) noexcept {
  clearLayout( ui->horizontalLayout_3 );

  QWidget *setWidget = prepareWidgetToPutOnTable( card );

  ui->horizontalLayout_3->addStretch();
  ui->horizontalLayout_3->addWidget( setWidget, 0, Qt::AlignCenter );
  ui->horizontalLayout_3->addStretch();
}

GamePage::~GamePage() {
  delete ui;
}

} // namespace durak