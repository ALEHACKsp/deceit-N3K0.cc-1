#include "class.h"


 IRender* sdk::global_var::pRender = 0;
 IPhysicalWorld* sdk::global_var::pPhysicalWorld = 0;
 IGameFramework* sdk::global_var::pGameFramework = 0;
 IEntitySystem* sdk::global_var::pEntitySystem = 0;
 IConsole* sdk::global_var::pConsole = 0;
 void* sdk::global_var::pLog = 0;


 /*
 
how to find offset 101
if you can't understand any of these for what ever reason like my english is to much retarded please remove all comments below offset or you will puke
take a look at this thread it really helpfull
https://unknowncheats.me/forum/warface/208747-reversing-warface-ssystemglobalenvironment-offsets-positions.html

1. open deceit.exe with ida pro this game doesn't encrypt anything you can just open it normaly
2. dowload cryengine-sdk https://github.com/CRYTEK/CRYENGINE
3. open notepad++ (or what every you like to search text in every file in directory)

If you're using notepad++ 
3.5 goto to search it top left menu -> find in file then set directory to ...\CRYENGINE-release\Code

4. search variable name like pGameFramework pEntitySystem or function name like RayWorldIntersection etc
5. the easiest to find these stuff (offset vtable index) is xref like how to find pGameFramework_offset
   (it really hard to explain when your ennglish skillz is below than your nonprime trust factor)
note
- Every offset you see in ida you need to subtract with base_adress (Deceit.exe is 140000000) before put it here
- some of pattern may not match with above instruction
 */
#define pGameFramework_offset 0x1B468D0
/*
pattern : 48 8B 0D ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 8B 54 24 ? 4C 8D 0D ? ? ? ? + 1
.text:0000000140568803                 mov     rcx, cs:pGameFramework
.text:000000014056880A                 mov     rax, [rcx]
.text:000000014056880D                 call    qword ptr [rax+0A0h]
.text:0000000140568813                 mov     rdx, [rsp+4C0h+var_458]
.text:0000000140568818                 lea     r9, aOwn        ; "own"
.text:000000014056881F                 lea     r8, aDude       ; "dude"

Or goto export list (in ida)
find function : CreateSystemInterface
look for 
  ModuleInitISystem(v3, "CrySystem");
  qword_141B468D0 = *(_QWORD *)(v1 + 0xA68);
then 0x1B468D0 is pGameFrameWork (141B468D0 - 140000000(base_address)
*/
#define pEntitySystem_offset  0x1B468E0
 /*
 pattern : E8 ? ? ? ? 4C 8B 7C 24 ? 4C 8B 6C 24 ? 48 83 C4 58 + 0x46
 "svoti_debug_probe" xref
 look aup one line
 mov     rcx, cs:pEntitySystem

 */
#define pRender_offset		  0x1B46930
 /*
 patter : 48 83 3D ? ? ? ? ? 0F 84 ? ? ? ? 48 89 B4 24 ? ? ? ?  +1
 "CScriptBind_System::Draw2DLine" xref -> 
 f5 (pesudo) 
 if ( some_function_bullshit(bullshit) == 8) {

  v4 = (*(__int64 (__fastcall **)(__int64, _QWORD))(*(_QWORD *)pRenderer + 1528i64))(pRenderer, 0i64); <-- pRender
  }

 Open CryRenderD3D11.dll with ida
 goto export list 
 find function : CreateSystemInterface ->
 f5 (pesudo) 

 v4 = (*(__int64 (**)(void))(*(_QWORD *)qword_1803C0110 + 600i64))();
  (*(void (__fastcall **)(__int64, void *(__fastcall ***)(std::_Facet_base *__hidden, unsigned int)))(*(_QWORD *)v4 + 8i64))(
	v4,
	&off_1803AC3C8);
  return qword_18044DBE0 <---;

  CryRenderD3D11.dll  + 0x44DBE0 = pRenderer
  it  work if you are to lazy to find it in sdk
 */
#define pPhysicalWorld_offset 0x1B46870
 /*
  pattern : 48 8B 3D ? ? ? ? 48 8D 4C 24 ? 33 D2  + 1
  take a look at IPhysicalWorld
  find "RayWorldIntersection(Game)" xref (just select one it dons't matter) ->
  f5 (pesudo) 
  find function that use "RayWorldIntersection(Game)" as paramiter
  like this
  (*(unsigned int (__fastcall **)(__int64, __int64 *, const char *, signed __int64))(v30 + 280))(
		 pPhysicalWorld,
		 &Dst,
		 "RayWorldIntersection(Game)",
		 4i64)
  first paramiter is pPhysicalWorld
 */
#define pConsole_offset		  0x1B468E8
 /*
 pattern : 48 8B 1D ? ? ? ? 48 8D 15 ? ? ? ? 48 8B F9  + 1
 find string like "cl_" "sv_" look at xref
 f5
 you will see stuff like (*qword_0x0000000 + index_something)(qword_0x0000000,"cl_blablabla"
 qword_0x0000000 - ba is pConsole 
 */
