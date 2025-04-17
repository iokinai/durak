#pragma once

#include <QObject>
#include <game/cards/card.hpp>
#include <game/cards/cardhash.hpp>
#include <memory>

namespace durak {

class GameController;

class Player : public QObject {
  Q_OBJECT

  friend class GameController;

protected:
  std::vector<std::unique_ptr<Card>> cards;
  virtual void takeCards( std::vector<std::unique_ptr<Card>> cards ) = 0;

public:
  virtual ~Player()                                 = default;
  virtual std::unique_ptr<Card> moveCard( Card *c ) = 0;

protected slots:
  // virtual void onPickCard( const Card &c, CardSuit trump ) = 0;
  virtual void gc_onAttackTurn() noexcept                          = 0;
  virtual void gc_onDefenceTurn( const Card &attackCard ) noexcept = 0;

  virtual void pw_onAttacked( Card *attackCard ) noexcept  = 0;
  virtual void pw_onDefended( Card *defenceCard ) noexcept = 0;

signals:
  void pw_attackTurn();
  void pw_defenceTurn( const Card &attackCard );
  void pw_takeCards( const QVector<Card *> &cards );

  void gc_attacked( Card *attackCard );
  void gc_defended( Card *defenceCard );
};

} // namespace durak