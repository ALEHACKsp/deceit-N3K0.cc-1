#pragma once

#include "..//sdk/class.h"

#define tp_key (GetAsyncKeyState('V') != 0 && GetAsyncKeyState(VK_MENU) != 0)

enum sort_type : int {
	distance,
	fov
};

namespace misc {
	void local_to_player(IEntity* entity,bool save_old_pos = true);
	extern int sort_mode;
	extern bool reset_distance;

	void dump_all_cvar();
}