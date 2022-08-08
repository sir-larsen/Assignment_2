/**
 * @file IndexBuffer.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the IndexBuffer class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once

/**
 * @class IndexBuffer
 * @brief Boilerplate OpenGL code regarding Element Array Buffers
 */
class IndexBuffer
{
private:
	unsigned int renderer_ID;
	unsigned int m_count;
public:
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	void selectIndices(const unsigned int* data, unsigned int count);

	void deleteBuffer();
	inline unsigned int getCount() const { return m_count; }
};

