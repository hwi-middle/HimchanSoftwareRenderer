#include "pch.h"

constexpr float Speed = 5.f;
Transform TempTransform;

Application::Application(uint32 InWidth, uint32 InHeight, WinRenderer* InRenderer, Input* InInputManager) : Renderer(InRenderer), InputManager(InInputManager), Width(InWidth), Height(InHeight)
{
    MainCamera = std::make_unique<Camera>();
	Renderer->Initialize(Width, Height);
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&PrevTime);
    TempTransform.SetPosition(Vector3(100, -100, 10));
    TempTransform.SetScale(Vector3(100.f, 100.f, 100.f));
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
    TempTransform.AddPitchRoation(InputManager->GetAxis(EAxis::VERTICAL) * Speed * DeltaTime);
    TempTransform.AddYawRoation(InputManager->GetAxis(EAxis::HORIZONTAL) * Speed * DeltaTime);
}

void Application::LateUpdate()
{
}

void Application::Render()
{
	auto& Renderer = GetRenderer();
    auto& Cam = GetMainCamera();
	Color LineColor = Color::Black;

    static constexpr float CUBE_HALF_SIZE = 0.5f;
    static constexpr uint32 VERTEX_CNT = 24; // 6면 x 4버텍스
    static constexpr uint32 TRI_CNT = 12;    // 6면 x 2삼각형

    Vertex VertexBuffer[VERTEX_CNT] = {
        // Front face
        Vertex(Vector4(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Red, Vector2(0.f, 1.f)),
        Vertex(Vector4(-CUBE_HALF_SIZE,  CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Red, Vector2(0.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE,  CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Red, Vector2(1.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE, -CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Red, Vector2(1.f, 1.f)),

        // Back face
        Vertex(Vector4(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Blue, Vector2(0.f, 1.f)),
        Vertex(Vector4(-CUBE_HALF_SIZE,  CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Blue, Vector2(0.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE,  CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Blue, Vector2(1.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Blue, Vector2(1.f, 1.f)),

        // Left face
        Vertex(Vector4(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Green, Vector2(0.f, 1.f)),
        Vertex(Vector4(-CUBE_HALF_SIZE,  CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Green, Vector2(0.f, 0.f)),
        Vertex(Vector4(-CUBE_HALF_SIZE,  CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Green, Vector2(1.f, 0.f)),
        Vertex(Vector4(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Green, Vector2(1.f, 1.f)),

        // Right face
        Vertex(Vector4(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Yellow, Vector2(0.f, 1.f)),
        Vertex(Vector4(CUBE_HALF_SIZE,  CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Yellow, Vector2(0.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE,  CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Yellow, Vector2(1.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE, -CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Yellow, Vector2(1.f, 1.f)),

        // Top face
        Vertex(Vector4(-CUBE_HALF_SIZE,  CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::White, Vector2(0.f, 1.f)),
        Vertex(Vector4(-CUBE_HALF_SIZE,  CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::White, Vector2(0.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE,  CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::White, Vector2(1.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE,  CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::White, Vector2(1.f, 1.f)),

        // Bottom face
        Vertex(Vector4(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Black, Vector2(0.f, 1.f)),
        Vertex(Vector4(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Black, Vector2(0.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE, -CUBE_HALF_SIZE,  CUBE_HALF_SIZE, 1.f), Color::Black, Vector2(1.f, 0.f)),
        Vertex(Vector4(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, 1.f), Color::Black, Vector2(1.f, 1.f))
    };

    uint32 IndexBuffer[TRI_CNT * 3] = {
        // Front face
        0, 1, 2,
        0, 2, 3,

        // Back face
        4, 6, 5,
        4, 7, 6,

        // Left face
        8, 9, 10,
        8, 10, 11,

        // Right face
        12, 14, 13,
        12, 15, 14,

        // Top face
        16, 18, 17,
        16, 19, 18,

        // Bottom face
        20, 21, 22,
        20, 22, 23
    };


    Cam.GetTransform().SetPosition(Vector3(100, -100, 0));
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

            Vector3 e1 = (sub[1].Position - sub[0].Position).ToVector3();
            Vector3 e2 = (sub[2].Position - sub[0].Position).ToVector3();
            Vector3 normal = Vector3::Cross(e1, e2);
            normal.Normalize();
            Vector3 viewDir = Vector3::UnitZ;
            if (Vector3::Dot(normal, viewDir) >= 0)
            {
                continue;
            }

            Renderer.DrawTriangle(sub[0], sub[1], sub[2], Color::Clear);

            Renderer.DrawLine(sub[0].Position.ToVector2(), sub[1].Position.ToVector2(), Color::Red);
            Renderer.DrawLine(sub[0].Position.ToVector2(), sub[2].Position.ToVector2(), Color::Red);
            Renderer.DrawLine(sub[1].Position.ToVector2(), sub[2].Position.ToVector2(), Color::Red);
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

