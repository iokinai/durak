#include "clickablecardwidget.hpp"

#include <QMouseEvent>

namespace durak {

void ClickableCardWidget::mousePressEvent( QMouseEvent *event ) {
  if ( event->button() == Qt::LeftButton ) {
    emit clicked( std::move( card ) );
  }

  QWidget::mousePressEvent( event );
}

void ClickableCardWidget::returnCard( std::unique_ptr<Card> card ) noexcept {
  if ( !card ) {
    card = std::move( card );
  }
}

ClickableCardWidget::ClickableCardWidget( Card *card, QWidget *parent )
    : CardWidget( card, parent ) { }

} // namespace durak