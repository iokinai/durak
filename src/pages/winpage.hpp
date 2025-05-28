#pragma once

#include <QWidget>
#include <game/playertype.hpp>

namespace Ui {
class WinPage;
}

namespace durak {

class WinPage : public QWidget {
  Q_OBJECT

  Ui::WinPage *ui;
  PlayerType t;

public:
  explicit WinPage( QWidget *parent, PlayerType type );

signals:
  void startAgain();
};

} // namespace durak