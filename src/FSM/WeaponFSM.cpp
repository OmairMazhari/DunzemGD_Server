#include "WeaponFSM.h"

WeaponFSM::WeaponFSM() {
}

WeaponFSM::~WeaponFSM() {
}


void WeaponFSM::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return; // don’t run in editor viewport
    }

    if (!current_state) {
        return;
    }

    // Handle weapon input
    if (Input::get_singleton()->is_action_just_pressed("primary")) {
        change_state(current_state, "primary");
    }
    if (Input::get_singleton()->is_action_just_pressed("secondary")) {
        change_state(current_state, "secondary");
    }
    if (Input::get_singleton()->is_action_just_pressed("melee")) {
        change_state(current_state, "melee");
    }

    current_state->Update(delta, player);
}

void WeaponFSM::_bind_methods() {
    // Export initial_state
    ClassDB::bind_method(D_METHOD("set_initial_state", "state"), &WeaponFSM::set_initial_state);
    ClassDB::bind_method(D_METHOD("get_initial_state"), &WeaponFSM::get_initial_state);

    ADD_PROPERTY(
        PropertyInfo(Variant::OBJECT, "initial_state", PROPERTY_HINT_NODE_TYPE, "State"),
        "set_initial_state",
        "get_initial_state"
    );

    // Export player
    ClassDB::bind_method(D_METHOD("set_player", "player"), &WeaponFSM::set_player);
    ClassDB::bind_method(D_METHOD("get_player"), &WeaponFSM::get_player);

    ADD_PROPERTY(
        PropertyInfo(Variant::OBJECT, "player", PROPERTY_HINT_NODE_TYPE, "Player"),
        "set_player",
        "get_player"
    );

    
}
