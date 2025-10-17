#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include "godot_cpp/classes/ray_cast3d.hpp"
#include "godot_cpp/classes/input_event_mouse_motion.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/label.hpp"

#include "State.h"

using namespace godot;

class WeaponResource;

class ResourceFSM : public Node {
    GDCLASS(ResourceFSM, Node);
public:
    ResourceFSM();
    ~ResourceFSM();

    // Overriden functions
    void _process(double delta) override;
    void _ready() override;
    
    // Class functions
    void Setup(Ref<ResourceState> weapon);
    void change_state(Ref<ResourceState> new_weapon_resource);

private:
    // Export Variables
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