#ifndef DIRECTX_PLATFORM_MANAGER_H_
#define DIRECTX_PLATFORM_MANAGER_H_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "XmlHelper.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

const float SCREEN_NEAR = 0.1f;
const float SCREEN_DEPTH = 1000.0f;

class DirectXPlatformManager
{
public:
	static DirectXPlatformManager& Instance()
	{
		static DirectXPlatformManager instance;
		return instance;
	}
	DirectXPlatformManager(DirectXPlatformManager const&) = delete;
	void operator=(DirectXPlatformManager const&) = delete;
	~DirectXPlatformManager();

	bool Initialize(int, int, HWND, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	void SetBackBufferRenderTarget();

	void ResetViewport();
	void ResetMatrix();
	void ResetBackBufferAndDepthStencilBuffer();
	void OnWindowResized();

private:
	DirectXPlatformManager();

	int m_videoCardMemory;
	char m_videoCardDescription[128];
	
	ID3D11Device* device_{ nullptr };
	IDXGISwapChain* swap_chain_{ nullptr };
	ID3D11DeviceContext* device_context_{ nullptr };
	ID3D11RenderTargetView* render_target_view_{ nullptr };
	ID3D11Texture2D* depth_stencil_buffer_{ nullptr };
	ID3D11DepthStencilState* depth_stencil_state_{ nullptr };
	ID3D11DepthStencilView* depth_stencil_view_{ nullptr };
	ID3D11RasterizerState* m_rasterState;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;

	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
};

#endif // !DIRECTX_PLATFORM_MANAGER_H_