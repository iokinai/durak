#pragma once

#include <QEventLoop>
#include <QObject>
#include <game/cards/card.hpp>
#include <game/fsm/fsm.hpp>
#include <game/player/player.hpp>
#include <game/playerbuffer.hpp>
#include <memory>
#include <random>

namespace durak {

class GameController : public QObject {
  Q_OBJECT

  PlayerBuffer b;
  std::unique_ptr<FSM> fsm;
  std::vector<std::unique_ptr<Card>> heap;

  std::random_device random_device;
  std::mt19937 random_engine;

  static constexpr const int default_cards_per_player = 6;

signals:
  void playerPickCard( const Card &current );
  void playerPickedCard();
  void uiShuffleCards();
  void uiDealCards();
  void uiAttackRequest();
  void uiDefenceRequest();
  void uiGameLogicError();

public:
  GameController( PlayerBuffer &&b, std::unique_ptr<FSM> fsm,
                  std::vector<std::unique_ptr<Card>> heap ) noexcept;

  std::vector<std::unique_ptr<Card>> randomFromHeap() noexcept;

  Card current_card();

  void doStartActions( Action act ) noexcept;
  void shuffleCards() noexcept;
  void start() noexcept;
  void gameLoop() noexcept;
  void dealCards() noexcept;
  void formatTable() noexcept;

  std::unique_ptr<Card>
  attackRequest( std::shared_ptr<Player> player ) noexcept;
  std::optional<Card> defenceRequest( std::shared_ptr<Player> player ) noexcept;

  // [[deprecated( "TEST" )]]
  // void testloop() {
  //   while ( true ) {
  //     auto cc     = current_card();
  //     auto player = b.next();
  //     QEventLoop el;
  //     Card picked;

  //     connect( this, &GameController::playerPickCard, player.get(),
  //              &Player::onPickCard );

  //     connect( player.get(), &Player::cardPicked, this,
  //              [&el, &picked]( const Card &result ) {
  //                picked = result;
  //                el.quit();
  //              } );

  //     emit playerPickCard( cc );

  //     el.exec();

  //     disconnect( this, &GameController::playerPickCard, player.get(),
  //                 &Player::onPickCard );

  //     disconnect( player.get(), &Player::cardPicked, this, nullptr );

  //     // do something with card
  //   }
  // }
};

} // namespace durak