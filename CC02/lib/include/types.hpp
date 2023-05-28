#pragma once

#include <unordered_map>
#include <vector>

enum Action
{
    Left,
    Down,
    Right,
    Up,
    NumActions
};

using State = unsigned long;
using Transition = std::tuple<float, State, float, bool>;
using TransitionPossibilities = std::vector<Transition>;
using StateTable = std::unordered_map<Action, TransitionPossibilities>;
using P = std::unordered_map<State, StateTable>;

