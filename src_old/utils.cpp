#include "utils.hpp"
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
using namespace godot;

godot::Vector2 server::get_custom_vector(const Dictionary &dict,
                                         const String &left, const String &right,
                                         const String &up, const String &down) {
    bool left_pressed  = dict.has(left)  ? (bool)dict[left]  : false;
    bool right_pressed = dict.has(right) ? (bool)dict[right] : false;
    bool up_pressed    = dict.has(up)    ? (bool)dict[up]    : false;
    bool down_pressed  = dict.has(down)  ? (bool)dict[down]  : false;

    float x = (right_pressed ? 1.0f : 0.0f) - (left_pressed ? 1.0f : 0.0f);
    float y = (down_pressed ? 1.0f : 0.0f) - (up_pressed ? 1.0f : 0.0f);

    Vector2 v(x, y);
    if (v.length() > 1.0f) {
        v = v.normalized();
    }
    return v;
}
