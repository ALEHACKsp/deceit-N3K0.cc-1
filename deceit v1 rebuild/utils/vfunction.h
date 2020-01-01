#pragma once

template<typename FuncType>
__forceinline static FuncType CallVFunction(void* ppClass, int index)
{
	DWORD64* pVTable = *(DWORD64**)ppClass;
	DWORD64 dwAddress = pVTable[index];
	return (FuncType)(dwAddress);
}