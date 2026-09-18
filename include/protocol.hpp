#pragma once

#include "store.hpp"

#include <string>

namespace redislite {

/// Parse one text protocol line and return the reply (including trailing newline).
/// Supported: PING, QUIT, SET [EX seconds], GET, DEL, EXISTS.
std::string handle_line(Store &store, const std::string &line);

}  // namespace redislite
