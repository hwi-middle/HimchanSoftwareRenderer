#pragma once

class Application
{
public:
	Application();
	~Application();
	void Tick();

private:
	void PreUpdate();
	void Update();
	void LateUpdate();
	void Render();
};