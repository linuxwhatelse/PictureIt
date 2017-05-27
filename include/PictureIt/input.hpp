/*
 *	Copyright (C) 2016  Hannes Haberl
 *
 *	This file is part of GLMViz.
 *
 *	GLMViz is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	GLMViz is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with GLMViz.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "buffer.hpp"
#include <memory>

class Input {
	public:
		typedef std::unique_ptr<Input> Ptr;
		virtual ~Input() {};
		virtual bool is_open() const = 0;
		virtual void read(Buffer<int16_t>&) const = 0;
		virtual void read_stereo(Buffer<int16_t>&, Buffer<int16_t>&) const = 0;
};
