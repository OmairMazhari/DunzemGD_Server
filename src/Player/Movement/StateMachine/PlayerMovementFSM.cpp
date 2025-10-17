#include "PlayerMovementFSM.h"

#include <godot_cpp/classes/input.hpp>
#include <Player/Movement/States/PlayerMovementState.h>
#include "utils.hpp"
#include "global.hpp"

using namespace godot;

PlayerMovementFSM::PlayerMovementFSM() {}
// k
PlayerMovementFSM::~PlayerMovementFSM() {}

void PlayerMovementFSM::_ready() {
    GAME_ONLY   
    // Initialize the state in the array first
    ResourceFSM::_ready();

    // Set up it's FSM and then call it's setup function 
    for(int i = 0; i < states.size(); i++){
    Ref<PlayerMovementState> state = Ref<PlayerMovementState>(Object::cast_to<PlayerMovementState>(states[i]));
    if(state.is_valid()){
        state->Setup(this);
    }
}
    
}

void PlayerMovementFSM::_process(double delta) {

}

void PlayerMovementFSM::_physics_process(double delta) {
    
}


void PlayerMovementFSM::Update(double delta, Dictionary input_dict) {
    // Update input dict of the active state
    Ref<PlayerMovementState> current_movement_state = Ref<PlayerMovementState>(current_state);
    if(current_movement_state.is_valid()) {
        current_movement_state->set_input_dict(input_dict);
    }
    ResourceFSM::Update(delta);
    this->input_dict = input_dict;
}



void PlayerMovementFSM::_bind_methods() {
    // player
    ClassDB::bind_method(D_METHOD("set_player", "p_player"), &PlayerMovementFSM::set_player);
    ClassDB::bind_method(D_METHOD("get_player"), &PlayerMovementFSM::get_player);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "player", PROPERTY_HINT_NODE_TYPE, "FPSController"), "set_player", "get_player");
    
    // Update function
    ClassDB::bind_method(D_METHOD("Update", "delta", "input_dict"), &PlayerMovementFSM::Update);

}