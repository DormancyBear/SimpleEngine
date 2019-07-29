#include <GUI/ExampleLayout.h>
#include <GUI/Image.h>
#include <GUI/HorizontalLayoutGroup.h>

namespace SimpleEngine
{
	namespace UI
	{
		ExampleLayout::ExampleLayout(float scale_weight) :
			BaseLayout(scale_weight)
		{
			LayoutNode *widget_a = new LayoutNode();
			RegisterRootNode(widget_a);
			std::shared_ptr<Image> image_a = std::make_shared<Image>(L"./media/Textures/3.jpg");
			widget_a->SetSkin(image_a);
			// 让 widget_a 不管在什么分辨率下都覆盖全屏
			widget_a->SetCoord(Rect<int>(0, 0, 0, 0), Point<float>(0, 0), Point<float>(1.0f, 1.0f));

			LayoutNode *widget_b = new LayoutNode(widget_a);
			std::shared_ptr<Image> image_b = std::make_shared<Image>(L"./media/Textures/2.jpg");
			widget_b->SetSkin(image_b);
			// 让 widget_b 始终靠在窗口右上角, 其大小一直跟着窗口等比缩放
			widget_b->SetCoord(Rect<int>(-254, 0, 0, -300), Point<float>(1.0f, 1.0f), Point<float>(1.0f, 1.0f));

			HorizontalLayoutGroup *widget_c = new HorizontalLayoutGroup(widget_a);
			widget_c->SetItemSpacing(10);
			widget_c->SetCoord(Rect<int>(0, 100, 0, -100), Point<float>(0.0f, 0.5f), Point<float>(1.0f, 0.5f));

			LayoutNode *widget_d = new LayoutNode(widget_c);
			std::shared_ptr<Image> image_d = std::make_shared<Image>(L"./media/Textures/1.jpg");
			widget_d->SetSkin(image_d);
			widget_d->SetCoord(Rect<int>(-128, 0, 0, -238), Point<float>(1.0f, 1.0f), Point<float>(1.0f, 1.0f));

			LayoutNode *widget_e = new LayoutNode(widget_c);
			std::shared_ptr<Image> image_e = std::make_shared<Image>(L"./media/Textures/0.jpg");
			widget_e->SetSkin(image_e);
			widget_e->SetCoord(Rect<int>(-72, 0, 0, -100), Point<float>(1.0f, 1.0f), Point<float>(1.0f, 1.0f));
		}
	}
}