#ifndef Struct_h__
#define Struct_h__

#include "Define.h"

namespace Client
{
	typedef struct colorVertex {
		colorVertex() {}

		colorVertex(_float x, _float y, _float z, _color color) :
			m_x(x), m_y(y), m_z(z), m_color(color) {}

		_float m_x;
		_float m_y;
		_float m_z;
		_color m_color;
	}ColorVertex;
}

#endif