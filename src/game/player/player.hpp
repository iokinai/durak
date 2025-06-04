#pragma once

#include <QObject>
#include <game/card_throw_result.hpp>
#include <game/cards/card.hpp>
#include <game/cards/cardhash.hpp>
#include <memory>

namespace durak {

class GameController;

class Player : public QObject {
  Q_OBJECT

  friend class GameController;

protected:
  template <typename T>
  QVector<T *> toQVectorRaw( std::vector<std::unique_ptr<T>> &v ) {
    QVector<T *> result;
    result.reserve( v.size() );
    for ( const auto &item : v ) {
      result.append( item.get() );
    }
    return result;
  }

  CardSuit currentTrump;

  std::vector<std::unique_ptr<Card>> cards;
  virtual void takeCards( std::vector<std::unique_ptr<Card>> &&cards );
  const std::vector<std::unique_ptr<Card>> *table;

public:
  virtual ~Player() = default;

  virtual std::unique_ptr<Card> moveCard( Card *c );
  virtual void setTable( const std::vector<std::unique_ptr<Card>> *table );

protected slots:
  virtual void gc_onAttackTurn() noexcept                    = 0;
  virtual void gc_onDefenceTurn( Card *attackCard ) noexcept = 0;

  virtual void gc_cardThrowResult( CardThrowResult result,
                                   Card *thrown_card ) noexcept;

  virtual void pw_takeCardFromDeck( Card *card ) noexcept   = 0;
  virtual void gc_setCurrentTrump( CardSuit suit ) noexcept = 0;
  virtual void pw_beaten() noexcept;

signals:
  void pw_takeCards( QVector<Card *> &cards );
  void pw_handleThrowResult( CardThrowResult result, Card *thrown_card );
  void gc_attacked( Card *attackCard );
  void gc_defended( Card *defenceCard );
  void gc_player_takeCardFromDeck( Card *card, Player *player );
  void gc_player_takeCurrentCard( Player *player );
  void gc_player_noCards( Player *player );
  void gc_playerBeaten( Player *player );
};

} // namespace durak