#pragma once
#include <godot_cpp/classes/Object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class InputState : public Object {
    GDCLASS(InputState, Object);

protected:
    static void _bind_methods();

public:
    InputState();
    ~InputState();
};