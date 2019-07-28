#ifndef UI_HORIZONTAL_LAYOUT_GROUP_H_
#define UI_HORIZONTAL_LAYOUT_GROUP_H_

#pragma once

#include <memory>
#include <vector>

#include <GUI/LayoutNode.h>

namespace SimpleEngine
{
	namespace UI
	{
		class HorizontalLayoutGroup : public LayoutNode
		{
		public:
			explicit HorizontalLayoutGroup(LayoutNode *parent = nullptr);
			virtual ~HorizontalLayoutGroup();

			void OnResize(float resize_scale) override;

			void SetLeftPadding(float left_padding);
			void SetItemSpacing(float spacing);

		protected:
			float left_padding_{ 0 };
			// 子控件的相距间隔
			int spacing_{ 0 };
		};
	}
}

#endif // UI_HORIZONTAL_LAYOUT_GROUP_H_