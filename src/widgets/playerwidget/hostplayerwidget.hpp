#pragma once

#include <widgets/cardwidget/clickablecardwidget.hpp>
#include <widgets/deckwidget/deckwidget.hpp>
#include <widgets/playerwidget/playerwidget.hpp>

namespace Ui {
class HostPlayerWidget;
}

namespace durak {

enum class UiMode {
  Idle,
  Attack,
  Defence,
};

class HostPlayerWidget : public PlayerWidget {
  Q_OBJECT

  Ui::HostPlayerWidget *ui;

  UiMode mode = UiMode::Idle;
  Card *currentCard;

  DeckWidget *deckWidget;

  void onCardAttackClicked( Card *card ) noexcept;
  void onCardDefenceClicked( Card *card, Card *attack ) noexcept;

  void prepareConnects() noexcept;

public slots:
  void onCardsGiven( const QVector<Card *> &cards ) noexcept override;
  void onAttackTurn() noexcept override;
  void onDefenceTurn( Card *attackCard ) noexcept override;
  virtual void throwResult( CardThrowResult result,
                            Card *thrown_card ) noexcept override;

public:
  explicit HostPlayerWidget( DeckWidget *widget, QWidget *parent = nullptr );
  ~HostPlayerWidget() override;
};

} // namespace durak