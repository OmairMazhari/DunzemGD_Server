#pragma once

#include "State.h"
#include "./Player.h"
class StateSlide :public State {
    GDCLASS(StateSlide, State);
    StateSlide(){

    }
    ~StateSlide(){

    }
public:
    void Enter() override {
        UtilityFunctions::print("Running in Slide state");
    }
    void Exit() override  {
        UtilityFunctions::print("Exiting Slide state");    
    }
    void Update(double delta, Player* player) override {
        UtilityFunctions::print("Updating in Slide state");
    }

protected:
    static void _bind_methods(){
        
    }
};