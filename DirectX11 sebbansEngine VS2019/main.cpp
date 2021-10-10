#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();
	engine->Initialize(hInstance, "Test title", "myWindowClass", 800, 600);

	while (engine->ProcessMessages())
	{
		engine->Update();
	}

	return 0;
}