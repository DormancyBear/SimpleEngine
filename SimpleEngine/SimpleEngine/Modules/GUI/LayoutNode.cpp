#include <iostream>

#include <GUI/LayoutNode.h>
#include <GUI/Skin.h>
#include <GUI/BaseLayout.h>
#include <NativePlatform/NativePlatform.h>
#include <GUI/GUIManager.h>

namespace SimpleEngine
{
	namespace UI
	{
		LayoutNode::LayoutNode(LayoutNode *parent) :
			node_parent_(parent)
		{
			if (node_parent_)
			{
				node_parent_->AddChild(this);
			}
		}

		LayoutNode::~LayoutNode()
		{
			for (size_t i = 0; i < node_children_.size(); i++)
			{
				delete node_children_[i];
				node_children_[i] = nullptr;
			}
		}

		bool LayoutNode::IsRootWidget()
		{
			return nullptr == node_parent_;
		}

		void LayoutNode::AddChild(LayoutNode *child)
		{
			node_children_.push_back(child);
		}

		void LayoutNode::OnResize(float resize_scale)
		{
			Size<int> parent_size;
			if (node_parent_)
			{
				parent_size = node_parent_->GetSize();
			}
			else
			{
				parent_size = Size<int>(NativePlatform::Instance().GetScreenWidth(),
					NativePlatform::Instance().GetScreenHeight());
			}

			relative_coord_.left = parent_size.width * anchor_min_.x
				+ anchor_offset_.left * resize_scale;

			relative_coord_.bottom = parent_size.height * anchor_min_.y
				+ anchor_offset_.bottom * resize_scale;

			relative_coord_.width = parent_size.width * anchor_max_.x
				+ anchor_offset_.right * resize_scale - relative_coord_.left;

			relative_coord_.height = parent_size.height * anchor_max_.y
				+ anchor_offset_.top * resize_scale - relative_coord_.bottom;

			for (auto widget : node_children_)
			{
				if (widget)
				{
					widget->OnResize(resize_scale);
				}
			}
		}

		void LayoutNode::OnRender(Point<int> parent_absolute_position)
		{
			// 虽然 UI 系统内部计算时用的是相对坐标, 但渲染时还是要换算成绝对坐标
			Point<int> absolute_position = parent_absolute_position;
			absolute_position += Point<int>(relative_coord_.left, relative_coord_.bottom);

			// 如果 owned_skin_ 为空, 就表示这个布局对象仅仅是用来辅助调整尺寸位置的, 并不参与渲染
			if (owned_skin_)
			{
				owned_skin_->DoRender(Rect<int>(
					absolute_position.x,
					absolute_position.y + relative_coord_.height,
					absolute_position.x + relative_coord_.width,
					absolute_position.y));
			}

			for (auto node : node_children_)
			{
				node->OnRender(absolute_position);
			}
		}

		void LayoutNode::SetRelativePosition(Point<int> position)
		{
			relative_coord_.left = position.x;
			relative_coord_.bottom = position.y;
		}

		void LayoutNode::SetCoord(Rect<int> anchor_offset, Point<float> anchor_min, Point<float> anchor_max)
		{
			anchor_offset_ = anchor_offset;
			anchor_min_ = anchor_min;
			anchor_max_ = anchor_max;
		}

		void LayoutNode::SetSkin(std::shared_ptr<ISkin> skin)
		{
			owned_skin_ = skin;
		}
	}
}