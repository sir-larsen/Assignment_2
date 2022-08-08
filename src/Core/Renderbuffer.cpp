/**
 * @file Renderbuffer.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source file for the Renderbuffer boilerplate OpenGL class
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Renderbuffer.h"

#include <iostream>

/**
 * @brief Construct a new Renderbuffer:: Renderbuffer object
 * 
 */
Renderbuffer::Renderbuffer()
{
	glGenRenderbuffers(1,&m_RendererID);
	Bind();
}

/**
 * @brief Destroy the Renderbuffer:: Renderbuffer object
 * 
 */
Renderbuffer::~Renderbuffer()
{
	glDeleteRenderbuffers(1, &m_RendererID);
}

/**
 * @brief Binds this renderbuffer object
 * 
 */
void Renderbuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
}

/**
 * @brief Unbinds this renderbuffer object
 * 
 */
void Renderbuffer::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/**
 * @brief Sets a ned renderbuffer storage
 * 
 * @param size - The size of which the storage is going to be. 
 */
void Renderbuffer::setStorage(int size)
{
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size, size);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}
