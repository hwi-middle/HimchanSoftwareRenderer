#include "pch.h"

Application::Application(uint32 InWidth, uint32 InHeight, WinRenderer* InRenderer, Input* InInputManager) : Renderer(InRenderer), InputManager(InInputManager), Width(InWidth), Height(InHeight)
{
    MainCamera = std::make_unique<Camera>();
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
}

void Application::LateUpdate()
{
}

void Application::Render()
{
	auto& Renderer = GetRenderer();
    auto& Cam = GetMainCamera();
    Transform TempTransform;
	Color LineColor = Color::Black;

    static constexpr float CUBE_HALF_SIZE = 0.5f;
    static constexpr uint32 VERTEX_CNT = 8;
    static constexpr uint32 TRI_CNT = 12;

    Vertex VertexBuffer[VERTEX_CNT] = {
        // Front face
        Vertex(Vector4(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Red, Vector2(0.f, 1.f)),
        Vertex(Vector4(-CUBE_HALF_SIZE,  CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Red, Vector2(0.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE,  CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Red, Vector2(1.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE, -CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Red, Vector2(1.f, 1.f)),

        // Back face
        Vertex(Vector4(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Blue, Vector2(1.f, 1.f)),
        Vertex(Vector4(-CUBE_HALF_SIZE,  CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Blue, Vector2(1.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE,  CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Blue, Vector2(0.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Blue, Vector2(0.f, 1.f))
    };

    uint32 IndexBuffer[TRI_CNT * 3] = {
        // Front face
        0, 1, 2,
        0, 2, 3,

        // Back face
        4, 6, 5,
        4, 7, 6,

        // Left face
        4, 5, 1,
        4, 1, 0,

        // Right face
        3, 2, 6,
        3, 6, 7,

        // Top face
        1, 5, 6,
        1, 6, 2,

        // Bottom face
        4, 0, 3,
        4, 3, 7
    };

    TempTransform.SetPosition(Vector3(100,-100,10));
    TempTransform.Rotate(45, 45, 0);
    TempTransform.SetScale(Vector3(100.f, 100.f, 100.f));
    Matrix4x4 FinalMatrix = Cam.GetViewMatrix() * TempTransform.GetModelingMatrix();
    for (auto& v : VertexBuffer)
    {
        v.Position = FinalMatrix * v.Position;
    }

    for (int ti = 0; ti < TRI_CNT; ++ti)
    {
        int bi0 = ti * 3, bi1 = ti * 3 + 1, bi2 = ti * 3 + 2;
        std::vector<Vertex> tvs = { VertexBuffer[IndexBuffer[bi0]] , VertexBuffer[IndexBuffer[bi1]] , VertexBuffer[IndexBuffer[bi2]] };

        size_t triangles = tvs.size() / 3;
        for (size_t ti = 0; ti < triangles; ++ti)
        {
            size_t si = ti * 3;
            std::vector<Vertex> sub(tvs.begin() + si, tvs.begin() + si + 3);
            Renderer.DrawLine(sub[0].Position.ToVector2(), sub[1].Position.ToVector2(), LineColor);
            Renderer.DrawLine(sub[0].Position.ToVector2(), sub[2].Position.ToVector2(), LineColor);
            Renderer.DrawLine(sub[1].Position.ToVector2(), sub[2].Position.ToVector2(), LineColor);
        }
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

