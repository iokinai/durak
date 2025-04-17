#pragma once

#include <QWidget>
#include <game/cards/card.hpp>
#include <game/player/player.hpp>

namespace Ui {
class CardWidget;
}

namespace durak {

class CardWidget : public QWidget {
  Q_OBJECT

  Ui::CardWidget *ui;

protected:
  Card *card;
  Player *player;

  QString loadFont() const noexcept;

public:
  CardWidget( Card *card, QWidget *parent = nullptr );
  ~CardWidget();
};

} // namespace durak