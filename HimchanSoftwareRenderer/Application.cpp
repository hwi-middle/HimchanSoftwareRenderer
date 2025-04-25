#include "pch.h"

constexpr float Speed = 5.f;
Transform TempTransform;

Application::Application(uint32 inWidth, uint32 inHeight, WinRenderer* inRenderer, Input* inInputManager) 
    : mRenderer(inRenderer), mInputManager(inInputManager), mWidth(inWidth), mHeight(inHeight)
{
    mMainCamera = std::make_unique<Camera>();
	mRenderer->initialize(mWidth, mHeight);
	QueryPerformanceFrequency(&mFrequency);
	QueryPerformanceCounter(&mPrevTime);
    TempTransform.SetPosition(Vector3(100, -100, 10));
    TempTransform.SetScale(Vector3(100.f, 100.f, 100.f));
}

Application::~Application()
{
}

void Application::Tick()
{
	preUpdate();
	update();
	render();
	lateUpdate();
	postUpdate();
}

void Application::Resize(uint32 inWidth, uint32 inHeight)
{
	mWidth = inWidth;
	mHeight = inHeight;
	mRenderer->Resize(mWidth, mHeight);
}

void Application::preUpdate()
{
	GetRenderer().FillBuffer();
}

void Application::update()
{
    TempTransform.AddPitchRotation(mInputManager->GetAxis(eEAxis::VERTICAL) * Speed * mDeltaTime);
    TempTransform.AddYawRotation(mInputManager->GetAxis(eEAxis::HORIZONTAL) * Speed * mDeltaTime);
}

void Application::lateUpdate()
{
}

void Application::render()
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
        v.position = FinalMatrix * v.position;
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

            Vector3 e1 = (sub[1].position - sub[0].position).ToVector3();
            Vector3 e2 = (sub[2].position - sub[0].position).ToVector3();
            Vector3 normal = Vector3::Cross(e1, e2);
            normal.Normalize();
            Vector3 viewDir = Vector3::unit_z;
            if (Vector3::Dot(normal, viewDir) >= 0)
            {
                continue;
            }

            Renderer.DrawTriangle(sub[0], sub[1], sub[2], Color::Clear);

            Renderer.DrawLine(sub[0].position.ToVector2(), sub[1].position.ToVector2(), Color::Red);
            Renderer.DrawLine(sub[0].position.ToVector2(), sub[2].position.ToVector2(), Color::Red);
            Renderer.DrawLine(sub[1].position.ToVector2(), sub[2].position.ToVector2(), Color::Red);
        }
    }
}

void Application::postUpdate()
{
	GetRenderer().SwapBuffer();
	GetInputManager().Update();

	QueryPerformanceCounter(&mCurrentTime);
	mDeltaTime = static_cast<float>(mCurrentTime.QuadPart - mPrevTime.QuadPart) / mFrequency.QuadPart;
	mPrevTime = mCurrentTime;
	mFps = 1.0f / mDeltaTime;
}

