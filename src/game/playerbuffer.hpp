#pragma once

#include <game/player/player.hpp>
#include <initializer_list>
#include <memory>

namespace durak {

class PlayerBuffer {
  std::vector<std::shared_ptr<Player>> players;
  mutable size_t current;

  size_t circlefn( size_t min, size_t max, size_t current ) const noexcept;

public:
  PlayerBuffer(
      std::initializer_list<std::shared_ptr<Player>> &&players ) noexcept;

  std::shared_ptr<Player> next() const noexcept;
  std::shared_ptr<Player> prev() const noexcept;

  PlayerBuffer( PlayerBuffer &&other ) noexcept;
};

} // namespace durak