#pragma once
#include "godot_cpp/classes/engine.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>

// Run in editor only
#define GAME_ONLY if(Engine::get_singleton()->is_editor_hint()) { return; }

// Custom data to make exporting values easier
#define SETUP_INPUT Input* input = Input::get_singleton();

