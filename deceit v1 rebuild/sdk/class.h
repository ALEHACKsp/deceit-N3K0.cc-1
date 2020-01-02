#pragma once

#include "math/math.h"
#include "UtlVector.h"
#include "..//utils/vfunction.h"
#include "..//feature/global_var.h"
	
typedef __int64 EntityId;

#include <vector>
#include <map>
#include <memory>

#pragma region entity_class
class ICharacterInstance;
class IEntity;
class IInventory;
class IEntityClass;
class IItem;
struct IEntityIt;
struct KillParams;
class IActor {
public:

	char _0x0004[4];
	IEntity* LinkedEntity; //0x0008 
	DWORD GameObject; //0x000C 
	DWORD EntityID; //0x0010 
	IInventory* get_inventory() {
		return CallVFunction< IInventory * (__thiscall*)(void*)>(this, 68)(this);
	}
	IItem* get_current_item(bool includeVehicle = false) {
		return CallVFunction < IItem * (__thiscall*)(void*, bool)>(this, 66)(this, includeVehicle);
	}
	bool is_player() {
		return CallVFunction<bool(__thiscall*)(void*)>(this, 81)(this);
	}
	void toggle_thirdperson() {
		return CallVFunction<void(__thiscall*)(void*)>(this, 75)(this);
	}
	bool is_thirdperson() {
		return CallVFunction<bool(__thiscall*)(void*)>(this, 74)(this);
	}

	void simle_net_kill() {
		return CallVFunction<void(__thiscall*)(void*)>(this, 180)(this);
	}
	void NetKill(KillParams* param) {
		CallVFunction<void(__thiscall*)(void*, KillParams*)>(this, 124)(this, param);
	}
	float get_health() {
		return CallVFunction<float(__thiscall*)(void*)>(this, 35)(this);
	}
	bool is_dead() {
		return *(BYTE*)(this + 0x1171);
	}

};

struct SWeaponAmmo
{
	IEntityClass* pAmmoClass;
	int						count;
};
typedef SWeaponAmmo** TAmmoVector;

//struct SWeaponAmmoUtils
//{
//	static const SWeaponAmmo* FindAmmoConst(const TAmmoVector& ammoVector, IEntityClass* pAmmoType)
//	{
//		const int ammoCount = ammoVector.Count();
//		for (int i = 0; i < ammoCount; ++i)
//		{
//			if (pAmmoType == ammoVector[i].pAmmoClass)
//			{
//				return &(ammoVector[i]);
//			}
//		}
//
//		return NULL;
//	}
//
//	static SWeaponAmmo* FindAmmo(TAmmoVector& ammoVector, IEntityClass* pAmmoType)
//	{
//	const int ammoCount = ammoVector.Count();
//	for (int i = 0; i < ammoCount; ++i)
//	{
//		if (pAmmoType == ammoVector[i].pAmmoClass)
//		{
//			return &(ammoVector[i]);
//		}
//	}
//
//	return NULL;
//	}
//
//	// returns true if the the ammo type was already found in the ammoVector
//	static bool SetAmmo(TAmmoVector& ammoVector, IEntityClass* pClass, int count)
//	{
//		SWeaponAmmo* pAmmo = SWeaponAmmoUtils::FindAmmo(ammoVector, pClass);
//		if (pAmmo != NULL)
//		{
//			pAmmo->count = count;
//			return true;
//		}
//		return false;
//
//	}
//
//	static int GetAmmoCount(const TAmmoVector& ammoVector, IEntityClass* pClass)
//	{
//		const SWeaponAmmo* pAmmo = SWeaponAmmoUtils::FindAmmoConst(ammoVector, pClass);
//
//		return pAmmo ? pAmmo->count : 0;
//	}
//};

class Vector2 {
public:
	Vector2() {
		x = 0;
		y = 0;
	}
	Vector2(float x1, float y1) {
		x = x1;
		y = y1;
	}
	float x, y;
};

struct SSpreadModParams
{

	float min_mod;
	float max_mod;
	float attack_mod;
	float decay_mod;
	float end_decay_mod;
	float speed_m_mod;
	float speed_holdBreathActive_m_mod;
	float rotation_m_mod;

	//Stance modifiers
	float spread_crouch_m_mod;
	float spread_jump_m_mod;
	float spread_slide_m_mod;
	float spread_holdBreathActive_m_mod;
};

