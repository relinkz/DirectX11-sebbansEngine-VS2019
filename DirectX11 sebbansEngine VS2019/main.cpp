#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// memoryleaks when exiting the window

	//COMException exception(0, "Shit", __FILE__, __FUNCTION__, __LINE__);
	//errorlogger::Log(exception);

	std::unique_ptr<Engine> engine = std::make_unique<Engine>();
	if(engine->Initialize(hInstance, "Sebbans Graphics Engine", "myWindowClass", 800, 600) == false)
	{
		OutputDebugStringA("Failed to initialize engine");
		return -1;
	}

	while (engine->ProcessMessages())
	{
		engine->Update();
		engine->RenderFrame();
	}

	return 0;
}