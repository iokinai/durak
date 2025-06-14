#include "hostplayerwidget.hpp"
#include "./ui_hostplayerwidget.h"

#include <widgets/cardwidget/clickablecardwidget.hpp>

#include <QMessageBox>
#include <QTimer>
#include <font.hpp>
#include <widgets/deckwidget/deckwidget.hpp>

namespace durak {

HostPlayerWidget::HostPlayerWidget( DeckWidget *widget, QWidget *parent )
    : PlayerWidget( parent ), ui( new Ui::HostPlayerWidget ),
      deckWidget( widget ) {
  ui->setupUi( this );

  connect( ui->takeButton, &QPushButton::clicked, this,
           &HostPlayerWidget::takeCurrentCard );

  connect( ui->beatenButton, &QPushButton::clicked, this,
           &HostPlayerWidget::callBeaten );
}

void HostPlayerWidget::onCardsGiven( QVector<Card *> &givenCards ) noexcept {
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
  ui->takeButton->setEnabled( false );
  ui->beatenButton->setEnabled( true );
}

void HostPlayerWidget::onDefenceTurn( Card *attackCard ) noexcept {
  mode        = UiMode::Defence;
  currentCard = attackCard;
  ui->takeButton->setEnabled( true );
  ui->beatenButton->setEnabled( false );
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
    QMessageBox::critical( this, "Wrong card",
                           "This card can not be used at this moment" );
    break;
  }
}

void HostPlayerWidget::takeCurrentCard() noexcept {
  emit player_takeCurrentCard();
  emit defence( nullptr );
}

void HostPlayerWidget::callBeaten() noexcept {
  emit player_playerBeaten();
  emit attack( nullptr );
}

void HostPlayerWidget::afterSettingCurrentTrump() {
  ui->currentTrumpText->setFont( QFont( loadFont(), 20, QFont::Bold ) );
  ui->currentTrumpText->setText(
      QString::fromStdString( suitToString( currentTrump ) ) );
}

HostPlayerWidget::~HostPlayerWidget() {
  delete ui;
}

} // namespace durak