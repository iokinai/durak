#include "./ui_mainwindow.h"
#include <mainwindow.hpp>
#include <pages/gamepage.hpp>

namespace durak {

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent ), ui( new Ui::MainWindow ),
      gp( new GamePage( this ) ), wp( nullptr ) {
  ui->setupUi( this );

  connect( gp, &GamePage::showWinPage, this, &MainWindow::showWinPage );

  ui->pages->addWidget( gp );
  ui->pages->setCurrentWidget( gp );
}

void MainWindow::showWinPage( PlayerType winner ) {
  if ( wp ) {
    delete wp;
    wp = nullptr;
  }

  wp = new WinPage( this, winner );

  connect( wp, &WinPage::startAgain, this, &MainWindow::onWpStartAgain );

  ui->pages->addWidget( wp );
  ui->pages->setCurrentWidget( wp );
}

void MainWindow::onWpStartAgain() {
  gp->reconfigure();
  ui->pages->setCurrentWidget( gp );

  ui->pages->removeWidget( wp );
  delete wp;
  wp = nullptr;
}

MainWindow::~MainWindow() {
  delete ui;

  if ( wp ) {
    delete wp;
  }
}

} // namespace durak