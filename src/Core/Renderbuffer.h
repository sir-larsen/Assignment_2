/**
 * @file Renderbuffer.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the renderbuffer class
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include <GL/glew.h>

class Renderbuffer
{
private:
	unsigned int m_RendererID;
public:
	Renderbuffer();
	~Renderbuffer();

	void Bind();
	void Unbind();
	void setStorage(int size);
	inline unsigned int getID() { return m_RendererID; }
};