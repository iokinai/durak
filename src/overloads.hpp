#pragma once

namespace durak {

template <class... Ts> struct overloads : Ts... {
  using Ts::operator()...;
};

} // namespace durak