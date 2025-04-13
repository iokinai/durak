#pragma once

#include <QObject>
#include <game/cards/card.hpp>
#include <game/cards/cardhash.hpp>
#include <unordered_set>
#include <vector>

namespace durak {

class GameController;

class Player : public QObject {
  Q_OBJECT

  friend class GameController;

protected:
  std::unordered_set<Card, CardHash> cards;

public:
  virtual ~Player();

protected slots:
  virtual void onPickCard( const Card &c, CardSuit trump ) = 0;

signals:
  void cardPicked( const Card &result );
  void cantDefend();
};

} // namespace durak