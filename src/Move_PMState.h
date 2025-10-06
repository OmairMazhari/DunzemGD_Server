#pragma once
#include <PlayerMovementState.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

class Move_PMState : public PlayerMovementState {
    GDCLASS(Move_PMState, PlayerMovementState);
public:
    Move_PMState() {}
    ~Move_PMState() {};

    // Overriden functions
    void Enter() override;
    void Exit() override;
    void Update(double delta) override;


    // Class functions
    void _handle_air_physics(double delta);
    void _handle_ground_physics(double delta);

protected:
    static void _bind_methods();

private:
   
};