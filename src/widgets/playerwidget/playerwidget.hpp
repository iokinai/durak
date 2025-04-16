#pragma once

#include <QObject>
#include <game/cards/card.hpp>

namespace durak {

class PlayerWidget {
  Q_OBJECT

protected:
  QVector<Card> cards;

public:
  virtual ~PlayerWidget() = default;

protected slots:
  virtual void onCardsGiven( const QVector<Card> &cards ) noexcept = 0;
  virtual void onAttackTurn() noexcept                             = 0;
  virtual void onDefenceTurn( const Card &attackCard ) noexcept    = 0;
  virtual void onTakeCards( const QVector<Card> &cards ) noexcept  = 0;

signals:
  void attack( const Card &card ) noexcept;
  void defence( std::optional<Card> card ) noexcept;
  void cardsTook() noexcept;
};

} // namespace durak