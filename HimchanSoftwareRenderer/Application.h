#pragma once

class Application
{
public:
	Application(uint32 InWidth, uint32 InHeight, WinRenderer* InRenderer, Input* InInputManager);
	~Application();
	void Tick();

	void Resize(uint32 InWidth, uint32 InHeight);

	FORCEINLINE WinRenderer& GetRenderer() const { return *Renderer.get(); }
	FORCEINLINE Input& GetInputManager() const { return *InputManager.get(); }
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
	std::unique_ptr<Input> InputManager;

	LARGE_INTEGER Frequency;
	LARGE_INTEGER PrevTime;
	LARGE_INTEGER CurrentTime;
	float DeltaTime = 0;

	float Fps = 0;
	int32 FrameCount = 0;
	float ElapsedTime = 0;

	static constexpr float SQUARE_HALF_SIZE = 80.f;
	static constexpr uint32 VERTEX_CNT = 4;
	static constexpr uint32 TRI_CNT = 2;

	Vertex VertexBuffer[VERTEX_CNT] = {
		Vertex(Vector2(-SQUARE_HALF_SIZE, -SQUARE_HALF_SIZE), Color::Red, Vector2(0.f, 1.f)),
		Vertex(Vector2(-SQUARE_HALF_SIZE, SQUARE_HALF_SIZE), Color::Red, Vector2(0.f, 0.f)),
		Vertex(Vector2(SQUARE_HALF_SIZE, SQUARE_HALF_SIZE), Color::Red, Vector2(1.f, 0.f)),
		Vertex(Vector2(SQUARE_HALF_SIZE, -SQUARE_HALF_SIZE), Color::Red, Vector2(1.f, 1.f))
	};

	uint32 IndexBuffer[TRI_CNT * 3] = {
		0, 1, 2,
		0, 2, 3
	};
};