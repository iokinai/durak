#include "./ui_mainwindow.h"
#include <mainwindow.hpp>
#include <pages/gamepage.hpp>

namespace durak {

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent ), ui( new Ui::MainWindow ) {
  ui->setupUi( this );
}
MainWindow::~MainWindow() {
  delete ui;
}

} // namespace durak