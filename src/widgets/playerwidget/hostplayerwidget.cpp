#include "hostplayerwidget.hpp"
#include "./ui_hostplayerwidget.h"

#include <widgets/cardwidget/clickablecardwidget.hpp>

#include <QMessageBox>
#include <QTimer>

namespace durak {

HostPlayerWidget::HostPlayerWidget( QWidget *parent )
    : PlayerWidget( parent ), ui( new Ui::HostPlayerWidget ) {
  ui->setupUi( this );
}

void HostPlayerWidget::onCardsGiven(
    const QVector<Card *> &givenCards ) noexcept {
  cardsGivenWithType<ClickableCardWidget>( ui, givenCards );
  prepareConnects();
}

void HostPlayerWidget::prepareConnects() noexcept {
  for ( auto card : cards ) {
    auto clickableCard = dynamic_cast<ClickableCardWidget *>( card );
    if ( clickableCard ) {
      connect( clickableCard, &ClickableCardWidget::clicked, this,
               [this]( Card *c ) {
                 switch ( mode ) {
                 case UiMode::Attack :
                   onCardAttackClicked( c );
                   break;

                 case UiMode::Defence :
                   onCardDefenceClicked( c, currentCard );
                   break;
                 case UiMode::Idle :
                   break;
                 }
               } );
    }
  }
}

void HostPlayerWidget::onAttackTurn() noexcept {
  mode = UiMode::Attack;
}

void HostPlayerWidget::onDefenceTurn( Card *attackCard ) noexcept {
  QMessageBox::critical( this, "Defence Turn",
                         QString( "You are attacked by %1 of %2" )
                             .arg( QString::number( attackCard->getRank() ) )
                             .arg( QString::fromStdString(
                                 suitToString( attackCard->getSuit() ) ) ) );

  mode        = UiMode::Defence;
  currentCard = attackCard;
}

void HostPlayerWidget::onCardAttackClicked( Card *card ) noexcept {
  QString cardString =
      QString::number( card->getRank() ) + "\t" +
      QString::fromStdString( suitToString( card->getSuit() ) );

  mode = UiMode::Idle;

  emit attack( card );
}

void HostPlayerWidget::onCardDefenceClicked( Card *card,
                                             Card *attack ) noexcept {

  if ( !card->beats( *attack, CardSuit::DM ) ) {
    QMessageBox::critical( this, "Does not beat",
                           "This card does not beat the attack one" );
    return;
  }

  mode        = UiMode::Idle;
  currentCard = nullptr;
  emit defence( card );
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

HostPlayerWidget::~HostPlayerWidget() {
  delete ui;
}

} // namespace durak