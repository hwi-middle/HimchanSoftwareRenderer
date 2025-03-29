#pragma once

class Application
{
public:
	Application(uint32 inWidth, uint32 inHeight, WinRenderer* inRenderer, Input* inInputManager);
	~Application();
	void Tick();

	void Resize(uint32 inWidth, uint32 inHeight);

	FORCEINLINE WinRenderer& GetRenderer() const { return *mRenderer.get(); }
	FORCEINLINE Input& GetInputManager() const { return *mInputManager.get(); }
	FORCEINLINE Camera& GetMainCamera() const { return *mMainCamera.get(); }
	FORCEINLINE float GetDeltaTime() const { return mDeltaTime; }
	FORCEINLINE float GetFps() const { return mFps; }

private:
	void preUpdate();
	void update();
	void lateUpdate();
	void render();
	void postUpdate();

	uint32 mWidth;
	uint32 mHeight;

	std::unique_ptr<WinRenderer> mRenderer;
	std::unique_ptr<Input> mInputManager;
	std::unique_ptr<Camera> mMainCamera;

	LARGE_INTEGER mFrequency;
	LARGE_INTEGER mPrevTime;
	LARGE_INTEGER mCurrentTime;
	float mDeltaTime = 0;

	float mFps = 0;
};