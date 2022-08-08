/**
 * @file IndexBuffer.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source code for the IndexBuffer class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "IndexBuffer.h"
#include <iostream>

#include <GL/glew.h>

/**
 * @brief Construct a new Index Buffer:: Index Buffer object
 * 
 * @param data 	- The data to be sent to the buffer
 * @param count - The amount of elements, not the total size in bytes.
 */
IndexBuffer::IndexBuffer(const void* data, unsigned int count)
	: m_count(count)
{
	glGenBuffers(1, &renderer_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

/**
 * @brief Destroy the Index Buffer:: Index Buffer object
 * 
 */
IndexBuffer::~IndexBuffer()
{
	deleteBuffer();
}

/**
 * @brief Binds the buffer with ID renderer_ID to GL_ELEMENT_ARRAY_BUFFER
 * 
 */
void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_ID);
}

/**
 * @brief Unbinds the buffer with ID renderer_ID
 * 
 */
void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/**
 * @brief Updates the buffer with a new set of data
 * 
 * @param data	- The data to be sent to the buffer
 * @param count - The amount of elements, not the total size in bytes. 
 */
void IndexBuffer::selectIndices(const unsigned int* data, unsigned int count)
{
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

/**
 * @brief Deletes the buffer with ID renderer_ID
 * 
 */
void IndexBuffer::deleteBuffer()
{
	glDeleteBuffers(1, &renderer_ID);
}