#define pLog_offset			  0x1B468F8
/*
pattern E8 ? ? ? ? 83 7B 10 0D  + 1
Actually you really don't need this one but if you want to 
just open up cryengine-sdk -> https://github.com/CRYTEK/CRYENGINE
and find CryLog("...") then find that string in ida
f5 then is should look like 
 if ( pLog )
	  (*(void (__fastcall **)(__int64, _QWORD, __int64, __int64 *))(*(_QWORD *)pLog + 8i64))(
		pLog,
		0i64,
		a1,
		(__int64 *)va);
ps. some of string in sdk might not show in releasse build if it doesn't show try another one
*/
#define SetPos_offset   0xC50350

// I'm sorry for not use actual vtable I just found it from reverse ScriptBind_Entity::SetLocalPos function
// find string SetLocalPos look a bit up grab the first function you see lea,sub_0func_offset go in to sub_0func_offset function
	/*
 sub_0func_offset
.text:0000000140C28450
.text:0000000140C28450                 mov     [rsp+arg_0], rbx
.text:0000000140C28455                 push    rdi
.text:0000000140C28456                 sub     rsp, 30h
.text:0000000140C2845A                 mov     rdi, r8
.text:0000000140C2845D                 mov     rbx, rdx
.text:0000000140C28460                 call    GetEntity
.text:0000000140C28465                 test    rax, rax
.text:0000000140C28468                 jz      short loc_140C28480
.text:0000000140C2846A                 xor     r9d, r9d
.text:0000000140C2846D                 mov     [rsp+38h+var_18], 0
.text:0000000140C28472                 xor     r8d, r8d
.text:0000000140C28475                 mov     rdx, rdi
.text:0000000140C28478                 mov     rcx, rax
.text:0000000140C2847B                 call    SetPos_0 <-- this is SetPos function
	 */
 bool sdk::Initialize(bool dump) {
	 static DWORD64 game_base = reinterpret_cast<DWORD64>(GetModuleHandleA(0));
	 sdk::global_var::pRender        = reinterpret_cast<IRender*>        (*(DWORD64*)(game_base + pRender_offset));
	 sdk::global_var::pPhysicalWorld = reinterpret_cast<IPhysicalWorld*> (*(DWORD64*)(game_base + pPhysicalWorld_offset));
	 sdk::global_var::pGameFramework = reinterpret_cast<IGameFramework*> (*(DWORD64*)(game_base + pGameFramework_offset));
	 sdk::global_var::pEntitySystem  = reinterpret_cast<IEntitySystem*>  (*(DWORD64*)(game_base + pEntitySystem_offset));
	 sdk::global_var::pConsole		 = reinterpret_cast<IConsole*>		 (*(DWORD64*)(game_base + pConsole_offset));
	 sdk::global_var::pLog			 = reinterpret_cast<void*>			 (*(DWORD64*)(game_base + pLog_offset));

	 if (dump) {
		 printf("game_base -> %p \n", GetModuleHandleA(0));
		 printf("sdk::global_var::pRender -> %p \n",		sdk::global_var::pRender);
		 printf("sdk::global_var::pPhysicalWorld -> %p \n", sdk::global_var::pPhysicalWorld);
		 printf("sdk::global_var::pGameFramework -> %p \n", sdk::global_var::pGameFramework);
		 printf("sdk::global_var::pEntitySystem -> %p \n", sdk::global_var::pEntitySystem);
		 printf("sdk::global_var::pLog -> %p \n", sdk::global_var::pLog);

		 printf("Window resolution: %dx%d (%s)\n"
			 ,sdk::global_var::pRender->window_width()
			 ,sdk::global_var::pRender->window_height()
			 ,sdk::global_var::pRender->is_full_screen() ? "FullScreen" : "Window Mode");
		 printf("Render resolution: %dx%d \n"
			 , sdk::global_var::pRender->render_width()
			 , sdk::global_var::pRender->render_height());
	 }

	 return sdk::global_var::pRender		&& 
			sdk::global_var::pPhysicalWorld && 
			sdk::global_var::pGameFramework && 
			sdk::global_var::pEntitySystem;
 }



 Vector IEntity::get_bone_position(__int64 id) {
	 ICharacterInstance* pCharacterInstance = this->get_character(0);
	 if (pCharacterInstance != 0) {
		 ISkeletonPose* pSkeletonPose = pCharacterInstance->GetISkeletonPose();
		 if (pSkeletonPose != 0) {
			 QuatT bitch = pSkeletonPose->GetAbsJointByID(id);
			 Matrix34 World = this->m_worldTM;
			 Matrix34 SkeletonAbs = Matrix34(bitch);
			 return (World * SkeletonAbs).GetTranslation();
		 }
	 }
	 return Vector();
 }

 Vector IEntity::get_bone_position(const char* name) {
	 __int64 id = this->get_character(0)->GetIDefaultSkeleton()->GetJointIDByName(name);
	 return get_bone_position(id);
 }

 void IEntity::set_position(Vector pos) {
	 ((void(__fastcall*)(IEntity*, Vector*, int))((DWORD64)(GetModuleHandleA(0)) + SetPos_offset))(this, &pos, 0);
	
 }

 