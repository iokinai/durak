#pragma once

#include <QEventLoop>
#include <QObject>
#include <game/cards/card.hpp>
#include <game/cardwaitresultmarker.hpp>
#include <game/fsm/fsm.hpp>
#include <game/player/player.hpp>
#include <game/playerbuffer.hpp>
#include <game/waitresult.hpp>
#include <memory>
#include <random>
#include <widgets/deckwidget/deckwidget.hpp>

namespace durak {

class GameController : public QObject {
  Q_OBJECT

  enum class CurrentTurn {
    Attack,
    Defence,
    Idle,
  } currentTurn = CurrentTurn::Idle;

  enum class WaitExitReason {
    None,
    Unknown,
    Ok,
    Exiting,
    OtherVariantRetNullptr,
  } exitReason = WaitExitReason::None;

  PlayerBuffer b;
  std::unique_ptr<FSM> fsm;
  std::vector<std::unique_ptr<Card>> heap;
  DeckWidget *deck;

  std::random_device random_device;
  std::mt19937 random_engine;

  std::shared_ptr<Player> currentPlayer;
  Card *currentCard;
  std::vector<std::unique_ptr<Card>> table;
  CardSuit currentTrump;
  Event lastEvent;

  std::shared_ptr<Player> attacker;
  std::shared_ptr<Player> defender;

  void dealCardsTo6() noexcept;

  QEventLoop wait;

  static constexpr const int default_cards_per_player = 6;

signals:
  void playerPickCard( const Card &current );
  void playerPickedCard();
  void uiShuffleCards();
  void uiDealCards();
  void uiAttackRequest();
  void uiDefenceRequest( Card *attackCard );
  void uiGameLogicError();
  void cardThrowResult( CardThrowResult result, Card *thrown );
  void putCardOnTable( Card *card );
  void setCurrentTrump( CardSuit suit );
  void addCardOnTable( Card *card );
  void clearTable();
  void roundEndWithWin( Player *player );

private slots:
  void playerTakeCardFromDeck( Card *card, Player *player ) noexcept;
  void playerTakeCardsFromTable( Player *player ) noexcept;

public:
  GameController( PlayerBuffer &&b, std::unique_ptr<FSM> fsm,
                  std::vector<std::unique_ptr<Card>> heap,
                  DeckWidget *deck ) noexcept;

  std::vector<std::unique_ptr<Card>> randomFromHeap() noexcept;

  void shuffleHeap( std::vector<std::unique_ptr<Card>> &heap ) noexcept;

  void start() noexcept;
  void gameLoop() noexcept;
  void dealCards() noexcept;
  void formatTable() noexcept;

  WaitResult<CardWaitResult>
  attackRequest( std::shared_ptr<Player> player ) noexcept;
  WaitResult<CardWaitResult> defenceRequest( std::shared_ptr<Player> player,
                                             Card *attackCard ) noexcept;
};

} // namespace durak