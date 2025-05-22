#include "playerbuffer.hpp"
#include <cstring>

namespace durak {

size_t PlayerBuffer::circlefn( size_t min, size_t max,
                               int64_t current ) const noexcept {
  if ( current > max ) {
    return min;
  }

  if ( current < min ) {
    return max;
  }

  return current;
}

PlayerBuffer::PlayerBuffer(
    std::initializer_list<std::shared_ptr<Player>> &&players ) noexcept
    : players( players.begin(), players.end() ), current( -1 ) { }

std::shared_ptr<Player> PlayerBuffer::next() const noexcept {
  current = circlefn( 0, players.size() - 1, ++current );
  return players[current];
}

std::shared_ptr<Player> PlayerBuffer::prev() const noexcept {
  current = circlefn( 0, players.size(), current - 1 );
  return players[current];
}

PlayerBuffer::PlayerBuffer( PlayerBuffer &&other ) noexcept
    : players( std::move( other.players ) ),
      current( std::move( other.current ) ) { }

size_t PlayerBuffer::size() const noexcept {
  return players.size();
}

PlayerBuffer::iterator PlayerBuffer::begin() noexcept {
  return players.begin();
}

PlayerBuffer::iterator PlayerBuffer::end() noexcept {
  return players.end();
}

PlayerBuffer::const_iterator PlayerBuffer::cbegin() const noexcept {
  return players.cbegin();
}

PlayerBuffer::const_iterator PlayerBuffer::cend() const noexcept {
  return players.cend();
}

std::shared_ptr<Player> PlayerBuffer::operator[] ( size_t idx ) const noexcept {
  return players[idx % players.size()];
}

} // namespace durak