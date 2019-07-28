#include <GUI/HorizontalLayoutGroup.h>
#include <NativePlatform/NativePlatform.h>

namespace SimpleEngine
{
	namespace UI
	{
		HorizontalLayoutGroup::HorizontalLayoutGroup(LayoutNode *parent) :
			LayoutNode(parent)
		{
		}

		HorizontalLayoutGroup::~HorizontalLayoutGroup()
		{

		}

		void HorizontalLayoutGroup::OnResize(float resize_scale)
		{
			// ȡ���ؼ��Ĵ�С
			Size<int> parent_size;
			if (node_parent_)
			{
				parent_size = node_parent_->GetSize();
			}
			else
			{
				parent_size = Size<int>(NativePlatform::Instance().GetScreenWidth(), NativePlatform::Instance().GetScreenHeight());
			}

			relative_coord_.left = parent_size.width * anchor_min_.left + anchor_offset_.left * resize_scale;
			relative_coord_.top = parent_size.height * anchor_min_.top + anchor_offset_.top * resize_scale;
			relative_coord_.width = parent_size.width * anchor_max_.left + anchor_offset_.right * resize_scale - relative_coord_.left;
			relative_coord_.height = parent_size.height * anchor_max_.top + anchor_offset_.bottom * resize_scale - relative_coord_.top;

			int child_left = left_padding_ * resize_scale;
			for (size_t i = 0; i < node_children_.size(); i++)
			{
				node_children_[i]->OnResize(resize_scale);

				Size<int> item_size = node_children_[i]->GetSize();
				int child_top = (GetSize().height - item_size.height) / 2;
				node_children_[i]->SetRelativePosition(Point<int>(child_left, child_top));

				child_left += item_size.width + spacing_ * resize_scale;
			}
		}

		void HorizontalLayoutGroup::SetLeftPadding(float left_padding)
		{
			left_padding_ = left_padding;
		}

		void HorizontalLayoutGroup::SetItemSpacing(float spacing)
		{
			spacing_ = spacing;
		}
	}
}