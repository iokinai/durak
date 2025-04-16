#include "cardwidget.hpp"
#include "./ui_cardwidget.h"
#include <QFontDatabase>
#include <QPainter>
#include <QPixmap>

namespace durak {

QString CardWidget::loadFont() const noexcept {
  int id         = QFontDatabase::addApplicationFont( ":/fonts/card.ttf" );
  QString family = QFontDatabase::applicationFontFamilies( id ).at( 0 );

  return family;
}

CardWidget::CardWidget( Card &&card, QWidget *parent )
    : QWidget( parent ), card( std::move( card ) ), ui( new Ui::CardWidget ) {
  ui->setupUi( this );

  QFont font( loadFont(), 20, QFont::Bold );

  ui->rankTopText->setFont( font );
  ui->rankTopText->setText( ( QString::fromStdString( card ) ) );

  font.setPointSize( 12 );

  ui->suitTopImage->setFont( font );
  ui->suitTopImage->setText(
      QString::fromStdString( suitToString( card.getSuit() ) ) );
}

CardWidget::~CardWidget() {
  delete ui;
}

} // namespace durak