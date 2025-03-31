#include "pch.h"
#include "MachineInfo.h"

MachineInfo::MachineInfo()
{
	int cpuInfo[4];
	__cpuid(cpuInfo, 1);
	mbIsSseSupport = (cpuInfo[3] & (1 << 25)) != 0;
}

MachineInfo* MachineInfo::GetInstance()
{
	static MachineInfo sInstance;
	return &sInstance;
}
