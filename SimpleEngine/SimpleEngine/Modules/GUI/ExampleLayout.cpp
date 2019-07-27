#include <GUI/ExampleLayout.h>
#include <GUI/Image.h>
#include <GUI/Widget.h>

namespace SimpleEngine
{
	namespace UI
	{
		ExampleLayout::ExampleLayout(float scale_weight) :
			BaseLayout(scale_weight)
		{
			LayoutNode *widget_a = new LayoutNode();
			RegisterRootNode(widget_a);
			std::shared_ptr<Image> image_a = std::make_shared<Image>(L"./media/Textures/4.jpg");
			widget_a->SetSkin(image_a);
			// 让 widget_a 不管在什么分辨率下都覆盖全屏
			widget_a->SetCoord(Rect<int>(0, 0, 0, 0), Point<float>(0, 0), Point<float>(1.0f, 1.0f));

			LayoutNode *widget_c = new LayoutNode(widget_a);
			std::shared_ptr<Image> image_c = std::make_shared<Image>(L"./media/Textures/2.jpg");
			widget_c->SetSkin(image_c);
			// 让 widget_c 始终靠在窗口右下角, 其大小一直跟着窗口等比缩放
			widget_c->SetCoord(Rect<int>(-718, -1000, 0, 0), Point<float>(1.0f, 1.0f), Point<float>(1.0f, 1.0f));
		}
	}
}