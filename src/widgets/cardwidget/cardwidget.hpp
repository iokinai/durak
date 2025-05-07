#pragma once

#include <QWidget>
#include <game/cards/card.hpp>
#include <game/player/player.hpp>

namespace Ui {
class CardWidget;
}

namespace durak {

class AIPlayerWidget;

class CardWidget : public QWidget {
  Q_OBJECT

  friend class PlayerWidget;
  friend class HostPlayerWidget;

  Ui::CardWidget *ui;

protected:
  Card *card;
  bool isFaceUp;

  void faceUpdate() noexcept;

  QString loadFont() const noexcept;

public:
  CardWidget( Card *card, QWidget *parent = nullptr, bool isFaceUp = false );
  ~CardWidget();

  void setFaceUp( bool isFaceUp ) noexcept;

  friend class AIPlayerWidget;
};

} // namespace durak