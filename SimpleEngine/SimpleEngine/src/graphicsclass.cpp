#include "GraphicsClass.h"
#include <GUI/GUIManager.h>

GraphicsClass::GraphicsClass()
{
	//m_HumanModel = 0;
	//m_GroundModel = 0;
	m_ShaderManager = 0;
	m_DirectionalLight = 0;
	m_PointLight = 0;
	m_RenderTexture = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	//D3DXMATRIX baseViewMatrix;

	// Initialize the Direct3D object.
	result = DirectXPlatformManager::Instance().Initialize(screenWidth, screenHeight, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_Camera = new Camera;
	if(!m_Camera)
	{
		return false;
	}
	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 100.0f, -300.0f);
	m_Camera->Render();
	//m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the model object.
	//m_HumanModel = new ModelClass;
	//if (!m_HumanModel)
	//{
	//	return false;
	//}

	// Initialize the model object.
	//result = m_HumanModel->Initialize(DirectXPlatformManager::Instance().GetDevice(), DirectXPlatformManager::Instance().GetDeviceContext(), "./media/Models/Bayonetta2/model.xml");
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
	//	return false;
	//}

	//m_HumanModel->SetPosition(-2.0f, 2.0f, 0.0f);


	// Create the ground model object.
	//m_GroundModel = new ModelClass;
	//if (!m_GroundModel)
	//{
	//	return false;
	//}
	//result = m_GroundModel->Initialize(DirectXPlatformManager::Instance().GetDevice(), DirectXPlatformManager::Instance().GetDeviceContext(), "./media/Models/Sponza/model.xml");
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the ground model object.", L"Error", MB_OK);
	//	return false;
	//}
	//m_GroundModel->SetPosition(0.0f, 1.0f, 0.0f);


	// Create the shader manager object.
	m_ShaderManager = new ShaderManagerClass;
	if (!m_ShaderManager)
	{
		return false;
	}

	// Initialize the shader manager object.
	result = m_ShaderManager->Initialize(DirectXPlatformManager::Instance().GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}


	// Create the light object.
	m_DirectionalLight = new DirectionalLightClass;
	if (!m_DirectionalLight)
	{
		return false;
	}

	// Initialize the light object.
	m_DirectionalLight->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_DirectionalLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirectionalLight->SetDirection(0.0f, 0.0f, 1.0f);
	m_DirectionalLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirectionalLight->SetSpecularPower(32.0f);


	// Create the light object.
	m_PointLight = new PointLightClass;
	if (!m_PointLight)
	{
		return false;
	}

	// Initialize the light object.
	m_PointLight->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_PointLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_PointLight->SetLookAt(0.0f, 0.0f, 0.0f);
	m_PointLight->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);


	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(DirectXPlatformManager::Instance().GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the light object.
	if (m_PointLight)
	{
		delete m_PointLight;
		m_PointLight = 0;
	}

	// Release the ground model object.
	//if (m_GroundModel)
	//{
	//	delete m_GroundModel;
	//	m_GroundModel = 0;
	//}

	// Release the light object.
	if (m_DirectionalLight)
	{
		delete m_DirectionalLight;
		m_DirectionalLight = 0;
	}

	// Release the light shader object.
	if (m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	// Release the model object.
	//if (m_HumanModel)
	//{
	//	delete m_HumanModel;
	//	m_HumanModel = 0;
	//}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	return;
}


bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	// Set the location of the mouse.
	//bool result = m_Text->SetMousePosition(mouseX, mouseY, DirectXPlatformManager::Instance().GetDeviceContext());
	//if(!result)
	//{
	//	return false;
	//}

	// Set the frames per second.
	//result = m_Text->SetFps(fps, DirectXPlatformManager::Instance().GetDeviceContext());
	//if (!result)
	//{
	//	return false;
	//}

	// Set the cpu usage.
	//result = m_Text->SetCpu(cpu, DirectXPlatformManager::Instance().GetDeviceContext());
	//if (!result)
	//{
	//	return false;
	//}

	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Update the position of the light each frame.
	static float lightPositionX = -5.0f;
	lightPositionX += 0.05f;
	if (lightPositionX > 5.0f)
	{
		lightPositionX = -5.0f;
	}

	// Update the position of the light.
	m_PointLight->SetPosition(lightPositionX, 1000.0f, -50.0f);

	
	// Render the graphics scene.
	if (!Render())
	{
		return false;
	}

	return true;
}


