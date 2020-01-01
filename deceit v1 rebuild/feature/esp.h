#pragma once

#include "../sdk/class.h"
#include "../d3d_render/render.hpp"


namespace esp {
	void draw_esp(IEntity* entity,IActor*  actor);
	void draw_aabb(IEntity* entity);
	void draw_name(IEntity* entity,XMCOLOR color , const char* text,float fl_offset = 10.0f);
}