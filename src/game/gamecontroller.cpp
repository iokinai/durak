#include "gamecontroller.hpp"
#include <algorithm>
#include <memory>
#include <random>
#include <vector>

namespace durak {

GameController::GameController(
    PlayerBuffer &&b, std::unique_ptr<FSM> fsm,
    std::vector<std::unique_ptr<Card>> &&heap ) noexcept
    : b( std::move( b ) ), fsm( std::move( fsm ) ), heap( std::move( heap ) ),
      random_device(), random_engine( random_device() ) { }

void GameController::start() noexcept {
  std::optional<Action> act = fsm->onEvent( Event::GameStarted );

  if ( !act.has_value() ) {
    return;
  }

  doStartActions( act.value() );

  gameLoop();
}

void GameController::doStartActions( Action act ) noexcept {
  // switch ( act ) {

  // case Action::ShuffleTheCards :
  //   shuffleCards();
  //   break;
  // case Action::DealTheCards :
  //   dealCards();
  // default :
  //   return;
  // }
}

void GameController::shuffleCards() noexcept {
  emit uiShuffleCards();
}

void GameController::dealCards() noexcept {
  emit uiDealCards();
}

Card GameController::attackRequest( std::shared_ptr<Player> player ) noexcept {
  emit uiAttackRequest();
}

std::vector<std::unique_ptr<Card>> GameController::randomFromHeap() noexcept {
  std::vector<std::unique_ptr<Card>> res;

  std::uniform_int_distribution<> distr( 0, heap.size() );

  for ( size_t i = 0; i < default_cards_per_player; ++i ) {
    auto card = std::move( heap[distr( random_engine )] );
    std::erase( heap, card );

    res.push_back( std::move( card ) );
  }

  return res;
}

void GameController::formatTable() noexcept {
  int playersCardsCount = b.size() * default_cards_per_player;

  for ( const auto &player : b ) {
    player->takeCards( randomFromHeap() );
  }
}

std::optional<Card>
GameController::defenceRequest( std::shared_ptr<Player> player ) noexcept {
  return std::nullopt;
}

void GameController::gameLoop() noexcept {
  CardSuit currentTrump;

  while ( true ) {
    auto currentPlayer              = b.next();
    std::optional<Card> currentCard = std::nullopt;

    Event lastEvent = Event::GameStarted;

    while ( true ) {
      if ( lastEvent == Event::RoundEnded ) {
        break;
      }

      auto action = fsm->onEvent( lastEvent );

      switch ( action.value() ) {
      case Action::GiveCards : {
        formatTable();
        lastEvent = Event::RoundStarted;
        break;
      }
      case Action::PlayerAttack : {
        currentCard = attackRequest( currentPlayer );
        lastEvent   = Event::PlayerAttacked;
        break;
      }
      case Action::NextPlayerDefend : {
        currentPlayer = b.next();
        auto result   = defenceRequest( currentPlayer );

        if ( result.has_value() ) {
          if ( !result->beats( currentCard.value(), currentTrump ) ) {
            emit uiGameLogicError();
            break;
          }

          lastEvent = Event::PlayerDefended;
          break;
        }

        lastEvent = Event::PlayerCantDefend;
        break;
      }
      case Action::PlayerTakeCards :
      case Action::DrawCards :
        break;
      case Action::RoundEnd :
        lastEvent = Event::RoundEnded;
        break;
      default :
        break;
      }
    }
  }
}

} // namespace durak