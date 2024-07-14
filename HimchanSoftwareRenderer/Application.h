#pragma once

class Application
{
public:
	Application(uint32 InWidth, uint32 InHeight, WinRenderer* InRenderer);
	~Application();
	void Tick();

	FORCEINLINE WinRenderer& GetRenderer() { return *Renderer.get(); }

private:
	void PreUpdate();
	void Update();
	void LateUpdate();
	void Render();
	void PostUpdate();

	uint32 Width, Height;
	float posY;
	float posX;
	float t = 0;

	std::unique_ptr<WinRenderer> Renderer;
};