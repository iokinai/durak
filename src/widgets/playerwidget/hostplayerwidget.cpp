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

void HostPlayerWidget::onDefenceTurn( Card *attackCard ) noexcept {
  QMessageBox::critical( this, "Defence Turn",
                         QString( "You are attacked by %1 of %2" )
                             .arg( QString::number( attackCard->getRank() ) )
                             .arg( QString::fromStdString(
                                 suitToString( attackCard->getSuit() ) ) ) );
}

void HostPlayerWidget::onCardAttackClicked( Card *card ) noexcept {
  QString cardString =
      QString::number( card->getRank() ) + "\t" +
      QString::fromStdString( suitToString( card->getSuit() ) );

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