#pragma once

#include <QMainWindow>
#include <pages/gamepage.hpp>

namespace Ui {
class MainWindow;
}

namespace durak {

class MainWindow : public QMainWindow {
  Q_OBJECT
  Ui::MainWindow *ui;

  GamePage *gp;

public:
  explicit MainWindow( QWidget *parent = nullptr );
  ~MainWindow();
};

} // namespace durak