#pragma once

#include <QWidget>

namespace Ui {
class GamePage;
}

namespace durak {

class GamePage : public QWidget {
  Q_OBJECT

  Ui::GamePage *ui;

public:
  explicit GamePage( QWidget *parent );
  ~GamePage();
};

} // namespace durak