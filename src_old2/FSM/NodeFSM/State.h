#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/signal.hpp>

using namespace godot;

class State : public Node {
    GDCLASS(State, Node);   
public:
    State(){ }
    ~State(){ }

    // Virtual Member Functions
    virtual void Enter() {}
    virtual void Exit() {}
    virtual void Update(double delta) {}

protected:
    static void _bind_methods(){
        // Signals
        ADD_SIGNAL(MethodInfo("state_transition", PropertyInfo(Variant::OBJECT , "old_state"),PropertyInfo(Variant::STRING, "new_state_name")));
    }
};