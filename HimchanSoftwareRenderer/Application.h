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

	static constexpr float SQUARE_HALF_SIZE = 50.f;
	static constexpr uint32 VERTEX_CNT = 4;
	static constexpr uint32 TRI_CNT = 1;

	std::array<Vertex, VERTEX_CNT> VertexBuffer =
	{
		Vertex(Vector2(300.f, 75.f), Color::Red),
		Vertex(Vector2(-150.f, 180.f), Color::Green),
		Vertex(Vector2(150.f, -75.f), Color::Blue),
	};

	std::array < uint32, TRI_CNT * 3> IndexBuffer = {
	   0, 1, 2,
	};
};