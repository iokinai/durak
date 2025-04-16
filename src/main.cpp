#include "mainwindow.hpp"
#include <QApplication>

#include <widgets/cardwidget/cardwidget.hpp>

int main( int argc, char **argv ) {
  QApplication app( argc, argv );

  durak::MainWindow w;
  w.show();

  durak::CardWidget cw( durak::Card( durak::CardSuit::Diamond, 12 ) );
  cw.show();

  return app.exec();
}