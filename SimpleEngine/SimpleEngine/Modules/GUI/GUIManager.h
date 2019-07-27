#ifndef UI_GUI_MANAGER_H_
#define UI_GUI_MANAGER_H_

#pragma once

#include <vector>
#include <memory>

#include <GUI/UICommon.h>

class Camera;

namespace SimpleEngine
{
	namespace UI
	{
		class BaseLayout;

		class GUIManager
		{
		public:
			static GUIManager& Instance()
			{
				static GUIManager instance;
				return instance;
			}
			GUIManager(GUIManager const&) = delete;
			void operator=(GUIManager const&) = delete;

			void Initialize();

			// 实际窗口的宽 / 标准分辨率的宽
			float GetWidthResizeScale();
			// 实际窗口的高 / 标准分辨率的高
			float GetHeightResizeScale();

			void SetStandardWindowSize(unsigned int width, unsigned int height)
			{
				standard_window_size_.width = width;
				standard_window_size_.height = height;
			}

			std::shared_ptr<Camera> GetCamera() { return camera_; }

			void OnWindowResized();
			void OnRender();

			void RegisterLayout(std::shared_ptr<BaseLayout> layout);

		private:
			GUIManager() { }

		private:
			std::shared_ptr<Camera> camera_;
			// 界面设计时所参考的的标准窗口大小
			Size<int> standard_window_size_;
			std::vector<std::shared_ptr<BaseLayout>> layout_list_;
		};
	}
}

#endif // !UI_GUI_MANAGER_H_