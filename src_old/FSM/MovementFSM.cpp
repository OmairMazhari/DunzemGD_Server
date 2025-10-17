#include "MovementFSM.h"
#include "godot_cpp/classes/engine.hpp"


MovementFSM::MovementFSM() {
}

MovementFSM::~MovementFSM() {
    
}


void MovementFSM::_process(double delta){
     if (Engine::get_singleton()->is_editor_hint()) {
        return; // don’t run in editor viewport
    }
    if (current_state){
        current_state->Update(delta, player);
        player->move_and_slide();
    }
}


void MovementFSM::_bind_methods() {
	// ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &MovementFSM::_physics_process);
    ClassDB::bind_method(D_METHOD("set_initial_state", "state"), &MovementFSM::set_initial_state);
    ClassDB::bind_method(D_METHOD("get_initial_state"), &MovementFSM::get_initial_state);

	ADD_PROPERTY(
    PropertyInfo(Variant::OBJECT, "initial_state", PROPERTY_HINT_NODE_TYPE, "State"),
    "set_initial_state", "get_initial_state"
);
}
