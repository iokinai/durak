#pragma once

#include <QObject>
#include <clearlayout.hpp>
#include <game/card_throw_result.hpp>
#include <game/cards/card.hpp>
#include <widgets/cardwidget/cardwidget.hpp>

#include <concepts>

namespace durak {

class PlayerWidget : public QWidget {
  Q_OBJECT

protected:
  QVector<CardWidget *> cards;
  CardSuit currentTrump;

  template <std::derived_from<CardWidget> T, class Ui>
  void cardsGivenWithType( Ui *ui, QVector<Card *> &cards ) noexcept {
    this->cards.clear();
    clearLayout( ui->cardsLayout );
    for ( auto card : cards ) {
      auto cardWidget = new T( card, this );
      this->cards.push_back( cardWidget );
      ui->cardsLayout->addWidget( cardWidget, 0, Qt::AlignCenter );
    }
  }

  template <class Ui> void onAcceptedCard( Ui *ui, Card *card ) noexcept {
    auto widget =
        std::find_if( cards.begin(), cards.end(),
                      [card]( CardWidget *c ) { return c->card == card; } );

    if ( widget == cards.end() ) {
      return;
    }

    ui->cardsLayout->removeWidget( *widget );
    ( *widget )->hide();
    ( *widget )->card = nullptr;
    ( *widget )->deleteLater();
    cards.erase( widget );
  }

public:
  inline explicit PlayerWidget( QWidget *parent = nullptr )
      : QWidget( parent ) { }
  virtual ~PlayerWidget() = default;

  virtual void afterSettingCurrentTrump() { }

  inline void setCurrentTrump( CardSuit suit ) noexcept {
    currentTrump = suit;
    afterSettingCurrentTrump();
  }

public slots:
  virtual void onCardsGiven( QVector<Card *> &cards ) noexcept = 0;
  virtual void onAttackTurn() noexcept                         = 0;
  virtual void onDefenceTurn( Card *attackCard ) noexcept      = 0;
  virtual void throwResult( CardThrowResult result,
                            Card *thrown_card ) noexcept       = 0;

signals:
  void attack( Card *card );
  void defence( Card *card );

  void player_playerTakeCardFromDeck( Card *card );
};

} // namespace durak