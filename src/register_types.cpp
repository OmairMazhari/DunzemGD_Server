#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

// #include "FPSController.h"
// #include "FSM.h"
// #include "State.h"
// #include "PlayerMovementFSM.h"
// #include "PlayerMovementState.h"
// #include "PlayerMovementStatsResource.h"
// #include "Move_PMState.h"
// #include "NoClip_PMState.h"
// #include "WeaponResource.h"
// #include "PlayerWeaponManager.h"
// #include "ViewCamera.h"
// #include "GunWeaponResource.h"

#include "Player/FPSController.h"
#include "FSM/NodeFSM/FSM.h"
#include "FSM/NodeFSM/State.h"
#include "Player/Movement/StateMachine/PlayerMovementFSM.h"
#include "Player/Movement/States/PlayerMovementState.h"
#include "Player/Movement/PlayerMovementStatsResource.h"
#include "Player/Movement/States/Move_PMState.h"
#include "Player/Movement/States/NoClip_PMState.h"
#include "Player/Weapon/WeaponResource.h"
#include "Player/Weapon/PlayerWeaponManager.h"
#include "Player/Weapon/ViewCamera.h"
#include "Player/Weapon/GunWeaponResource.h"
#include "FSM/ResourceFSM/FSM.h"

#include "Server/Server.h"



using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_RUNTIME_CLASS(Server);

	GDREGISTER_RUNTIME_CLASS(FPSController);
	GDREGISTER_RUNTIME_CLASS(FSM);
	GDREGISTER_RUNTIME_CLASS(State);

	GDREGISTER_RUNTIME_CLASS(ResourceFSM);
	GDREGISTER_RUNTIME_CLASS(ResourceState);
	
	GDREGISTER_RUNTIME_CLASS(PlayerMovementFSM);
	GDREGISTER_RUNTIME_CLASS(PlayerMovementState);

	GDREGISTER_RUNTIME_CLASS(PlayerMovementStatsResource);
	GDREGISTER_RUNTIME_CLASS(Move_PMState);
	GDREGISTER_RUNTIME_CLASS(NoClip_PMState);
	GDREGISTER_RUNTIME_CLASS(WeaponResource);
	GDREGISTER_RUNTIME_CLASS(PlayerWeaponManager);
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