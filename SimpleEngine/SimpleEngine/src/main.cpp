#include <NativePlatform/NativePlatform.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool result = NativePlatform::Instance().Initialize();
	if(result)
	{
		NativePlatform::Instance().Run();
	}

	return 0;
}