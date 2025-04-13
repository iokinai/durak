#include "gamepage.hpp"
#include "./ui_gamepage.h"

namespace durak {

GamePage::GamePage( QWidget *parent )
    : QWidget( parent ), ui( new Ui::GamePage ) {
  ui->setupUi( this );
}
GamePage::~GamePage() {
  delete ui;
}

} // namespace durak