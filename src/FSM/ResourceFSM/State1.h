#pragma once

#include "State.h"
#include "Global.hpp"

using namespace godot;

class State1 : public ResourceState {
    GDCLASS(State1, ResourceState);
    
public:
    State1() {}
    ~State1() {}

    // Overriden Functions
    void Enter() override {}
    void Exit() override{}
      void Update(double delta) override {
        SETUP_INPUT
        UtilityFunctions::print("Current running in state 1 : ");
        if(input->is_action_just_pressed("noclip")){
            FSM->change_state(Ref<ResourceState>(Object::cast_to<ResourceState>(FSM->get_states()[1])));
        }
    }
    void Setup() override {}

protected:

    static void _bind_methods()  { 
        
    } 
}
;