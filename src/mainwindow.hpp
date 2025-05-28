#pragma once

#include <QMainWindow>
#include <pages/gamepage.hpp>
#include <pages/winpage.hpp>

namespace Ui {
class MainWindow;
}

namespace durak {

class MainWindow : public QMainWindow {
  Q_OBJECT
  Ui::MainWindow *ui;

  GamePage *gp;
  WinPage *wp;

public:
  explicit MainWindow( QWidget *parent = nullptr );
  ~MainWindow();

public slots:
  void showWinPage( PlayerType winner );
  void onWpStartAgain();
};

} // namespace durak