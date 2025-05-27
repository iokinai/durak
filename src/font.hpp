#pragma once

#include <QFontDatabase>
#include <QList>
#include <QString>


namespace durak {

inline QString loadFont() noexcept {
  int id         = QFontDatabase::addApplicationFont( ":/fonts/card.ttf" );
  QString family = QFontDatabase::applicationFontFamilies( id ).at( 0 );

  return family;
}

} // namespace durak