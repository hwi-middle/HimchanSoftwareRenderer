#include "pch.h"

Application::Application(uint32 InWidth, uint32 InHeight, WinRenderer* InRenderer, Input* InInputManager) : Renderer(InRenderer), InputManager(InInputManager), Width(InWidth), Height(InHeight)
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

Vector2 SquarePosition = Vector2(0.f, 0.f);
Vector2 InputVector = Vector2(0.f, 0.f);
constexpr float Speed = 500.f;
void Application::Update()
{
	//std::cout << "Horizontal: " << InputManager->GetAxis(EAxis::HORIZONTAL) << ", Vertical: " << InputManager->GetAxis(EAxis::VERTICAL) << std::endl;
	InputVector = Vector2(InputManager->GetAxis(EAxis::HORIZONTAL), InputManager->GetAxis(EAxis::VERTICAL));
	SquarePosition += InputVector.GetNormalized() * Speed * DeltaTime;
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
		Vertex v0 = VertexBuffer[IndexBuffer[bi]];
		Vertex v1 = VertexBuffer[IndexBuffer[bi + 1]];
		Vertex v2 = VertexBuffer[IndexBuffer[bi + 2]];

		v0.Position += SquarePosition;
		v1.Position += SquarePosition;
		v2.Position += SquarePosition;

		Renderer.DrawTriangle(v0, v1, v2, LineColor);
		Renderer.DrawLine(v0.Position, v1.Position, LineColor);
		Renderer.DrawLine(v0.Position, v2.Position, LineColor);
		Renderer.DrawLine(v1.Position, v2.Position, LineColor);

		//Renderer.DrawTriangle(VertexBuffer[IndexBuffer[bi]], VertexBuffer[IndexBuffer[bi + 1]], VertexBuffer[IndexBuffer[bi + 2]], LineColor);
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

