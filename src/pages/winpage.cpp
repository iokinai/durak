#include "winpage.hpp"
#include "ui_winpage.h"

namespace durak {

WinPage::WinPage( QWidget *parent, PlayerType type )
    : QWidget( parent ), ui( new Ui::WinPage ), t( type ) {
  ui->setupUi( this );

  switch ( type ) {

  case PlayerType::Human :
    ui->textLoseWin->setText( "выиграли" );
    ui->textLoseWin->setStyleSheet( "color: green;" );
    break;
  case PlayerType::AI :
    ui->textLoseWin->setText( "проиграли" );
    ui->textLoseWin->setStyleSheet( "color: red;" );
    break;
  }

  connect( ui->startAgain, &QPushButton::clicked, this,
           [this]() { emit startAgain(); } );
}

} // namespace durak