struct SRecoilModParams
{
	float max_recoil_mod;
	float attack_mod;
	float first_attack_mod;
	float decay_mod;
	float end_decay_mod;
	Vector2 max_mod;
	float impulse_mod;

	//Stance modifiers
	float recoil_crouch_m_mod;
	float recoil_jump_m_mod;

	float recoil_holdBreathActive_m_mod;
};

typedef DWORD64 _QWORD;
typedef signed __int16 _WORD ;

struct IFireMode {

	float __fastcall GetFireRate()
	{
		return (float) * (signed __int16*)(*(DWORD64*)(*(DWORD64*)(this + 0x10) + 8) + 0x90);
	}
	void set_fire_rate(signed __int16 r) {
		*(_WORD*)(*(_QWORD*)(*(_QWORD*)(this + 0x10) + 8) + 0x92) = r;
	}

	void set_recoil_param( SRecoilModParams* sRMP, float modMultiplier) {
		CallVFunction < void(__thiscall*)(void*, SRecoilModParams*, float)>(this, 64)(this,sRMP, modMultiplier);
	}
	void set_spread_param( SSpreadModParams* sSMP, float modMultiplier) {
		CallVFunction < void(__thiscall*)(void*, SSpreadModParams*, float)>(this, 62)(this,sSMP, modMultiplier);
	}
	void Fire() {
		// 27
		CallVFunction<void(__thiscall*)(void*)>(this, 27)(this);
	}
	void StopFire() {
		// 28
		CallVFunction<void(__thiscall*)(void*)>(this, 28)(this);
	}
	bool IsFire() {
		// 29
		return CallVFunction<bool(__thiscall*)(void*)>(this, 29)(this);
	}
	void net_shoot(Vector* bullshit, int bullshit2) {
		CallVFunction<void(__thiscall*)(void*, Vector*, int)>(this,37)(this, bullshit, bullshit2);
	}
	char pad_0x0000[0x90]; //0x0000
	float N000000FA; //0x0090 
	char pad_0x0094[0x14]; //0x0094
	float N000000FD; //0x00A8 
	char pad_0x00AC[0x74]; //0x00AC
	float fl_MinSpread; //0x0120 
	float fl_MaxSpread; //0x0124 
}; //Size=0x0880
class IWeapon {
public:
	__int32 get_ammo_count(IEntityClass * type) {
		return CallVFunction<__int32(__thiscall*)(void*,IEntityClass*)>(this, 25) (this, type);
	}
	void set_ammo_count(IEntityClass* type, __int32 count) {
		CallVFunction<__int32(__thiscall*)(void*, IEntityClass*,__int32)>(this, 26) (this, type, count);
	}
	char pad_0x0000[0x38]; //0x0000
	IFireMode* m_fm; //0x0038 
	char pad_0x0040[0xA0]; //0x0040
	SWeaponAmmo* m_ammo; //0x00E0 
	SWeaponAmmo* m_bonusammo; //0x00E8 
	unsigned char m_fire_alternation; //0x00F0 
	unsigned char m_restartZoom; //0x00F1 
};
class IItem {
public:
	IWeapon* GetIWeapon() {
		return CallVFunction< IWeapon * (__thiscall*)(void*)>(this, 85)(this);
	}
};

#define USE_LAGOMETER 1

struct KillParams {
	EntityId shooterId;
	EntityId targetId;
	EntityId weaponId;
	EntityId projectileId;
	EntityId itemIdToDrop;
	int weaponClassId;
	float damage;
	float impulseScale;
	Vector dir;
	int material;
	int hit_type;
	int targetTeam;
	unsigned __int16 hit_joint;
	unsigned __int16 projectileClassId;
	unsigned __int8 penetration;
	unsigned __int8 lagOMeterHitId;
	bool ragdoll;
	bool winningKill;
	bool firstKill;
	bool bulletTimeReplay;
	bool killViaProxy;
	bool forceLocalKill; // Not serialised. skips prohibit death reaction checks. 
	// Special case - used when actor is killed from FullSerialize to supress certain logic from running
	bool fromSerialize;
};
class IEntity
{
public:
	ICharacterInstance* get_character(DWORD64 id) {
		typedef ICharacterInstance* (__thiscall * OriginalFunc)(void* self, DWORD64);
		return CallVFunction<OriginalFunc>(this, 110)(this, id);
	}
	Matrix34& get_world_tm() {
		typedef Matrix34& (__thiscall * OriginalFunc)(void* self);
		return CallVFunction<OriginalFunc>(this, 29)(this);
	}
	void GetWorldBounds(AABB* aabb) {
		CallVFunction<void(__thiscall*)(void*, AABB*)>(this, 31)(this, aabb);
	}
	void get_local_bounds(AABB* aabb) {
		CallVFunction<void(__thiscall*)(void*, AABB*)>(this, 32)(this, aabb);
	}
	void* get_entity_render() {
		return CallVFunction<void*(__thiscall*)(void*)>(this, 40)(this);
	}

