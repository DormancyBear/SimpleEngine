#ifndef UI_COMMON_H_
#define UI_COMMON_H_

#pragma once

namespace SimpleEngine
{
	namespace UI
	{
		template<typename T>
		struct Point
		{
			T left, top;

			Point() : left(0), top(0) {}

			Point(T left, T top) : left(left), top(top) {}

			Point operator+(Point const& other) const
			{
				return Point(left + other.left, top + other.top);
			}

			Point& operator+=(Point other)
			{
				left += other.left;
				top += other.top;
				return *this;
			}
		};

		template<typename T>
		struct Size
		{
			T width, height;

			Size() : width(0), height(0) {}

			Size(T width, T height) : width(width), height(height) {}
		};

		template<typename T>
		struct Coord
		{
			T left, top, width, height;

			Coord() : left(0), top(0), width(0), height(0) {}

			Coord(T left, T top, T width, T height) : left(left), top(top), width(width), height(height) {}

			Size<T> GetSize()
			{
				return Size<T>(width, height);
			}
		};

		template<typename T>
		struct Rect
		{
			T left, top, right, bottom;

			Rect() : left(0), top(0), right(0), bottom(0) {}

			Rect(T left, T top, T right, T bottom) : left(left), top(top), right(right), bottom(bottom) {}

			Point<T> LeftTop() const
			{
				return Point<T>(left, top);
			}
		};
	}
}

#endif // !UI_COMMON_H_