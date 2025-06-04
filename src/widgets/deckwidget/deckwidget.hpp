#pragma once

#include <QWidget>
#include <game/cards/card.hpp>
#include <stack>
#include <widgets/cardwidget/cardwidget.hpp>

namespace Ui {
class DeckWidget;
}

namespace durak {

class DeckWidget : public QWidget {
  Q_OBJECT

  friend class GameController;

  std::stack<std::unique_ptr<Card>> deck;
  Ui::DeckWidget *ui;
  CardWidget *currentTopCardWidget = nullptr;

  CardWidget *getCardTopWidget();

public:
  explicit DeckWidget( QWidget *parent = nullptr );
  ~DeckWidget();

  void putCards( std::vector<std::unique_ptr<Card>> &cards );
  std::unique_ptr<Card> takeTopCard() noexcept;
};

} // namespace durak