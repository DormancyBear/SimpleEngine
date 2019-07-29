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
			T x, y;

			Point() : x(0), y(0) {}

			Point(T x, T y) : x(x), y(y) {}

			Point operator+(Point const& other) const
			{
				return Point(x + other.x, y + other.y);
			}

			Point& operator+=(Point const & other)
			{
				x += other.x;
				y += other.y;
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
			T left, bottom, width, height;

			Coord() : left(0), bottom(0), width(0), height(0) {}

			Coord(T left, T bottom, T width, T height) : left(left), bottom(bottom), width(width), height(height) {}

			bool operator==(Coord const & other) const
			{
				return ((left == other.left) && (bottom == other.bottom) && (width == other.width) && (height == other.height));
			}

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

			bool operator==(Rect const & other) const
			{
				return ((left == other.left) && (top == other.top) && (right == other.right) && (bottom == other.bottom));
			}
		};
	}
}

#endif // !UI_COMMON_H_