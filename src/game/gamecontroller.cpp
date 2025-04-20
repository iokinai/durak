#include "gamecontroller.hpp"
#include <memory>
#include <random>
#include <vector>

namespace durak {

GameController::GameController(
    PlayerBuffer &&b, std::unique_ptr<FSM> fsm,
    std::vector<std::unique_ptr<Card>> heap ) noexcept
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

std::unique_ptr<Card>
GameController::attackRequest( std::shared_ptr<Player> player ) noexcept {
  Card *card = nullptr;

  QEventLoop waitForAttack;

  connect(
      player.get(), &Player::gc_attacked, this,
      [&card, &waitForAttack, player = player.get()]( Card *sent ) mutable {
        disconnect( player, &Player::gc_attacked, nullptr, nullptr );
        card = sent;
        waitForAttack.exit();
      } );

  connect( this, &GameController::uiAttackRequest, player.get(),
           &Player::gc_onAttackTurn );

  emit uiAttackRequest();

  waitForAttack.exec();

  return std::move( std::unique_ptr<Card>( card ) );
}

std::vector<std::unique_ptr<Card>> GameController::randomFromHeap() noexcept {
  std::vector<std::unique_ptr<Card>> res;

  for ( size_t i = 0; i < default_cards_per_player; ++i ) {
    std::uniform_int_distribution<> distr( 0, heap.size() - 1 );

    size_t idx = distr( random_engine );
    res.push_back( std::move( heap[idx] ) );
    heap.erase( heap.begin() + idx );
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
    auto currentPlayer                = b.next();
    std::unique_ptr<Card> currentCard = nullptr;

    Event lastEvent = Event::GameStarted;

    while ( true ) {
      if ( lastEvent == Event::RoundEnded ) {
        break;
      }

      auto action_opt = fsm->onEvent( lastEvent );

      if ( !action_opt.has_value() ) {
        return;
      }

      auto action = action_opt.value();

      switch ( action ) {
      case Action::GiveCards : {
        formatTable();
        lastEvent = Event::RoundStarted;
        break;
      }
      case Action::PlayerAttack : {
        currentCard = std::move( attackRequest( currentPlayer ) );
        lastEvent   = Event::PlayerAttacked;
        break;
      }
      case Action::NextPlayerDefend : {
        // currentPlayer = b.next();
        // auto result   = defenceRequest( currentPlayer );

        // if ( result.has_value() ) {
        //   if ( !result->beats( currentCard.value(), currentTrump ) ) {
        //     emit uiGameLogicError();
        //     break;
        //   }

        //   lastEvent = Event::PlayerDefended;
        //   break;
        // }

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