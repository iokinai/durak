#pragma once

#include <variant>

namespace durak {

struct WaitResultCritical { };

template <class T> using WaitResult = std::variant<WaitResultCritical, T>;

} // namespace durak