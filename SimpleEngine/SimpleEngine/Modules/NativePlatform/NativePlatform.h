#ifndef NATIVE_PLATFORM_H_
#define NATIVE_PLATFORM_H_

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "InputClass.h"
#include "GraphicsClass.h"
#include "soundclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
#include "PositionClass.h"

// 与 Win32 API 相关联的逻辑, 包括创建销毁窗口、操作显示器等
class NativePlatform
{
public:
	static NativePlatform& Instance()
	{
		static NativePlatform instance;
		return instance;
	}
	NativePlatform(NativePlatform const&) = delete;
	void operator=(DirectXPlatformManager const&) = delete;
	~NativePlatform();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	int GetScreenWidth() { return screen_width_; }
	int GetScreenHeight() { return screen_height_; }
	HWND GetWindowHandle() { return hwnd_; }

private:
	NativePlatform() {}
	bool Frame();
	bool HandleInput(float);
	void InitializeWindows();
	void ShutdownWindows();
	void OnWindowResized();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND hwnd_{ nullptr };

	InputClass* m_Input{ nullptr };
	GraphicsClass* m_Graphics{ nullptr };
	//SoundClass* m_Sound{ nullptr };
	FpsClass* m_Fps{ nullptr };
	CpuClass* m_Cpu{ nullptr };
	TimerClass* m_Timer{ nullptr };
	PositionClass* m_Position{ nullptr };
	// 玩家当前实际的窗口大小
	int screen_width_{ 1200 }, screen_height_{ 768 };
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#endif // !NATIVE_PLATFORM_H_