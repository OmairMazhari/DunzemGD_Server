#pragma once

#include <godot_cpp/core/class_db.hpp>
#include "State.h"

using namespace godot;

class PlayerWeaponState : public State {
    GDCLASS(PlayerWeaponState, State);

protected:
    static void _bind_methods();

public:
    PlayerWeaponState();
    ~PlayerWeaponState();
};