#include "pch.h"
#include "SList.h"

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    

void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
void RenderFrame(void);     // renders a single frame
void CleanD3D(void);         // closes Direct3D and releases memory
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/*******************************************************************************************************************/
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 800, 600 };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
						  L"WindowClass1",    // name of the window class
						  L"DirectX Program",   // title of the window
						  WS_OVERLAPPEDWINDOW,    // window style
						  500,    // x-position of the window
						  300,    // y-position of the window
						  wr.right - wr.left,    // width of the window
						  wr.bottom - wr.top,    // height of the window
						  NULL,    // we have no parent window, NULL
						  NULL,    // we aren't using menus, NULL
						  hInstance,    // application handle
						  NULL);    // used with multiple windows, NULL

	// display the window on the screen
	ShowWindow(hWnd, nShowCmd);

	// set up and initialize Direct3D
	InitD3D(hWnd);

	MSG msg = { 0 };

	while (TRUE)
	{
		//FieaGameEngine::SList list(10);
		//std::int32_t data = list.Data();
		//data;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// check to see if it's time to quit
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			// Run game code here
		}

		RenderFrame();
	}

	// clean up DirectX and COM
	CleanD3D();

	return (int)msg.wParam;
}

/*******************************************************************************************************************/
// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

    // create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, NULL, &devcon);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	devcon->RSSetViewports(1, &viewport);
}

/*******************************************************************************************************************/
// this is the function used to render a single frame
void RenderFrame(void)
{
	// clear the back buffer to a deep blue
	std::float_t color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	devcon->ClearRenderTargetView(backbuffer, color);

	// do 3D rendering on the back buffer here

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}

/*******************************************************************************************************************/
// this is the function that cleans up Direct3D and COM
void CleanD3D()
{
	// close and release all existing COM objects
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}

/*******************************************************************************************************************/
// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			// close the application entirely
			PostQuitMessage(0);
			return 0;
		} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}