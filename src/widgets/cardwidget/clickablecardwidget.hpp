#pragma once

#include <widgets/cardwidget/cardwidget.hpp>

namespace durak {

class ClickableCardWidget : public CardWidget {
  Q_OBJECT

signals:
  void clicked( Card *card );

public:
  explicit ClickableCardWidget( Card *card, QWidget *parent = nullptr );

  void mousePressEvent( QMouseEvent *event ) override;
  void returnCard( std::unique_ptr<Card> card ) noexcept;
};

} // namespace durak