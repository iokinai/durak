#include "hostplayerwidget.hpp"
#include "./ui_hostplayerwidget.h"

#include <widgets/cardwidget/clickablecardwidget.hpp>

#include <QMessageBox>

namespace durak {

HostPlayerWidget::HostPlayerWidget( QWidget *parent )
    : PlayerWidget( parent ), ui( new Ui::HostPlayerWidget ) {
  ui->setupUi( this );
}

void HostPlayerWidget::onCardsGiven( const QVector<Card *> &cards ) noexcept {
  cardsGivenWithType<ClickableCardWidget>( ui, cards );
}

void HostPlayerWidget::onAttackTurn() noexcept {
  for ( auto card : cards ) {
    auto clickableCard = dynamic_cast<ClickableCardWidget *>( card );
    if ( clickableCard ) {
      connect( clickableCard, &ClickableCardWidget::clicked, this,
               &HostPlayerWidget::onCardAttackClicked, Qt::UniqueConnection );
    }
  }
}

void HostPlayerWidget::onDefenceTurn( Card *attackCard ) noexcept { }

void HostPlayerWidget::onCardAttackClicked( Card *card ) noexcept {
  QString cardString =
      QString::number( card->getRank() ) + "\t" +
      QString::fromStdString( suitToString( card->getSuit() ) );

  // QMessageBox::information( this, "Clicked card", cardString );
  emit attack( card );
}

void HostPlayerWidget::throwResult( CardThrowResult result,
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

void HostPlayerWidget::onCardDefenceClicked( Card *card ) noexcept { }

HostPlayerWidget::~HostPlayerWidget() {
  delete ui;
}

} // namespace durak