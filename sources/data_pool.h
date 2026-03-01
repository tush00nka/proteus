#pragma once

#include "vector4.h"
#include <queue>


class DataPool {
private:
	std::queue<Vector4> _queue;
public:
	DataPool();
	size_t size();
	void push(const Vector4& vec4);
	void pop();
	[[nodiscard]] const Vector4& front() const;
	Vector4& frontMut();
};	