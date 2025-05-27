#include "cardwidget.hpp"
#include "./ui_cardwidget.h"
#include <QFontDatabase>
#include <QPainter>
#include <QPixmap>
#include <QResizeEvent>
#include <font.hpp>

namespace durak {

CardWidget::CardWidget( Card *card, QWidget *parent, bool isFaceUp )
    : QWidget( parent ), card( card ), ui( new Ui::CardWidget ),
      isFaceUp( isFaceUp ) {
  ui->setupUi( this );

  setAutoFillBackground( true );
  setAttribute( Qt::WA_StyledBackground, true );

  QFont font( loadFont(), 20, QFont::Bold );

  ui->rankTopText->setFont( font );

  auto cardStr = QString::fromStdString( *card );

  ui->rankTopText->setText( cardStr );

  font.setPointSize( 12 );

  ui->suitTopImage->setFont( font );
  ui->suitTopImage->setText(
      QString::fromStdString( suitToString( card->getSuit() ) ) );

  faceUpdate();
}

void CardWidget::faceUpdate() noexcept {
  QString color    = isFaceUp ? "255, 255, 255" : "30, 144, 255";
  QString colorRGB = QString( "rgb(%1)" ).arg( color );
  QString backImage;

  if ( !isFaceUp ) {
    backImage = "background-image: url(:/images/cardback.png)";
  }

  setStyleSheet(
      QString(
          "#CardWidget { background-color: %1; %2; } QLabel { color: black; }" )
          .arg( colorRGB )
          .arg( backImage ) );

  ui->suitTopImage->setText(
      isFaceUp ? QString::fromStdString( suitToString( card->getSuit() ) )
               : " " );
  ui->rankTopText->setText( isFaceUp ? QString::fromStdString( *card ) : " " );
}

void CardWidget::setFaceUp( bool isFaceUp ) noexcept {
  this->isFaceUp = isFaceUp;
  faceUpdate();
}

CardWidget::~CardWidget() {
  delete ui;
}

} // namespace durak