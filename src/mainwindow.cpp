#include "./ui_mainwindow.h"
#include <mainwindow.hpp>
#include <pages/gamepage.hpp>

namespace durak {

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent ), ui( new Ui::MainWindow ),
      gp( new GamePage( this ) ) {
  ui->setupUi( this );
}
MainWindow::~MainWindow() {
  delete ui;
}

} // namespace durak