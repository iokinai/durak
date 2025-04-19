#include "cardwidget.hpp"
#include "./ui_cardwidget.h"
#include <QFontDatabase>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QResizeEvent>

namespace durak {

QString CardWidget::loadFont() const noexcept {
  int id         = QFontDatabase::addApplicationFont( ":/fonts/card.ttf" );
  QString family = QFontDatabase::applicationFontFamilies( id ).at( 0 );

  return family;
}

CardWidget::CardWidget( Card *card, QWidget *parent )
    : QWidget( parent ), card( card ), ui( new Ui::CardWidget ) {
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
}

CardWidget::~CardWidget() {
  delete ui;
}

} // namespace durak