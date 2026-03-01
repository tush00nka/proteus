#include "data_pool.h"
#include "vector4.h"

DataPool::DataPool()
{
	this->_queue.emplace();
}

size_t DataPool::size()
{
	return this->_queue.size();
}

void DataPool::push(const Vector4& vec)
{
	this->_queue.push(vec);
}

void DataPool::pop()
{
	this->_queue.pop();
}

const Vector4& DataPool::front() const
{
	return this->_queue.front();
}

Vector4& DataPool::frontMut()
{
	return this->_queue.front();
}