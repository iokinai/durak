#pragma once

#include <QWidget>
#include <game/cards/card.hpp>

namespace Ui {
class CardWidget;
}

namespace durak {

class CardWidget : public QWidget {
  Q_OBJECT

  Ui::CardWidget *ui;

  Card card;

  QString loadFont() const noexcept;

public:
  CardWidget( Card &&card, QWidget *parent = nullptr );
  ~CardWidget();
};

} // namespace durak