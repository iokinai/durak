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

void DeckWidget::mousePressEvent( QMouseEvent *event ) {
  if ( event->button() != Qt::LeftButton )
    return;

  if ( deck.empty() ) {
    emit playerTakeCard( nullptr );
    return;
  }

  std::unique_ptr<Card> topCard = std::move( deck.top() );
  deck.pop();

  if ( currentTopCardWidget ) {
    ui->verticalLayout->removeWidget( currentTopCardWidget );
    currentTopCardWidget->hide();
    currentTopCardWidget->deleteLater();
    currentTopCardWidget = nullptr;
  }

  if ( !deck.empty() ) {
    Card *nextCard       = deck.top().get();
    currentTopCardWidget = new CardWidget( nextCard, this, false );
    ui->verticalLayout->addWidget( currentTopCardWidget );
  }

  emit playerTakeCard( topCard.release() );
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