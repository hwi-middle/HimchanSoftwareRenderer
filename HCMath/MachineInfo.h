#pragma once
class MachineInfo
{
public:
	~MachineInfo() = default;
	MachineInfo(const MachineInfo&) = delete;
	MachineInfo& operator=(const MachineInfo&) = delete;

	static MachineInfo* GetInstance();

	FORCEINLINE bool IsSseSupport() const { return mbIsSseSupport; }

private:
	MachineInfo();
	bool mbIsSseSupport;
};

