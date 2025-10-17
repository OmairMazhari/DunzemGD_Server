#pragma once

#include "State.h"
#include "Global.hpp"

using namespace godot;

class State2 : public ResourceState {
    GDCLASS(State2, ResourceState);
public:
    State2() {}
    ~State2() {}

    // Overriden Functions
    void Enter() override {}
    void Exit() override{}
    void Update(double delta) override {
        SETUP_INPUT
        UtilityFunctions::print("Current running in state 2 : ");
        if(input->is_action_just_pressed("noclip")){
            FSM->change_state( Ref<ResourceState>(Object::cast_to<ResourceState>(FSM->get_states()[0])));
        }
    }
    void Setup() override {}
protected:

    static void _bind_methods()  { 
        
    } 
};