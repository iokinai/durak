#pragma once

#include <QObject>
#include <game/cards/card.hpp>
#include <widgets/cardwidget/cardwidget.hpp>

namespace durak {

class PlayerWidget : public QWidget {
  Q_OBJECT

protected:
  QVector<CardWidget *> cards;

public:
  inline explicit PlayerWidget( QWidget *parent = nullptr )
      : QWidget( parent ) { }
  virtual ~PlayerWidget() = default;

public slots:
  virtual void onCardsGiven( const QVector<Card *> &cards ) noexcept = 0;
  virtual void onAttackTurn() noexcept                               = 0;
  virtual void onDefenceTurn( const Card &attackCard ) noexcept      = 0;

signals:
  void attack( Card *card );
  void defence( Card *card );
};

} // namespace durak