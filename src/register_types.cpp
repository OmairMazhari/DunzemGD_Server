#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "FPSController.h"
#include "FSM.h"
#include "State.h"
#include "PlayerMovementFSM.h"
#include "PlayerMovementState.h"
#include "PlayerMovementStatsResource.h"
#include "Move_PMState.h"
#include "NoClip_PMState.h"
#include "WeaponResource.h"
#include "PlayerWeaponManager.h"
#include "ViewCamera.h"
#include "GunWeaponResource.h"

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_RUNTIME_CLASS(FPSController);
	GDREGISTER_RUNTIME_CLASS(FSM);
	GDREGISTER_RUNTIME_CLASS(State);
	GDREGISTER_RUNTIME_CLASS(PlayerMovementFSM);
	GDREGISTER_RUNTIME_CLASS(PlayerMovementState);
	GDREGISTER_RUNTIME_CLASS(PlayerMovementStatsResource);
	GDREGISTER_RUNTIME_CLASS(Move_PMState);
	GDREGISTER_RUNTIME_CLASS(NoClip_PMState);
	GDREGISTER_RUNTIME_CLASS(WeaponResource);
	GDREGISTER_CLASS(PlayerWeaponManager);
	GDREGISTER_RUNTIME_CLASS(ViewCamera);
	GDREGISTER_RUNTIME_CLASS(GunWeaponResource);





}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT DunzemEngine_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}