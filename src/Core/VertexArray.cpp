/**
 * @file VertexArray.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The source file for the VertexArray class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "VertexArray.h"

/**
 * @brief Construct a new Vertex Array:: Vertex Array object
 * 
 */
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &rendererID); //Generating a vertex array for the array object
	Bind();
}

/**
 * @brief Destroy the Vertex Array:: Vertex Array object
 * 
 */
VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &rendererID);
}

/**
 * @brief Combines a VertexBuffer and a VertexBufferLayout and adds it to the VertexArray
 * 
 * @param vb 		- VertexBuffer containing the data
 * @param layout 	- VertexBufferaLayout containing the information as to how the information is to be interpreted. 
 */
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized,
			layout.getStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

/**
 * @brief Binds the VertexArray
 * 
 */
void VertexArray::Bind() const
{
	glBindVertexArray(rendererID);
}

/**
 * @brief Unbinds the VertexArray
 * 
 */
void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

/**
 * @brief Changes the data in this VertexArray object
 * 
 * @param VBO - The vbo holding the data
 * @param data - The new data
 * @param size  - The size in bytes of the new data
 */
void VertexArray::changeData(VertexBuffer* VBO,const void* data, unsigned int size)
{
	Bind();
	VBO->Bind();
	VBO->updateBuffer(data, size);
}
