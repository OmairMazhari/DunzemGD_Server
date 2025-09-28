#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node.hpp>

#include "State.h" 

using namespace godot;

class FSM : public Node {
    GDCLASS(FSM, Node);

public:
    FSM() { };
    ~FSM() { };

    // Overriden functions
    void _ready() override;
    void _process(double delta) override;

    // Class functions
    void change_state(State* old_state, StringName new_state_name);

    // Getter and setters
    void set_initial_state(Node* p_state) { initial_state = Object::cast_to<State>(p_state); }
    Node* get_initial_state() {return initial_state; }

protected:
    static void _bind_methods();

protected:
    // State Variables
    std::unordered_map<StringName, State*> states;
    State* current_state = nullptr;
    State* initial_state = nullptr;
};




