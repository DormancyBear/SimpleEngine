#ifndef UI_BASELAYOUT_H_
#define UI_BASELAYOUT_H_

#pragma once

#include <memory>

namespace SimpleEngine
{
	namespace UI
	{
		class LayoutNode;

		// 一个独立的界面 (比如商店界面、背包界面等)
		class BaseLayout
		{
		public:
			BaseLayout(float scale_weight);
			virtual ~BaseLayout();

			float GetResizeScale();

			void OnRegister();
			void OnResize();
			void OnRender();

		protected:
			void RegisterRootNode(LayoutNode *node);

		private:
			// 所属于同一个界面的 widget 必须等比缩放才能保证不会变形
			float scale_weight_{ 1 };
			// 根控件(没有父控件的节点), 一个 BaseLayout = 一棵树, 对应一个根控件及其底下所有的子控件
			LayoutNode *root_node_{ nullptr };
		};
	}
}

#endif // !UI_BASELAYOUT_H_