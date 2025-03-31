#pragma once
class MachineInfo
{
public:
	MachineInfo();
	~MachineInfo() = default;
	MachineInfo(const MachineInfo&) = delete;
	MachineInfo& operator=(const MachineInfo&) = delete;

	static MachineInfo* GetInstance();

	FORCEINLINE bool IsSseSupport() const { return mbIsSseSupport; }

private:
	bool mbIsSseSupport;
};

