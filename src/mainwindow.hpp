#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace durak {

class MainWindow : public QMainWindow {
  Q_OBJECT
  Ui::MainWindow *ui;

public:
  explicit MainWindow( QWidget *parent = nullptr );
  ~MainWindow();
};

} // namespace durak