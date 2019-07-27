#ifndef UI_ExampleLayout_H_
#define UI_ExampleLayout_H_

#pragma once

#include <memory>

#include <GUI/BaseLayout.h>

namespace SimpleEngine
{
	namespace UI
	{
		class ExampleLayout : public BaseLayout
		{
		public:
			ExampleLayout(float scale_weight);
			virtual ~ExampleLayout() {}
		};
	}
}

#endif // !UI_ExampleLayout_H_