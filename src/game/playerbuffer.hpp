#pragma once

#include <game/player/player.hpp>
#include <initializer_list>
#include <memory>
#include <vector>

namespace durak {

class PlayerBuffer {
  std::vector<std::shared_ptr<Player>> players;
  mutable int64_t current;

  using iterator       = std::vector<std::shared_ptr<Player>>::iterator;
  using const_iterator = std::vector<std::shared_ptr<Player>>::const_iterator;

  size_t circlefn( int64_t min, int64_t max, int64_t current ) const noexcept;

public:
  PlayerBuffer(
      std::initializer_list<std::shared_ptr<Player>> &&players ) noexcept;

  std::shared_ptr<Player> next() const noexcept;
  std::shared_ptr<Player> prev() const noexcept;

  size_t size() const noexcept;
  size_t get_current() const noexcept;

  PlayerBuffer( PlayerBuffer &&other ) noexcept;

  iterator begin() noexcept;
  iterator end() noexcept;

  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  std::shared_ptr<Player> operator[] ( size_t idx ) const noexcept;
};

} // namespace durak