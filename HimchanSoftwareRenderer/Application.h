#pragma once

class Application
{
public:
	Application(uint32 InWidth, uint32 InHeight, WinRenderer* InRenderer);
	~Application();
	void Tick();

	void Resize(uint32 InWidth, uint32 InHeight);

	FORCEINLINE WinRenderer& GetRenderer() const { return *Renderer.get(); }
	FORCEINLINE float GetDeltaTime() const { return DeltaTime; }
	FORCEINLINE float GetFps() const { return Fps; }

private:
	void PreUpdate();
	void Update();
	void LateUpdate();
	void Render();
	void PostUpdate();

	uint32 Width, Height;

	std::unique_ptr<WinRenderer> Renderer;

	LARGE_INTEGER Frequency;
	LARGE_INTEGER PrevTime;
	LARGE_INTEGER CurrentTime;
	float DeltaTime = 0;
	
	float Fps = 0;
	int32 FrameCount = 0;
	float ElapsedTime = 0;

	float posY;
	float posX;
	float t = 0;
};