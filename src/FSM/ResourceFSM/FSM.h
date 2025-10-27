#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <FSM/ResourceFSM/State.h>

using namespace godot;

class ResourceState;

class ResourceFSM : public Node {
    GDCLASS(ResourceFSM, Node);
public:
    ResourceFSM();
    ~ResourceFSM();

    // Overriden functions
    virtual void Update(double delta);
    virtual void unhanlded_input_update(const Ref<InputEvent> &p_event);
    void _ready() override;
    
    
    // Class functions    

    void change_state(Ref<ResourceState> new_state);

protected:
    // Export Variablesvoid Setup(Ref<ResourceState> weapon);
    TypedArray<ResourceState> states;
    // Class Variables
    Ref<ResourceState> current_state;

public:
    // Settes and getters

    void set_states(const TypedArray<ResourceState> p_states) {
        states = p_states;
    }
    TypedArray<ResourceState> get_states() const { 
        return states; 
    }

protected:
	static void _bind_methods();
};