	const char* get_name() {
		return CallVFunction<const char* (__thiscall*)(void*)>(this, 14)(this);
	}
	void set_name(const char* name) {
		CallVFunction<void(__thiscall*)(void*,const char*)>(this, 13)(this, name);
	}
	void set_invisible(bool b_invisible) {
		 CallVFunction<void(__thiscall*)(void*,bool)>(this, 56)(this, b_invisible);
	}
	bool is_invisible() {
		return CallVFunction<bool(__thiscall*)(void*)>(this, 57)(this);
	}

	EntityId get_id() {
		return CallVFunction<EntityId (__thiscall*)(void*)>(this, 1)(this);
	}
	IEntityClass* get_class() {
		return CallVFunction<IEntityClass*(__thiscall*)(void*)>(this, 3)(this);
	}

	Ang3 get_angle() {
		return Ang3(quat_to_matrix33(m_rotation));
	}
	void set_local_position(Vector vPos) {
		CallVFunction<void(__thiscall*)(void*, Vector*)>(this, 35)(this, &vPos);
	}
	Vector get_bone_position(__int64 id);
	Vector get_bone_position(const char* name = "Bip01 Head");
	void set_position(Vector pos);

	char pad_0x0000[0x8]; //0x0000
	char* Name; //0x0008 
	char pad_0x0010[0x30]; //0x0010
	Vector m_position; //0x0040 
	Quat m_rotation; //0x004C 
	Vector m_scale; //0x005C 
	Matrix34 m_worldTM; //0x0068 

}; //Size=0x004C
class ISkeletonPose {
public:
	QuatT& GetAbsJointByID(__int64 id) {
		typedef QuatT& (__thiscall * OriginalFunc)(void* self, __int64);
		return CallVFunction<OriginalFunc>(this, 24)(this, id);
	}
};
class IDefaultSkeleton {
public:
	__int64 GetJointIDByName(const char* name) {
		typedef __int64(__thiscall * get_join_)(void*, const char*);
		return CallVFunction<get_join_>(this, 9)(this, name);
	}
	QuatT& GetDefaultAbsJointByID(__int64 nJointIdx) {
		typedef QuatT& (__thiscall * get_abs_)(void*, __int64);
		return CallVFunction<get_abs_>(this, 11)(this, nJointIdx);
	}
};
class ICharacterInstance {
public:
	ISkeletonPose* GetISkeletonPose() {
		typedef ISkeletonPose* (__thiscall * OriginalFunc)(void* self);
		return CallVFunction<OriginalFunc>(this, 17)(this);
	}
	IDefaultSkeleton* GetIDefaultSkeleton() {
		typedef IDefaultSkeleton* (__thiscall * get_default_)(void*);
		return CallVFunction<get_default_>(this, 21)(this);
	}
};
class IActorIterator
{
public:
	virtual void Function0(); 
	virtual void Function1(); 
	virtual IActor* Next(); 
	virtual void IncreaseIndex(); 
	virtual int Count(); 

};//Size=0x0004
class IActorSystem
{
public:
	IActor* GetActor(EntityId id)
	{
		typedef IActor* (__thiscall * OriginalFunc)(void* self, EntityId id);
		return CallVFunction<OriginalFunc>(this, 3)(this, id);
	}
	int GetActorCount()
	{
		typedef int(__thiscall * OriginalFunc)(void* self);
		return CallVFunction<OriginalFunc>(this, 6)(this);
	}
	void create_actor_interator(IActorIterator** ptr) {
		CallVFunction<void(__thiscall*)(void*, IActorIterator * *)>(this, 7)(this, ptr);
	}
};
class IGameFramework {
public:
	IActor* get_client_actor() {
		return CallVFunction<IActor * (__thiscall*)(void*)>(this, 64)(this);
	}
	IEntity* get_client_entity() {
		return CallVFunction<IEntity * (__thiscall*)(void*)>(this, 66)(this);
	}
	IActorSystem* get_actor_system() {
		return CallVFunction<IActorSystem * (__thiscall*)(void*)>(this, 22)(this);
	}

};
class IInventory {
public:
	void SetAmmoCount(IEntityClass* c_class, __int64 amount) {
		CallVFunction<void(__thiscall*)(void*, IEntityClass*, __int64)>(this, 74)(this, c_class, amount);
	}
};
class IEntityClassRegistry {
public:
	IEntityClass* FindClass(const char* name) {
		return CallVFunction< IEntityClass * (__thiscall*)(void*, const char*)>(this, 3)(this, name);
	}
};
class IEntitySystem {
public:
	IEntityClassRegistry* GetClassRegistry() {
		return CallVFunction< IEntityClassRegistry * (__thiscall*)(void*)>(this, 8)(this);
	}
	IEntityIt* GetEntityIterator() {
		return CallVFunction< IEntityIt * (__thiscall*)(void*)>(this, 17)(this);
	}
};
class IEntityIt
{
public:
	// <interfuscator:shuffle>
	virtual ~IEntityIt() {}

