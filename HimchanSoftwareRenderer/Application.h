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

	static constexpr uint32 VERTEX_CNT = 12;
	static constexpr uint32 TRI_CNT = 4;

	std::array<Vertex, VERTEX_CNT> VertexBuffer =
	{
		Vertex(Vector2(100.f, 115.f), Color::Red),
		Vertex(Vector2(-50.f, 150.f), Color::Green),
		Vertex(Vector2(50.f, 65.f), Color::Blue),

		Vertex(Vector2(-50.f, 50.f), Color::Red),
		Vertex(Vector2(-150.f, -50.f), Color::Green),
		Vertex(Vector2(50.f, -50.f), Color::Blue),

		Vertex(Vector2(-50.f, -70.f), Color::Cyan),
		Vertex(Vector2(-150.f, -170.f), Color::Magenta),
		Vertex(Vector2(50.f, -170.f), Color::Yellow),

		Vertex(Vector2(0.f, 50.f), Color::Red),
		Vertex(Vector2(200.f, 50.f), Color::Green),
		Vertex(Vector2(100.f, -50.f), Color::Blue),
	};

	std::array < uint32, TRI_CNT * 3> IndexBuffer = {
	   0, 1, 2,
	   3, 4, 5,
	   6, 7, 8,
	   9, 10, 11
	};
};