bool GraphicsClass::RenderSceneToTexture()
{
	//D3DXMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, translateMatrix;
	//float posX, posY, posZ;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(DirectXPlatformManager::Instance().GetDeviceContext());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(DirectXPlatformManager::Instance().GetDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	// Generate the light view matrix based on the light's position.
	m_PointLight->GenerateViewMatrix();

	// Get the view and orthographic matrices from the light object.
	//m_PointLight->GetViewMatrix(lightViewMatrix);
	//m_PointLight->GetProjectionMatrix(lightProjectionMatrix);


	// Get the world matrix from the d3d object.
	//DirectXPlatformManager::Instance().GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the cube model.
	//m_HumanModel->GetPosition(posX, posY, posZ);
	//D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the cube model with the depth shader.
	//m_HumanModel->RenderDepthShader(m_ShaderManager, DirectXPlatformManager::Instance().GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);


	// Reset the world matrix.
	//DirectXPlatformManager::Instance().GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the ground model.
	//m_GroundModel->GetPosition(posX, posY, posZ);
	//D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the ground model with the depth shader.
	//m_GroundModel->RenderDepthShader(m_ShaderManager, DirectXPlatformManager::Instance().GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	DirectXPlatformManager::Instance().SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	DirectXPlatformManager::Instance().ResetViewport();

	return true;
}


bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	// First render the scene to a texture.
	bool result = RenderSceneToTexture();
	if (!result)
	{
		return false;
	}

	// Clear the buffers to begin the scene.
	DirectXPlatformManager::Instance().BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Generate the light view matrix based on the light's position.
	m_PointLight->GenerateViewMatrix();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	DirectXPlatformManager::Instance().GetWorldMatrix(worldMatrix);
	DirectXPlatformManager::Instance().GetProjectionMatrix(projectionMatrix);
	DirectXPlatformManager::Instance().GetOrthoMatrix(orthoMatrix);

	// Get the light's view and projection matrices from the light object.
	D3DXMATRIX lightViewMatrix, lightProjectionMatrix;
	m_PointLight->GetViewMatrix(lightViewMatrix);
	m_PointLight->GetProjectionMatrix(lightProjectionMatrix);

	// first do all your 3D rendering
	// then turn the Z buffer off and do your 2D rendering
	// and then turn the Z buffer on again

	// Reset the world matrix.
	DirectXPlatformManager::Instance().GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the ground model.
	//float posX, posY, posZ;
	//m_GroundModel->GetPosition(posX, posY, posZ);
	//D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the ground model using the shadow shader.
	//m_GroundModel->RenderShadowShader(m_ShaderManager, DirectXPlatformManager::Instance().GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
	//	lightProjectionMatrix, m_RenderTexture->GetShaderResourceView(), m_PointLight->GetPosition(),
	//	m_PointLight->GetAmbientColor(), m_PointLight->GetDiffuseColor());
	//m_GroundModel->RenderLightShader(m_ShaderManager, DirectXPlatformManager::Instance().GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
	//	m_DirectionalLight->GetDirection(), m_DirectionalLight->GetAmbientColor(), m_DirectionalLight->GetDiffuseColor(),
	//	m_Camera->GetPosition(), m_DirectionalLight->GetSpecularColor(), m_DirectionalLight->GetSpecularPower());


	//DirectXPlatformManager::Instance().GetWorldMatrix(worldMatrix);
	//m_HumanModel->GetPosition(posX, posY, posZ);
	//D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);
	//m_HumanModel->RenderShadowShader(m_ShaderManager, DirectXPlatformManager::Instance().GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
	//	lightProjectionMatrix, m_RenderTexture->GetShaderResourceView(), m_PointLight->GetPosition(),
	//	m_PointLight->GetAmbientColor(), m_PointLight->GetDiffuseColor());
	//m_HumanModel->RenderLightShader(m_ShaderManager, DirectXPlatformManager::Instance().GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
	//	m_DirectionalLight->GetDirection(), m_DirectionalLight->GetAmbientColor(), m_DirectionalLight->GetDiffuseColor(),
	//	m_Camera->GetPosition(), m_DirectionalLight->GetSpecularColor(), m_DirectionalLight->GetSpecularPower());

	DirectXPlatformManager::Instance().TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	DirectXPlatformManager::Instance().TurnOnAlphaBlending();

	DirectXPlatformManager::Instance().GetWorldMatrix(worldMatrix);

	// Render the text strings.
	//result = m_Text->Render(DirectXPlatformManager::Instance().GetDeviceContext(), worldMatrix, orthoMatrix);
	//if(!result)
	//{
	//	return false;
	//}

	// Turn off alpha blending after rendering the text.
	DirectXPlatformManager::Instance().TurnOffAlphaBlending();

	DirectXPlatformManager::Instance().TurnZBufferOn();

	SimpleEngine::UI::GUIManager::Instance().OnRender();

	// Present the rendered scene to the screen.
	DirectXPlatformManager::Instance().EndScene();

	return true;
}