	virtual void AddRef() = 0;

	//! Deletes this iterator and frees any memory it might have allocated.
	virtual void Release() = 0;

	//! Checks whether current iterator position is the end position.
	//! \return true if iterator at end position.
	virtual bool IsEnd() = 0;

	//! Retrieves next entity.
	//! \return Pointer to the entity that the iterator points to before it goes to the next.
	virtual IEntity* Next() = 0;

	//! Retrieves current entity.
	//! \return Entity that the iterator points to.
	virtual IEntity* This() = 0;

	//! Positions the iterator at the beginning of the entity list.
	virtual void MoveFirst() = 0;
	// </interfuscator:shuffle>
};
class IEntityClass {
public:
	const char* get_class_name() {
		return CallVFunction<const char* (__thiscall*)(void*)>(this, 2)(this);
	}
};
enum EVarFlags : uint32_t
{
	VF_NULL = 0x00000000,      //!< Just to have one recognizable spot where the flags are located in the Register call.
	VF_CHEAT = 0x00000002,      //!< Stays in the default state when cheats are disabled.
	VF_DEV_ONLY = 0x00000004,      //!< Cvar is only registered with the console in non release builds.
	VF_DEDI_ONLY = 0x00000008,      //!< Cvar is only registered with the console in non release or dedicated server builds.
	VF_NET_SYNCED = 0x00000080,      //!< Synchronised between server and client(s); server is authoritative.
	VF_DUMPTODISK = 0x00000100,
	VF_READONLY = 0x00000800,      //!< Can not be changed by the user.
	VF_REQUIRE_LEVEL_RELOAD = 0x00001000,
	VF_REQUIRE_APP_RESTART = 0x00002000,
	VF_WARNING_NOTUSED = 0x00004000,      //!< Shows warning that this var was not used in config file.
	VF_COPYNAME = 0x00008000,      //!< (Deprecated) Otherwise the const char * to the name will be stored without copying the memory.
	VF_MODIFIED = 0x00010000,      //!< Set when variable value modified.
	VF_WASINCONFIG = 0x00020000,      //!< Set when variable was present in config file.
	VF_BITFIELD = 0x00040000,      //!< Allow bitfield setting syntax.
	VF_RESTRICTEDMODE = 0x00080000,      //!< Is visible and usable in restricted (normal user) console mode.
	VF_INVISIBLE = 0x00100000,      //!< Invisible to the user in console.
	VF_ALWAYSONCHANGE = 0x00200000,      //!< Always accept variable value and call on change callback even if variable value didnt change.
	VF_BLOCKFRAME = 0x00400000,      //!< Blocks the execution of console commands for one frame.
	VF_CONST_CVAR = 0x00800000,      //!< Set if it is a const cvar not to be set inside cfg-files.
	VF_CHEAT_ALWAYS_CHECK = 0x01000000,      //!< This variable is critical to check in every hash, since it's extremely vulnerable.
	VF_CHEAT_NOCHECK = 0x02000000,      //!< This variable is set as VF_CHEAT but doesn't have to be checked/hashed since it's harmless to hack.
	VF_SYSSPEC_OVERWRITE = 0x04000000,      //!< This variable is specified by system.cfg with the intention to overwrite all subsequent settings.
	VF_CVARGRP_IGNOREINREALVAL = 0x08000000,      //!< This variable will be ignored when cvar group's real val is checked (Needed for cvars which are in a group but change in various situations).
	VF_LIVE_CREATE_SYNCED = 0x10000000,      //!< This variable will be synced with LiveCreate clients.
	VF_RENDERER_CVAR = 0x20000000,      //!< The update of this variable will be done in render thread.
	VF_DEPRECATED = 0x40000000,      //!< Deprecated cvars use default values which cannot be modified outside the code.
	VF_EXPERIMENTAL = 0x80000000,      //!< This variable is used by WIP or experimental feature.
};
enum class ECVarType : signed __int64
{
	Invalid = 0,
	Int = 1,
	Float = 2,
	String = 3,
	Int64 = 4
};

