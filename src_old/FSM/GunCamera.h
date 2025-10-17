#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

class GunCamera : public Camera3D {
    GDCLASS(GunCamera, Camera3D);

protected:
    static void _bind_methods(){
        
    ClassDB::bind_method(D_METHOD("set_camera", "camera"), &GunCamera::set_camera);
    ClassDB::bind_method(D_METHOD("get_camera"), &GunCamera::get_camera);

    ADD_PROPERTY(
        PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NODE_TYPE, "Camera3D"),
        "set_camera",
        "get_camera"
    );
    }

public:
    Camera3D* camera = nullptr;
    Camera3D* get_camera() const {return camera;}
    void set_camera(Node* p_val) { camera = Object::cast_to<Camera3D>(p_val); }

    void _process(double delta) override {
        if(Engine::get_singleton()->is_editor_hint()){
            return;
        }
        set_global_transform(camera->get_global_transform());
        
    }
};
