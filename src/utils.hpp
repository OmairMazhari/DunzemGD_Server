#pragma once
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace server {
    godot::Vector2 get_custom_vector(const godot::Dictionary &dict,
                                     const godot::String &left, const godot::String &right,
                                     const godot::String &up, const godot::String &down);
}
