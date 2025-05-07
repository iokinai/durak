#pragma once

#include <QWidget>
#include <widgets/playerwidget/hostplayerwidget.hpp>

namespace Ui {
class GamePage;
}

namespace durak {

class GamePage : public QWidget {
  Q_OBJECT

  Ui::GamePage *ui;

  HostPlayerWidget *hpw;
  AIPlayerWidget *apw;

public:
  explicit GamePage( QWidget *parent );
  ~GamePage();
};

} // namespace durak