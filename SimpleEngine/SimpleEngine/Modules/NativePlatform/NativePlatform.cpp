#include <NativePlatform/NativePlatform.h>
#include <GUI/GUIManager.h>

NativePlatform::~NativePlatform()
{
	Shutdown();
}


bool NativePlatform::Initialize()
{
	// Initialize the windows api.
	InitializeWindows();

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}
	// Initialize the input object.
	bool result = m_Input->Initialize(m_hinstance, hwnd_, screen_width_, screen_height_);
	if (!result)
	{
		MessageBox(hwnd_, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}
	// Initialize the graphics object.
	result = m_Graphics->Initialize(screen_width_, screen_height_, hwnd_);
	if (!result)
	{
		return false;
	}

	// Create the sound object.
	//m_Sound = new SoundClass;
	//if (!m_Sound)
	//{
	//	return false;
	//}

	// Initialize the sound object.
	//result = m_Sound->Initialize(hwnd_);
	//if (!result)
	//{
	//	MessageBox(hwnd_, L"Could not initialize Direct Sound.", L"Error", MB_OK);
	//	return false;
	//}

	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();


	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd_, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}


	// Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(0.0f, 120.0f, -360.0f);

	SimpleEngine::UI::GUIManager::Instance().Initialize();

	return true;
}

// Shutdown and release the system object.
void NativePlatform::Shutdown()
{
	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the sound object.
	//if (m_Sound)
	//{
	//	m_Sound->Shutdown();
	//	delete m_Sound;
	//	m_Sound = 0;
	//}

	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}


void NativePlatform::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.  If frame processing fails then exit.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool NativePlatform::Frame()
{
	bool result;
	int mouseX, mouseY;


	// Do the input frame processing.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Do the frame input processing.
	result = HandleInput(m_Timer->GetTime());
	if (!result)
	{
		return false;
	}

	// Get the view point position/rotation.
	float posX, posY, posZ, rotX, rotY, rotZ;
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Get the location of the mouse from the input object,
	m_Input->GetMouseLocation(mouseX, mouseY);

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(mouseX, mouseY, m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime(),
		posX, posY, posZ, rotX, rotY, rotZ);
	if (!result)
	{
		return false;
	}

	return true;
}


bool NativePlatform::HandleInput(float frameTime)
{
	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// 前进
	bool keyDown = m_Input->IsWPressed();
	m_Position->MoveForward(keyDown);
	// 后退
	keyDown = m_Input->IsSPressed();
	m_Position->MoveBackward(keyDown);
	// 左移
	keyDown = m_Input->IsAPressed();
	m_Position->MoveLeft(keyDown);
	// 右移
	keyDown = m_Input->IsDPressed();
	m_Position->MoveRight(keyDown);

	// 向上看
	keyDown = m_Input->IsUpPressed();
	m_Position->LookUpward(keyDown);
	// 向下看
	keyDown = m_Input->IsDownPressed();
	m_Position->LookDownward(keyDown);
	// 向左看
	keyDown = m_Input->IsLeftPressed();
	m_Position->TurnLeft(keyDown);
	// 向右看
	keyDown = m_Input->IsRightPressed();
	m_Position->TurnRight(keyDown);

	// 上升
	keyDown = m_Input->IsRPressed();
	m_Position->MoveUpward(keyDown);
	// 下降
	keyDown = m_Input->IsFPressed();
	m_Position->MoveDownward(keyDown);

	return true;
}


LRESULT CALLBACK NativePlatform::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_SIZE:
	{
		if (wparam != SIZE_MINIMIZED)
		{
			OnWindowResized();
		}
		break;
	}
	// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	// 丢弃 Windows Input System => 等待Windows的消息通知, 可能发生延迟
	// 转用 DirectX input system => 每帧主动查询, 反馈更快
	//case WM_KEYDOWN:
	//{
	//	// If a key is pressed send it to the input object so it can record that state.
	//	m_Input->KeyDown((unsigned int)wparam);
	//	return 0;
	//}
	//case WM_KEYUP:
	//{
	//	// If a key is released then send it to the input object so it can unset the state for that key.
	//	m_Input->KeyUp((unsigned int)wparam);
	//	return 0;
	//}
	default:
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}

	return 0;
}


void NativePlatform::InitializeWindows()
{
	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"SimpleEngine";

	// Setup the windows class with default settings.
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	int posX, posY;
	if (FULL_SCREEN)
	{
		// Determine the resolution of the clients desktop screen.
		screen_width_ = GetSystemMetrics(SM_CXSCREEN);
		screen_height_ = GetSystemMetrics(SM_CYSCREEN);

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screen_width_;
		dmScreenSettings.dmPelsHeight = (unsigned long)screen_height_;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screen_width_) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screen_height_) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	// 无边框
	//hwnd_ = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
	//	WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
	//	posX, posY, screen_width_, screen_height_, NULL, NULL, m_hinstance, NULL);
	// 普通窗口
	hwnd_ = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		posX, posY, screen_width_, screen_height_, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd_, SW_SHOW);
	SetForegroundWindow(hwnd_);
	SetFocus(hwnd_);
}


void NativePlatform::ShutdownWindows()
{
	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(hwnd_);
	hwnd_ = nullptr;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	return;
}

void NativePlatform::OnWindowResized()
{
	RECT rect = { 0, 0, 0, 0 };
	GetClientRect(hwnd_, &rect);
	screen_width_ = rect.right - rect.left;
	screen_height_ = rect.bottom - rect.top;

	DirectXPlatformManager::Instance().OnWindowResized();
	SimpleEngine::UI::GUIManager::Instance().OnWindowResized();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return NativePlatform::Instance().MessageHandler(hwnd, umessage, wparam, lparam);
}