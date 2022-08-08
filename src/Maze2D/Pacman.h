/**
 * @file Pacman.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The header file for the Pacman class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include "MovableObject.h"
#include <vector>

/**
 * @class Pacman : Public MovableObject
 * @brief The class handling everything related to the player (pacman)
 */
class Pacman : public MovableObject
{
private:
	Camera* m_Player;
	std::vector <MovableObject*> m_Ghosts;
public:
	Pacman(Maze3D* maze, Shader* shader, Renderer* renderer, const int ID, 
		   const std::string spritePaths, std::vector <MovableObject*> ghosts, Camera* player);
	~Pacman();

	virtual void setDirection();
	virtual void setPosition(float dt);
};

