#pragma once

#include "FSM.h"
#include "State.h"
#include "Player.h"
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class WeaponFSM : public FSM {
    GDCLASS(WeaponFSM, FSM);

public:
    WeaponFSM();
    ~WeaponFSM();

protected:
    static void _bind_methods();

public:
    void _process(double delta) override;
};
