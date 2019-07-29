#ifndef UI_SKIN_H_
#define UI_SKIN_H_

#pragma once

#include <GUI/UICommon.h>

namespace SimpleEngine
{
	namespace UI
	{
		// 控件的皮肤, 负责一个控件的渲染
		class ISkin
		{
		public:
			ISkin() {}
			virtual ~ISkin() {}

			virtual void DoRender(Rect<int> bounds) = 0;
		};
	}
}

#endif // !UI_SKIN_H_