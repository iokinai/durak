#pragma once

#include <QEventLoop>
#include <QObject>
#include <game/cards/card.hpp>
#include <game/fsm/fsm.hpp>
#include <game/player/player.hpp>
#include <game/playerbuffer.hpp>
#include <memory>

namespace durak {

class GameController : public QObject {
  Q_OBJECT
  PlayerBuffer b;
  FSM fsm;

signals:
  void playerPickCard( const Card &current );
  void playerPickedCard();
  void uiShuffleCards();

public:
  GameController( PlayerBuffer &&b, const FSM &fsm ) noexcept;

  Card current_card();

  void doStartActions( Action act ) noexcept;

  void shuffleCards() noexcept;

  void start() noexcept;

  void gameLoop() noexcept;

  [[deprecated( "TEST" )]]
  void testloop() {
    while ( true ) {
      auto cc     = current_card();
      auto player = b.next();
      QEventLoop el;
      Card picked;

      connect( this, &GameController::playerPickCard, player.get(),
               &Player::onPickCard );

      connect( player.get(), &Player::cardPicked, this,
               [&el, &picked]( const Card &result ) {
                 picked = result;
                 el.quit();
               } );

      emit playerPickCard( cc );

      el.exec();

      disconnect( this, &GameController::playerPickCard, player.get(),
                  &Player::onPickCard );

      disconnect( player.get(), &Player::cardPicked, this, nullptr );

      // do something with card
    }
  }
};

} // namespace durak