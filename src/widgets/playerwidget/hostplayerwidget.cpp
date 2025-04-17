#include "hostplayerwidget.hpp"
#include "./ui_hostplayerwidget.h"

#include <widgets/cardwidget/clickablecardwidget.hpp>

namespace durak {

HostPlayerWidget::HostPlayerWidget( QWidget *parent )
    : PlayerWidget( parent ), ui( new Ui::HostPlayerWidget ) {
  ui->setupUi( this );
}

void HostPlayerWidget::onCardsGiven( const QVector<Card *> &cards ) noexcept {
  for ( auto card : cards ) {
    auto cardWidget = new ClickableCardWidget( card, this );
    this->cards.push_back( cardWidget );
    ui->cardsLayout->addWidget( cardWidget );
  }

  emit cardsTook();
}

void HostPlayerWidget::onAttackTurn() noexcept {
  for ( auto card : cards ) {
    auto clickableCard = dynamic_cast<ClickableCardWidget *>( card );
    if ( clickableCard ) {
      connect( clickableCard, &ClickableCardWidget::clicked, this,
               &HostPlayerWidget::onCardAttackClicked );
    }
  }
}

void HostPlayerWidget::onDefenceTurn( const Card &attackCard ) noexcept { }

void HostPlayerWidget::onTakeCards( const QVector<Card *> &cards ) noexcept { }

void HostPlayerWidget::onCardAttackClicked( Card *card ) noexcept {
  emit attack( card );
}

void HostPlayerWidget::onCardDefenceClicked( Card *card ) noexcept { }

HostPlayerWidget::~HostPlayerWidget() {
  delete ui;
}

} // namespace durak