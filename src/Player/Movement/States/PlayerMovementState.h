#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>

#include <FSM/NodeFSM/State.h>
#include <Player/Movement/StateMachine/PlayerMovementFSM.h>
#include <Player/Movement/PlayerMovementStatsResource.h>
#include <FSM/ResourceFSM/State.h>

using namespace godot;

class PlayerMovementState : public ResourceState {
    GDCLASS(PlayerMovementState, ResourceState);
public:
    PlayerMovementState() { }
    ~PlayerMovementState() { }

    void Setup(ResourceFSM* p_FSM) override{
        this->FSM = Object::cast_to<PlayerMovementFSM>(p_FSM);
        if(FSM){
            player = Object::cast_to<FPSController>(FSM->get_player());
        } else { 
            UtilityFunctions::print("ERROR:FSM_NULL_IN_MOVEMENT_STATE");
        }
    }

protected:
    PlayerMovementFSM* FSM = nullptr;
    FPSController* player = nullptr;
    Dictionary input;

    static void _bind_methods(){
    }
    
public:
    PlayerMovementFSM* get_FSM() { return FSM; }
    void set_FSM (PlayerMovementFSM* p_val) { FSM = p_val; }

    void set_input_dict(Dictionary p_val) { input = p_val; }
    Dictionary get_input_dict() { return input; }
};