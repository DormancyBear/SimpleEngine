#include "GUIManager.h"
#include <Graphics/DirectXPlatformManager.h>
#include <GUI/ExampleLayout.h>
#include <NativePlatform/NativePlatform.h>

namespace SimpleEngine
{
	namespace UI
	{
		void GUIManager::Initialize()
		{
			D3DXMatrixTranslation(&world_matrix_, -static_cast<float>(NativePlatform::Instance().GetScreenWidth()) / 2,
				-static_cast<float>(NativePlatform::Instance().GetScreenHeight()) / 2, 0);

			camera_ = std::make_shared<Camera>();
			camera_->SetPosition(0.0f, 0.0f, -100.0f);
			camera_->Render();

			GUIManager::Instance().SetStandardWindowSize(1920, 1080);

			std::shared_ptr<BaseLayout> layout_a = std::make_shared<ExampleLayout>(1);
			RegisterLayout(layout_a);
		}

		float GUIManager::GetWidthResizeScale()
		{
			return static_cast<float>(NativePlatform::Instance().GetScreenWidth()) / standard_window_size_.width;
		}

		float GUIManager::GetHeightResizeScale()
		{
			return static_cast<float>(NativePlatform::Instance().GetScreenHeight()) / standard_window_size_.height;
		}

		void GUIManager::GetWorldMatrix(D3DXMATRIX & world_matrix)
		{
			world_matrix = world_matrix_;
		}

		void GUIManager::OnWindowResized()
		{
			D3DXMatrixTranslation(&world_matrix_,
				-static_cast<float>(NativePlatform::Instance().GetScreenWidth()) / 2,
				-static_cast<float>(NativePlatform::Instance().GetScreenHeight()) / 2,
				0);

			for (auto layout : layout_list_)
			{
				layout->OnResize();
			}
		}

		void GUIManager::OnRender()
		{
			DirectXPlatformManager::Instance().TurnZBufferOff();

			for (auto layout : layout_list_)
			{
				layout->OnRender();
			}

			DirectXPlatformManager::Instance().TurnZBufferOn();
		}

		void GUIManager::RegisterLayout(std::shared_ptr<BaseLayout> layout)
		{
			layout_list_.push_back(layout);
			layout->OnRegister();
		}
	}
}