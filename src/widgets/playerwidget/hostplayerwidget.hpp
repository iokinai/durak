#pragma once

#include <widgets/cardwidget/clickablecardwidget.hpp>
#include <widgets/playerwidget/playerwidget.hpp>

namespace Ui {
class HostPlayerWidget;
}

namespace durak {

class HostPlayerWidget : public PlayerWidget {
  Q_OBJECT

  Ui::HostPlayerWidget *ui;

public slots:
  void onCardsGiven( const QVector<Card *> &cards ) noexcept override;
  void onAttackTurn() noexcept override;
  void onDefenceTurn( const Card &attackCard ) noexcept override;
  void onTakeCards( const QVector<Card *> &cards ) noexcept override;

  void onCardAttackClicked( Card *card ) noexcept;
  void onCardDefenceClicked( Card *card ) noexcept;

public:
  explicit HostPlayerWidget( QWidget *parent = nullptr );
  ~HostPlayerWidget() override;
};

} // namespace durak