class ICVar {
public:
	template<typename type>
	type get_val() {
		return **(type**)(this + 0x48);
	}
	template<typename type>
	void set_val(type value) {
		**(type* *)(this + 0x48) = value;
	}
	void* value_ptr() {
		return *(void**)(this + 0x48);
	}
	ECVarType get_type() {
		return CallVFunction< ECVarType(__thiscall*)(void*)>(this,14)(this);
	}
	void* vftable;
	char* str_name; //0x0008 
	char* str_desciption; //0x0010 
};
class IConsole {
public:
	ICVar* get_cvar(const char* name) {
		return CallVFunction<ICVar * (__thiscall*)(void*, const char*)>(this, 23)(this,name);
	}
	void show_console(bool show, int iRequestScrollMax = -1) {
		CallVFunction<void(__thiscall*)(void*, bool, int)>(this,14)(this, show, iRequestScrollMax);
	}
	void dump_cvars(void* callback,  int var_flag = 0) {
		CallVFunction<void(__thiscall*)(void*, void*, int)>(this, 15)(this, callback, var_flag);
	}
};
#pragma endregion



#pragma region render class
class IRender { 
public:

	bool ProjectToScreen(float ptx, float pty, float ptz, float* sx, float* sy, float* sz)
	{
		static DWORD64 game_base = (DWORD64)(GetModuleHandleA(0));
		typedef bool(__thiscall * oFunc)(void*, float, float, float, float*, float*, float*);
		bool ret = CallVFunction< oFunc >(this, 86)(this, ptx, pty, ptz, sx, sy, sz);
		*sx *= ((float)window_width() / 100.0f);
		*sy *= ((float)window_height() / 100.0f);
		*sz *= 1.0f;

		return (*sz < 1.0f);
	}

	 bool ProjectToScreen(Vector in, Vector* out) {
		float x, y, z;
		if (!ProjectToScreen(in.x, in.y, in.z, &x, &y, &z))
			return false;
		out->x = x;
		out->y = y;
		out->z = z;
		return true;
	}
	 int window_width() {
		 return *(int*)(uintptr_t(this) + 0xAC30);
	 }
	 int window_height() {
		 return *(int*)(uintptr_t(this) + 0xAC34);
	 }
	 int render_width() {
		 return *(int*)(uintptr_t(this) + 0xA9F0);
	 }
	 int render_height() {
		 return *(int*)(uintptr_t(this) + 0xA9F4);
	 }
	 bool is_full_screen() {
		 return *(bool*)(uintptr_t(this) + 0xB5A8);
	 }
	 char* title_name() {
		 return (char*)(uintptr_t(this) + 0xC8E1);
	 }
	 HANDLE window_handle() {
		 return *(HANDLE*)(uintptr_t(this) + 0xC938);
	 }

};
#pragma endregion

#pragma region Physical classes
struct SCollisionClass
{
	uint32_t type;     //!< Collision_class flags to identify the entity.
	uint32_t ignore;   //!< Another entity will be ignored if *any* of these bits are set in its type.

	SCollisionClass() {}

	SCollisionClass(uint32_t t, uint32_t i)
	{
		type = t;
		ignore = i;
	}
};

