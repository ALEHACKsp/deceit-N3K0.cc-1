#include "esp.h"

#include "../d3d_render/render.hpp"

using namespace sdk::global_var;

void esp::draw_esp(IEntity* ent, IActor* actor) {
	if (!ent)
		return;
	bool is_player = (actor);
	Vector player_head = ent->m_position;
	Vector w2s = Vector();

	if (pRender->ProjectToScreen(player_head, &w2s)) {
		if (is_player) {
			renderer->drawText(Vec2(w2s.x, w2s.y), XMCOLOR(0.0f, 1.0f, 0.0f, 1.0f), FW1_LEFT, 15.0f, "%s", ent->get_name());
			renderer->drawText(Vec2(w2s.x, w2s.y + 10.0f), std::string(std::to_string(actor->get_health())), XMCOLOR(0.0f, 1.0f, 1.0f, 1.0f), FW1_LEFT, 15.0f);
		}
		else 
			if (ent->get_class())
				renderer->drawText(Vec2(w2s.x, w2s.y + 10.0f), std::string(ent->get_class()->get_class_name()), XMCOLOR(1.0f, 1.0f, 0, 1.0f), FW1_LEFT, 15.0f);
		
	}

}

void esp::draw_name(IEntity* entity, XMCOLOR color, const char* text, float fl_offset) {
	if (!entity)
		return;

	Vector player_head = entity->m_position;
	Vector w2s = Vector();

	if (pRender->ProjectToScreen(player_head, &w2s))
		renderer->drawText(Vec2(w2s.x, w2s.y + fl_offset), color, FW1_LEFT, 15.0f, "%s", text);
	
}

void esp::draw_aabb(IEntity* entity) {
	Vector Min, Max, vVec1, vVec2, vVec3, vVec4, vVec5, vVec6, vVec7, vVec8;
	AABB bBox;
	entity->get_local_bounds(&bBox);
	Max = bBox.max;
	Min = bBox.min;
	int width = 2;
	vVec3 = Min;
	vVec3.x = Max.x;
	vVec4 = Min;
	vVec4.y = Max.y;
	vVec5 = Min;
	vVec5.z = Max.z;
	vVec6 = Max;
	vVec6.x = Min.x;
	vVec7 = Max;
	vVec7.y = Min.y;
	vVec8 = Max;
	vVec8.z = Min.z;

	if (!pRender->ProjectToScreen(Min, &vVec1))
		return;
	if (!pRender->ProjectToScreen(Max, &vVec2))
		return;
	if (!pRender->ProjectToScreen(vVec3, &vVec3))
		return;
	if (!pRender->ProjectToScreen(vVec4, &vVec4))
		return;
	if (!pRender->ProjectToScreen(vVec5, &vVec5))
		return;
	if (!pRender->ProjectToScreen(vVec6, &vVec6))
		return;
	if (!pRender->ProjectToScreen(vVec7, &vVec7))
		return;
	if (!pRender->ProjectToScreen(vVec8, &vVec8))
		return;

	renderer->drawLine(Vec2(vVec1.x, vVec1.y), Vec2(vVec5.x, vVec5.y), XMCOLOR(0xffffff));
	renderer->drawLine(Vec2(vVec2.x, vVec2.y), Vec2(vVec8.x, vVec8.y), XMCOLOR(0xffffff));
	renderer->drawLine(Vec2(vVec3.x, vVec3.y), Vec2(vVec7.x, vVec7.y), XMCOLOR(0xffffff));
	renderer->drawLine(Vec2(vVec4.x, vVec4.y), Vec2(vVec6.x, vVec6.y), XMCOLOR(0xffffff));
	renderer->drawLine(Vec2(vVec1.x, vVec1.y), Vec2(vVec3.x, vVec3.y), XMCOLOR(0xffffff));
	renderer->drawLine(Vec2(vVec1.x, vVec1.y), Vec2(vVec4.x, vVec4.y), XMCOLOR(0xffffff));
	renderer->drawLine(Vec2(vVec8.x, vVec8.y), Vec2(vVec3.x, vVec3.y), XMCOLOR(0xffffff));
	renderer->drawLine(Vec2(vVec8.x, vVec8.y), Vec2(vVec4.x, vVec4.y), XMCOLOR(0xffffff));
	renderer->drawLine(Vec2(vVec2.x, vVec2.y), Vec2(vVec6.x, vVec6.y), XMCOLOR(0xffffff));
	renderer->drawLine(Vec2(vVec2.x, vVec2.y), Vec2(vVec7.x, vVec7.y), XMCOLOR(0xffffff));
	renderer->drawLine(Vec2(vVec5.x, vVec5.y), Vec2(vVec6.x, vVec6.y), XMCOLOR(0xffffff));
	renderer->drawLine(Vec2(vVec5.x, vVec5.y), Vec2(vVec7.x, vVec7.y), XMCOLOR(0xffffff));

}