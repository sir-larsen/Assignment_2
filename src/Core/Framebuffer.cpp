/**
 * @file Framebuffer.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Boilerplate code for handling OpenGL regarding framebuffers. 
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Framebuffer.h"

/**
 * @brief Construct a new Framebuffer:: Framebuffer object
 * 
 */
Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &m_RendererID);
	Bind();
}

/**
 * @brief Destroy the Framebuffer:: Framebuffer object
 * 
 */
Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
}

/**
 * @brief Binds the framebuffer.
 * 
 */
void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,m_RendererID);
}

/**
 * @brief Unbinds the framebuffer, making the default active. 
 * 
 */
void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief Add a renderbuffer to the framebuffer
 * 
 * @param rb_ID - The id to the renderbuffer to be added. 
 */
void Framebuffer::addRenderBuffer(unsigned int rb_ID)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb_ID);
}
