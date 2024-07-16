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

void Application::Resize(uint32 InWidth, uint32 InHeight)
{
	std::cout << "Resize: (" << Width << ", " << Height << ") -> (" << InWidth << ", " << InHeight << ")" << std::endl;
	Width = InWidth;
	Height = InHeight;
	Renderer->Resize(Width, Height);
}

void Application::PreUpdate()
{
	GetRenderer().FillBuffer();
}

void Application::Update()
{
	//posY = Math::Sin(t * 0.5f) * (Height / 2);
	//posX = Math::Cos(t * 0.5f) * (Width / 2);

	posY = 0;
	posX = 0;
}

void Application::LateUpdate()
{
	t += 0.02f;
}

void Application::Render()
{
	//std::cout << "pos: (" << posX << ", " << posY << ")" << std::endl;
	GetRenderer().DrawLine(Vector2(-(int)Width, posY), Vector2(Width, posY), Color::Red);
	GetRenderer().DrawLine(Vector2(posX, -(int)Height), Vector2(posX, Height), Color::Green);

	GetRenderer().DrawPoint(Vector2(posX, posY), Color::Blue);
	GetRenderer().DrawPoint(Vector2(posX + 1, posY), Color::Blue);
	GetRenderer().DrawPoint(Vector2(posX - 1, posY), Color::Blue);
	GetRenderer().DrawPoint(Vector2(posX, posY + 1), Color::Blue);
	GetRenderer().DrawPoint(Vector2(posX, posY - 1), Color::Blue);
	GetRenderer().DrawPoint(Vector2(posX + 1, posY + 1), Color::Blue);
	GetRenderer().DrawPoint(Vector2(posX + 1, posY - 1), Color::Blue);
	GetRenderer().DrawPoint(Vector2(posX - 1, posY + 1), Color::Blue);
	GetRenderer().DrawPoint(Vector2(posX - 1, posY - 1), Color::Blue);
}

void Application::PostUpdate()
{
	GetRenderer().SwapBuffer();
}

