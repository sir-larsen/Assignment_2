/**
 * @file Framebuffer.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the boilerplate Framebuffer class
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include <GL/glew.h>

/**
 * @class Framebuffer
 * @brief Boilerplate code for handling OpenGl framebuffers
 */
class Framebuffer
{
private:
	unsigned int m_RendererID;
public:
	Framebuffer();
	~Framebuffer();

	void Bind();
	void Unbind();
	void addRenderBuffer(unsigned int rb_ID);
	inline unsigned int getID() { return m_RendererID; }
};