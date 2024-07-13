#include <iostream>
#include "Application.h"

Application::Application()
{
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
}

void Application::PreUpdate()
{
	std::cout << "PreUpdate" << std::endl;
}

void Application::Update()
{
	std::cout << "Update" << std::endl;
}

void Application::LateUpdate()
{
	std::cout << "LateUpdate" << std::endl;
}

void Application::Render()
{
	std::cout << "Render" << std::endl;
}

