/**
 * @file VertexBuffer.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source file for the VertexBuffer class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "VertexBuffer.h"


#include <GL/glew.h>

/**
 * @brief Construct a new Vertex Buffer:: Vertex Buffer object
 * 
 * @param data - The data that is to be put into the Vertex Buffer
 * @param size - The actual size of the data in bytes. 
 */
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &renderer_ID); //generating a buffer for the vertex buffer object
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

/**
 * @brief Destroy the Vertex Buffer:: Vertex Buffer object
 * 
 */
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &renderer_ID);
}

/**
 * @brief Binds the VertexBuffer.
 * 
 */
void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, renderer_ID);
}

/**
 * @brief Unbinds the VertexBuffer
 * 
 */
void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::updateBuffer(const void* data, unsigned int size)
{
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}