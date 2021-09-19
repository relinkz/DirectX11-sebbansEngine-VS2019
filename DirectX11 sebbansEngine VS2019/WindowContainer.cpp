#include "WindowContainer.h"

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // rerouted from RenderWindow
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
