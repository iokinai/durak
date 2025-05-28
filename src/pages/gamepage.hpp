#pragma once

#include <QWidget>
#include <game/playertype.hpp>
#include <widgets/deckwidget/deckwidget.hpp>
#include <widgets/playerwidget/hostplayerwidget.hpp>

namespace Ui {
class GamePage;
}

namespace durak {

class GamePage : public QWidget {
  Q_OBJECT

  Ui::GamePage *ui;
  DeckWidget *dw;

  HostPlayerWidget *hpw;
  AIPlayerWidget *apw;
  GameController *controller;

  void configure();

  QWidget *prepareWidgetToPutOnTable( Card *card ) noexcept;

private slots:
  void onPutCardOnTable( Card *card ) noexcept;
  void onAddCardOnTable( Card *card ) noexcept;
  void onClearTable() noexcept;
  void onRoundEndWithWin( Player *player );

signals:
  void showWinPage( PlayerType winner );

public:
  explicit GamePage( QWidget *parent );
  void reconfigure();
  ~GamePage();
};

} // namespace durak