#include "FSM.h"


class MovementFSM : public FSM  {
    GDCLASS(MovementFSM, FSM);
public: 
    void _process(double delta) override;
    MovementFSM();
    ~MovementFSM();
protected: 
    static void _bind_methods();
};