/**
 * @file Texture.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The header file for the Texture class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include <string>

/**
 * @class Texture
 * @brief Boilerplate OpenGL code for handling texture. 
 */
class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& filepath);
	Texture();
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }
};


