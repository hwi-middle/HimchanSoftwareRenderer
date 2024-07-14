#include "pch.h"

Application::Application(uint32 InWidth, uint32 InHeight, WinRenderer* InRenderer) : Renderer(InRenderer), Width(InWidth), Height(InHeight)
{
	Renderer->Initialize(Width, Height);
}

Application::~Application()
{
}

void Application::Tick()
{
	PreUpdate();
	Update();
	Render();
	LateUpdate();
	PostUpdate();
}

void Application::PreUpdate()
{
	//std::cout << "PreUpdate" << std::endl;
	GetRenderer().FillBuffer();
}

void Application::Update()
{
	//std::cout << "Update" << std::endl;
	pos = Math::Cos(t++ * 0.02f) * (Height / 2);
	//std::cout << pos << std::endl;
}

void Application::LateUpdate()
{
	//std::cout << "LateUpdate" << std::endl;
}

void Application::Render()
{
	//std::cout << "Render" << std::endl;
	std::cout << "pos: " << pos << std::endl;
	GetRenderer().DrawLine(Vector2(Width, Height), Vector2(-(int)Width, pos), Vector2(Width, pos), Color(1.f, 0.f, 0.f, 1.f));
}

void Application::PostUpdate()
{
	//std::cout << "PostUpdate" << std::endl;
	GetRenderer().SwapBuffer();
}

