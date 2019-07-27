#ifndef UI_WIDGET_H_
#define UI_WIDGET_H_

#pragma once

#include <memory>
#include <vector>

#include <GUI/UICommon.h>

namespace SimpleEngine
{
	namespace UI
	{
		class BaseLayout;
		class ISkin;

		// 布局对象, 负责管理控件的位置坐标和尺寸大小
		// 建立一个 UI 坐标系: 以屏幕左上角为原点, X轴向右为正方向, Y轴向下为正方向
		// 布局对象的相对坐标: 以 LayoutNode 左上角为基准点进行比较
		class LayoutNode
		{
		public:
			explicit LayoutNode(LayoutNode *parent = nullptr);
			virtual ~LayoutNode();

			bool IsRootWidget();
			void AddChild(LayoutNode *child);

			virtual void OnResize(float resize_scale);
			virtual void OnRender(Point<int> parent_absolute_position = Point<int>());

			Size<int> GetSize()
			{
				return relative_coord_.GetSize();
			}

			// 这些 public API 都是按照界面设计时的标准分辨率来设置控件的尺寸, 至于分辨率适配后的实际坐标, 对于 API 调用者是透明的
			void SetCoord(Rect<int> anchor_offset,
				Point<float> anchor_min = Point<float>(0.5, 0.5), Point<float> anchor_max = Point<float>(0.5, 0.5));

			std::shared_ptr<ISkin> GetSkin() { return owned_skin_; }

			void SetSkin(std::shared_ptr<ISkin> skin);

		protected:
			// left, top 的锚点 [0, 1]
			Point<float> anchor_min_{ 0.5,0.5 };
			// right, bottom 的锚点 [0, 1]
			Point<float> anchor_max_{ 0.5,0.5 };
			// 界面设计时矩形框的四边相对于锚点的距离
			Rect<int> anchor_offset_;
			// 相对于父控件左上角的尺寸坐标 (分辨率适配后的结果)
			Coord<int> relative_coord_;

			LayoutNode *node_parent_{ nullptr };
			std::vector<LayoutNode*> node_children_;
			std::shared_ptr<ISkin> owned_skin_;
		};
	}
}

#endif // UI_WIDGET_H_