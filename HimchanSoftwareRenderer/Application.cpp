#include "pch.h"

Application::Application(uint32 InWidth, uint32 InHeight, WinRenderer* InRenderer,Input* InInputManager) : Renderer(InRenderer), InputManager(InInputManager), Width(InWidth), Height(InHeight)
{
	Renderer->Initialize(Width, Height);
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&PrevTime);
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
	if (InputManager->GetKeyDown(EKeyCode::ESC))
	{
		std::cout << "ESC Key Down" << std::endl;
	}
	if (InputManager->GetKey(EKeyCode::ESC))
	{
		std::cout << "ESC Key" << std::endl;
	}
	if (InputManager->GetKeyUp(EKeyCode::ESC))
	{
		std::cout << "ESC Key Up" << std::endl;
	}
}

void Application::LateUpdate()
{
}

void Application::Render()
{
	GetRenderer().DrawLine(Vector2(-(int)Width, 0), Vector2(Width, 0), Color::Red);
	GetRenderer().DrawLine(Vector2(0, -(int)Height), Vector2(0, Height), Color::Green);

	auto& Renderer = GetRenderer();
	Color LineColor = Color::Blue;

	for (uint32 ti = 0; ti < TRI_CNT; ++ti)
	{
		uint32 bi = ti * 3;
		Renderer.DrawTriangle(VertexBuffer[IndexBuffer[bi]], VertexBuffer[IndexBuffer[bi + 1]], VertexBuffer[IndexBuffer[bi + 2]], LineColor);
	}
}

void Application::PostUpdate()
{
	GetRenderer().SwapBuffer();
	GetInputManager().Update();
	
	QueryPerformanceCounter(&CurrentTime);
	DeltaTime = static_cast<float>(CurrentTime.QuadPart - PrevTime.QuadPart) / Frequency.QuadPart;
	PrevTime = CurrentTime;
	Fps = 1.0f / DeltaTime;
}

