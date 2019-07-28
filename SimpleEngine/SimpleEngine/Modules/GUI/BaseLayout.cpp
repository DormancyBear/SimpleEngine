#include <GUI/BaseLayout.h>
#include <GUI/LayoutNode.h>
#include <GUI/GUIManager.h>

namespace SimpleEngine
{
	namespace UI
	{
		BaseLayout::BaseLayout(float scale_weight) :
			scale_weight_(scale_weight)
		{
		}

		BaseLayout::~BaseLayout()
		{
			if (root_node_)
			{
				delete root_node_;
				root_node_ = nullptr;
			}
		}

		// 界面控件缩放比例 = 实际屏幕宽缩放比 * (1-scale_weight_) + 实际屏幕高缩放比 * scale_weight_
		// scale_weight_ == 0 时表示完全以宽度为基准进行适配,
		// scale_weight_ == 1 时表示完全以高度为基准进行适配
		float BaseLayout::GetResizeScale()
		{
			return GUIManager::Instance().GetWidthResizeScale() * (1 - scale_weight_) +
				GUIManager::Instance().GetHeightResizeScale() * scale_weight_;
		}

		void BaseLayout::RegisterRootNode(LayoutNode *node)
		{
			root_node_ = node;
		}

		// 注册到 GUIManager 的时候进行一次初始化
		void BaseLayout::OnRegister()
		{
			OnResize();
		}

		void BaseLayout::OnResize()
		{
			root_node_->OnResize(GetResizeScale());
		}

		void BaseLayout::OnRender()
		{
			root_node_->OnRender();
		}
	}
}