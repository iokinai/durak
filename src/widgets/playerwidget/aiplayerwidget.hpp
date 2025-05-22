#pragma once

#include <widgets/playerwidget/playerwidget.hpp>

namespace Ui {
class AIPlayerWidget;
}

namespace durak {

class AIPlayerWidget : public PlayerWidget {
  Q_OBJECT

  Ui::AIPlayerWidget *ui;

public slots:
  void onCardsGiven( QVector<Card *> &cards ) noexcept override;
  void onAttackTurn() noexcept override;
  void onDefenceTurn( Card *attackCard ) noexcept override;
  virtual void throwResult( CardThrowResult result,
                            Card *thrown_card ) noexcept override;

public:
  explicit AIPlayerWidget( QWidget *parent = nullptr );
  ~AIPlayerWidget() override;
};

} // namespace durak