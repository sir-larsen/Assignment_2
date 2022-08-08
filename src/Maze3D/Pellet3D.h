/**
 * @file Pellet3D.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The header file for the Pellet3D class
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include <GL/glew.h>
#include "Ghost3D.h"

/**
 * @class Pellet3D
 * @brief Will handle the creation of, and drawing of the 3d pellets. 
 */
class Pellet3D
{
public:
	Pellet3D(Model* pellet, Maze3D* maze);

	void Draw(Shader* shader, glm::mat4 projection, glm::mat4 view);
	void eatPellet(Camera* camera, Maze3D* maze);
	bool allEaten;
	int pelletCount;
private:
	Model* pellet;
	unsigned int VAO, VBO;
	std::vector <glm::mat4> modelMatrices;

	void generateModelMatrices(Maze3D* maze);
	void addModelMatrices();
};