#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/signal.hpp>
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class ResourceState : public Resource {
    GDCLASS(ResourceState, Resource);   
public:
    ResourceState(){ }
    ~ResourceState(){ }

    // Virtual Member Functions
    virtual void Enter() {}
    virtual void Exit() {}
    virtual void Update(double delta) {}
    virtual void Setup() {}

protected:
    static void _bind_methods(){
        // Signals
        ADD_SIGNAL(MethodInfo("state_transition", PropertyInfo(Variant::OBJECT , "old_state"),PropertyInfo(Variant::STRING, "new_state_name")));
    }
};