struct IPhysicalEntity;
struct ray_hit
{
	float            dist;
	IPhysicalEntity* pCollider;
	int              ipart;
	int              partid;
	short            surface_idx;
	short            idmatOrg; //!< original material index, not mapped with material mapping
	int              foreignIdx;
	int              iNode; //!< BV tree node that had the intersection; can be used for "warm start" next time
	Vector             pt;
	Vector             n;        //!< surface normal
	int              bTerrain; //!< global terrain hit
	int              iPrim;    //!< hit triangle index
	ray_hit* next;     //!< reserved for internal use, do not change
};
struct ray_hit_cached   //!< used in conjunction with rwi_reuse_last_hit
{
	ray_hit_cached() { pCollider = 0; ipart = 0; }
	ray_hit_cached(const ray_hit& hit) { pCollider = hit.pCollider; ipart = hit.ipart; iNode = hit.iNode; }
	ray_hit_cached& operator=(const ray_hit& hit) { pCollider = hit.pCollider; ipart = hit.ipart; iNode = hit.iNode; return *this; }

	IPhysicalEntity* pCollider;
	int              ipart;
	int              iNode;
};

class SRWIParams
{
public:
	SRWIParams() {
		memset(this, 0, sizeof(*this));
	}
	void* pForeignData;
	int               iForeignData;
	int               (*OnEvent)(const void*);
	Vector              org;
	Vector              dir;
	int               objtypes;
	unsigned int      flags;
	ray_hit* hits;
	int               nMaxHits;
	ray_hit_cached* phitLast;
	int               nSkipEnts;
	IPhysicalEntity** pSkipEnts;
	SCollisionClass   collclass;
	IPhysicalEntity** pPortals;	   // returns null-terminated list of encountered portals
	int               nMaxPortals; //
}; //Size=0x0040

#define RWI_NAME_TAG "RayWorldIntersection(Game)"
#define MAX_PHYS_THREADS 4



class IPhysicalWorld {
public:

	int RayWorldIntersection(SRWIParams* rp, const char* pNameTag, int iCaller) {
		typedef int(__thiscall * ray_function)(void*, SRWIParams*, const char*, int);
		return CallVFunction<ray_function>(this, 35)(this, rp, pNameTag, iCaller);
	}

	inline int RayWorldIntersection(const Vector& org, const Vector& dir, int objtypes, unsigned int flags, ray_hit* hits, int nMaxHits,
		IPhysicalEntity** pSkipEnts = 0, int nSkipEnts = 0, void* pForeignData = 0, int iForeignData = 0,
		const char* pNameTag = RWI_NAME_TAG, ray_hit_cached * phitLast = 0, int iCaller = MAX_PHYS_THREADS)
	{
		SRWIParams rp = SRWIParams();
		rp.org = org;
		rp.dir = dir;
		rp.objtypes = objtypes;
		rp.flags = flags;
		rp.hits = hits;
		rp.nMaxHits = nMaxHits;
		rp.pForeignData = pForeignData;
		rp.iForeignData = iForeignData;
		rp.phitLast = phitLast;
		rp.pSkipEnts = pSkipEnts;
		rp.nSkipEnts = nSkipEnts;
		return RayWorldIntersection(&rp, pNameTag, iCaller);
	}

};
#pragma endregion



namespace sdk {
	namespace global_var {
		extern IRender* pRender;					// render class (world to screen)
		extern IPhysicalWorld* pPhysicalWorld;		// physical world class (traceray sutff)
		extern IGameFramework* pGameFramework;		// pgameframework class (get_local_ent , create_entity_system)
		extern IEntitySystem* pEntitySystem;		//pEntity class (some thing i can't remem lol)
		extern  IConsole* pConsole;					//Find Cvar stuff
		extern void* pLog; // i'm fucking lazy soorry :(
	}
	bool Initialize(bool dump = true);

}

#define crylog_function_address 0xE9B10
/*
pattern : E8 ? ? ? ? 48 8B 9F ? ? ? ? 8B 74 24 28  + 1
or see how to find pLog
*/

template<typename... Args> // look similar right ? Thx to CSGOSimple :D
void CryLog(const char* format, Args...args) {
	static auto import = (void(*)(const char*, ...))(((DWORD64)GetModuleHandleA(0) + crylog_function_address));
	return import(format, args...);
}
