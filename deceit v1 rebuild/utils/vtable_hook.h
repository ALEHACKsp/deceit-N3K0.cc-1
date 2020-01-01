#pragma once
#include <Windows.h>
#include <stdio.h>
#include <assert.h>

#include <memory>
// New class
//class vtable_hook {
//public:
//	~vtable_hook();
//	void setup(void* _class) {
//		try {
//			if (!_class || _class == 0)
//				throw "_class is null are you out of your mind ? ";
//			pVtable = std::shared_ptr<void*>(static_cast<void**>(*(void**)_class));
//			vtable_size = get_vtable_size() - 1;
//			original_vtable = std::shared_ptr<void*>(new void* [vtable_size]);
//			memcpy(original_vtable.get(), pVtable.get(), vtable_size * sizeof(void*));
//			printf("vtable_size : %lld \n", vtable_size);
//			printf("original_vtable : %p \n", original_vtable.get());
//		}
//		catch (const char* error) {
//			printf("[ERROR] %s \n", error);
//			global_var::eject_module = true;
//		}
//	}
//
//	void hook_virtual_function(int _index, void* to_hook) {
//		if (!pVtable || pVtable == 0) 
//			throw "Original vtable is null";
//		
//		if (_index <= vtable_size) 
//			throw "function index is out of range";
//
//		DWORD old_protect = 0;
//		LPVOID address = (void*)(reinterpret_cast<uintptr_t>(pVtable.get()) + (sizeof(void*) * (uintptr_t)_index));
//
//		if (!VirtualProtect(address, sizeof(void*), PAGE_READWRITE, &old_protect)) 
//			throw "can't change region memory protection";
//
//		pVtable.get()[_index] = to_hook;
//
//		if (!VirtualProtect(address, sizeof(void*), old_protect, 0)) 
//			throw "can't restore region memory protection";
//
//	}
//	void unhook_virtual_function(int _index) {
//		if (!pVtable || pVtable == 0)
//			throw "Original vtable is null";
//		if (_index <= vtable_size)
//			throw "function index is out of range";
//
//		DWORD old_protect = 0 ;
//		LPVOID address = (void*)(reinterpret_cast<uintptr_t>(pVtable.get()) + (sizeof(void*) * (uintptr_t)_index));
//		if (!VirtualProtect(address, sizeof(void*), PAGE_READWRITE, &old_protect))
//			throw "can't change region memory protection";
//
//		pVtable.get()[_index] = original_vtable.get()[_index];
//
//		if (!VirtualProtect(address, sizeof(void*), old_protect, 0))
//			throw "can't restore region memory protection";
//	}
//	template<typename type>
//	type get_orignial_function(int _index) {
//		if (!pVtable || pVtable == 0)
//			throw "Original vtable is null";
//		return (type)original_vtable.get()[_index];
//	}
//
//	size_t get_vtable_size() {
//		if (!pVtable || pVtable == 0)
//			throw "Original vtable is null";
//		MEMORY_BASIC_INFORMATION memInfo = { NULL };
//		int m_nSize = -1;
//		do {
//			m_nSize++;
//			VirtualQuery(reinterpret_cast<LPCVOID>(pVtable.get()[m_nSize]), &memInfo, sizeof(memInfo));
//
//		} while (memInfo.Protect == PAGE_EXECUTE_READ || memInfo.Protect == PAGE_EXECUTE_READWRITE);
//
//		return m_nSize;
//	}
//	bool should_unhook() {
//		return pVtable || original_vtable || vtable_size;
//	}
//private:
//	size_t vtable_size = 0;
//	std::shared_ptr<void*> pVtable = 0;
//	std::shared_ptr<void*> original_vtable=0 ;
//};
//
//vtable_hook::~vtable_hook() {
//	original_vtable.reset();
//}

// well i'm somehow fucked up vtable_hook class sooooooooo
// Cr. CsgoSimple
#include <Windows.h>

#include <cstdint>
#include <stdexcept>

namespace detail
{
	class protect_guard
	{
	public:
		protect_guard(void* base, size_t len, std::uint32_t flags)
		{
			_base = base;
			_length = len;
			if (!VirtualProtect(base, len, flags, (PDWORD)&_old))
				throw std::runtime_error("Failed to protect region.");
		}
		~protect_guard()
		{
			VirtualProtect(_base, _length, _old, (PDWORD)&_old);
		}

	private:
		void* _base;
		size_t        _length;
		std::uint32_t _old;
	};
}

class vtable_hook
{
public:
	vtable_hook();
	vtable_hook(void* base);
	~vtable_hook();

	bool setup(void* class_base = nullptr);	

	template<typename T>
	void hook_index(int index, T fun)
	{
		assert(index >= 0 && index <= (int)vftbl_len);
		new_vftbl[index + 1] = reinterpret_cast<std::uintptr_t>(fun);
	}
	void unhook_index(int index)
	{
		new_vftbl[index] = old_vftbl[index];
	}
	void unhook_all()
	{
		try {
			if (old_vftbl != nullptr) {
				auto guard = detail::protect_guard{ class_base, sizeof(std::uintptr_t), PAGE_READWRITE };
				*(std::uintptr_t**)class_base = old_vftbl;
				old_vftbl = nullptr;
			}
		}
		catch (...) {
		}
	}

	template<typename T>
	T get_original(int index)
	{
		return (T)old_vftbl[index];
	}

private:
	static inline std::size_t estimate_vftbl_length(std::uintptr_t* vftbl_start);

	void* class_base;
	std::size_t     vftbl_len;
	std::uintptr_t* new_vftbl;
	std::uintptr_t* old_vftbl;
};