#include "deckwidget.hpp"
#include "./ui_deckwidget.h"
#include <QMouseEvent>

namespace durak {

DeckWidget::DeckWidget( QWidget *parent )
    : QWidget( parent ), ui( new Ui::DeckWidget ) {
  ui->setupUi( this );
}

DeckWidget::~DeckWidget() {
  delete ui;
}

CardWidget *DeckWidget::getCardTopWidget() {
  if ( deck.empty() )
    return nullptr;

  Card *topCard          = deck.top().get();
  CardWidget *cardWidget = new CardWidget( topCard, this, false );
  return cardWidget;
}

std::unique_ptr<Card> DeckWidget::takeTopCard() noexcept {
  if ( deck.empty() ) {
    return nullptr;
  }

  std::unique_ptr<Card> topCard = std::move( deck.top() );
  deck.pop();

  return topCard;
}

void DeckWidget::putCards( std::vector<std::unique_ptr<Card>> &cards ) {
  for ( auto &card : cards ) {
    deck.emplace( std::move( card ) );
  }

  if ( currentTopCardWidget ) {
    ui->verticalLayout->removeWidget( currentTopCardWidget );
    currentTopCardWidget->deleteLater();
    currentTopCardWidget = nullptr;
  }

  if ( !deck.empty() ) {
    currentTopCardWidget = getCardTopWidget();
    ui->verticalLayout->addWidget( currentTopCardWidget );
  }
}

} // namespace durak
