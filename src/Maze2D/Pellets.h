/**
 * @file Pellets.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The header file for the Pellets class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include "../Maze3D/Maze3D.h"
#include "Pacman.h"
#include "../Core/Texture.h"


/**
 * @class Pellets
 * @brief The class handling everything related to pellets. 
 */
class Pellets
{
private:
	int  remainingPellets;
	bool allPelletsEaten;
	std::vector <unsigned int>	pelletsIndices;
	std::vector <glm::vec3>		pelletVertices;
	
	Maze3D*				m_Maze;
	Renderer*			m_Renderer;
	Shader*				m_Shader;

	VertexArray*		pelletsVAO;
	VertexBuffer*		pelletsVBO;
	VertexBufferLayout* pelletsVBLayout;
	IndexBuffer*	    pelletsIBO;
	Texture*			pelletsTexture;

	/*Camera*				m_Player;*/
public:
	Pellets(Maze3D* maze, Shader* shader, Renderer* renderer/*, Camera* player*/);
	~Pellets();

	void generatePellets();
	void makePelletsIndices();
	void draw();
	void camera(int hSize, int vSize);
	void makeVertices();
	bool allPelletsGone() { return allPelletsEaten; }
	int  getScore() { return remainingPellets; }
private:
	void hasBeenEaten();
};
