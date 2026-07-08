#pragma once

#include <deque>
#include <string>

struct Model {
    using container = std::deque<std::string>;
    container items;
    container::iterator iterator = items.begin();
};