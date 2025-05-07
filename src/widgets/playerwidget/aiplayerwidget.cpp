#include "aiplayerwidget.hpp"
#include "./ui_aiplayerwidget.h"

namespace durak {

AIPlayerWidget::AIPlayerWidget( QWidget *parent )
    : PlayerWidget( parent ), ui( new Ui::AIPlayerWidget ) {
  ui->setupUi( this );
}

void AIPlayerWidget::onCardsGiven( const QVector<Card *> &cards ) noexcept {
  cardsGivenWithType<CardWidget>( ui, cards );
}

void AIPlayerWidget::onDefenceTurn( Card *attackCard ) noexcept { }

void AIPlayerWidget::onAttackTurn() noexcept {
  if ( cards.empty() ) {
    return;
  }

  emit attack( cards.first()->card );
}

void AIPlayerWidget::throwResult( CardThrowResult result,
                                  Card *thrown_card ) noexcept {
  switch ( result ) {

  case CardThrowResult::Accepted : {
    onAcceptedCard( ui, thrown_card );

    break;
  }
  case CardThrowResult::RejectedRequiersRepeat :
    break;
  }
}

AIPlayerWidget::~AIPlayerWidget() {
  delete ui;
}

} // namespace durak