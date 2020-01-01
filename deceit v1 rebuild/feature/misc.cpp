#include "misc.h"

int misc::sort_mode = sort_type::fov;
bool misc::reset_distance = false;
void misc::local_to_player(IEntity* entity, bool save_old_pos) { 

	static Vector old_pos = Vector();
	static bool should_restore = false;
	IEntity* localplayer = sdk::global_var::pGameFramework->get_client_entity();

	if (localplayer == 0)
		return;

	if (tp_key) {
		should_restore = true;
		localplayer->set_position(entity->m_position);
	}
	else {
		if (should_restore) {
			while (localplayer->m_position != old_pos)
				localplayer->set_position(old_pos);
			should_restore = false;
		}

		if (!global_var::disable_save_pos) {
			old_pos = localplayer->m_position;
		}

	}
}

struct ICVarDumpSink {
	virtual ~ICVarDumpSink() ;
	virtual void OnElement(ICVar* var) ;
};
ICVarDumpSink::~ICVarDumpSink() {};
void ICVarDumpSink::OnElement(ICVar* var) {
	if (var == 0)
		return;
	printf("[CVar Dump] | <%p> %s | \n",
		var,
		var->str_name);

	/*switch (var->get_type())
	{
	case ECVarType::Invalid:
		break;
	case ECVarType::Int:
		printf("%d  \n", var->get_val<__int32>());
		break;
	case ECVarType::Float:
		printf("%.2f \n", var->get_val<float>());
		break;
	case ECVarType::Int64:
		printf("%lld \n", var->get_val<__int64>());
		break;

	case ECVarType::String:
		printf("%s  \n", var->get_val<const char*>());
		break;
	default:
		break;
	}*/
}
void misc::dump_all_cvar() {
	ICVarDumpSink* c_var_dump = new ICVarDumpSink();
	sdk::global_var::pConsole->dump_cvars((void*)c_var_dump);
	//c_var_dump.release();
}