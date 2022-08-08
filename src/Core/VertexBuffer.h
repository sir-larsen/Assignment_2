/**
 * @file VertexBuffer.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The header file for the VertexBuffer class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
/**
 * @class VertexBuffer
 * @brief Boilerplate OpenGL code handling VertexBuffers
 */
class VertexBuffer
{
private:
	unsigned int renderer_ID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void updateBuffer(const void* data, unsigned int size);
};

