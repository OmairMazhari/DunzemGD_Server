#include "FSM.h"
#include "godot_cpp/classes/engine.hpp"

void FSM::_bind_methods() {
    // Export Variables
    ClassDB::bind_method(D_METHOD("set_initial_state", "state"), &FSM::set_initial_state);
    ClassDB::bind_method(D_METHOD("get_initial_state"), &FSM::get_initial_state);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "initial_state", PROPERTY_HINT_NODE_TYPE, "State"),"set_initial_state", "get_initial_state");
}

void FSM::_ready() {
    TypedArray<Node> children = get_children();
    for(int i = 0; i < children.size(); i++){
        Node* child = Object::cast_to<Node>(children[i]);
        if(child){
            State* state = Object::cast_to<State>(child);
            if(state) {
                states[child->get_name().to_lower()] = state;
                UtilityFunctions::print(child->get_name().to_lower());
                UtilityFunctions::print(state->connect("state_transition", Callable(this, "change_state"))); 
            }
        }
        if (initial_state != nullptr) {
            current_state = Object::cast_to<State>(initial_state);
            current_state->Enter();
        }
    }
}

void FSM::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return; // don’t run in editor viewport
    }
    if (current_state != nullptr){
        current_state->Update(delta);
    }
}

void FSM::change_state(State* old_state, StringName new_state_name) {
    UtilityFunctions::print("Old State Name: " + old_state->get_name() + " New State Name: " + new_state_name);
    UtilityFunctions::print("Change State called");
    if(current_state != old_state){
        // ERROR
        UtilityFunctions::print("ERROR_CHANGE_STATE::CURRENT_STATE!=OLD_STATE");
        return;
    }
    State* new_state = states[new_state_name.to_lower()];
    if(new_state == nullptr){ 
        UtilityFunctions::print("ERROR_CHANGE_STATE::NEW_STATE=NULL");
        // ERROR NEW STATE EMPTY
        return;
    }
    if(current_state){
        current_state->Exit();
    }
    new_state->Enter();
    current_state = new_state;
}

