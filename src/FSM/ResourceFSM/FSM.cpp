#include "FSM.h"

#include "Global.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>

using namespace godot;

ResourceFSM::ResourceFSM() {
    // Default value initialization 
    for(int i = 0; i < states.size(); i++){
        states[i] = Ref<Resource>();
    }
}

ResourceFSM::~ResourceFSM() {

}

void ResourceFSM::Update(double delta) {
    GAME_ONLY
    SETUP_INPUT

    if(current_state.is_valid()){
        current_state->Update(delta);
    }
}

void ResourceFSM::unhanlded_input_update(const Ref<InputEvent> &p_event) {
    GAME_ONLY
     if(current_state.is_valid()){
        current_state->unhandled_state_input(p_event);
    }
}

void ResourceFSM::_ready() {
    GAME_ONLY
    // Load all the weapon reosource names into the weapon_resource_list variable
    for(int i = 0; i < states.size(); i++){
        Ref<ResourceState> state = Ref<ResourceState>(Object::cast_to<ResourceState>(states[i]));
        if(state.is_valid()){
            states[i] = states[i].duplicate();
        }
    }
    
     if(states[0]){
        Ref<ResourceState> initial_state = Ref<ResourceState>(Object::cast_to<ResourceState>(states[0]));
        if(initial_state.is_valid()) {
             current_state = initial_state;
        }
    }
}

void ResourceFSM::change_state(Ref<ResourceState> new_state) {
	if (new_state == current_state) {
        UtilityFunctions::print("ERROR_CHANGE_STATE::weapon == current_weapon_resource");
        return;
    } if(new_state.is_null()){ 
        UtilityFunctions::print("ERROR_CHANGE_STATE::weapon.is_null())");
        // ERROR NEW STATE EMPTY
        return;
    } if(current_state.is_valid()){
        current_state->Exit();
    }
    new_state->Enter();
    current_state = new_state;
}

void ResourceFSM::_bind_methods() {
	// weapon_resource
    ClassDB::bind_method(D_METHOD("set_states", "p_states"), &ResourceFSM::set_states);
    ClassDB::bind_method(D_METHOD("get_states"), &ResourceFSM::get_states);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "States", PROPERTY_HINT_ARRAY_TYPE, "ResourceState"), "set_states", "get_states");

}