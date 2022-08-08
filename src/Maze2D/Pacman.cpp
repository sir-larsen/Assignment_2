/**
 * @file Pacman.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The source code for the Pacman class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Pacman.h"
#include "MovableObject.h"
#include <iostream>

/**
 * @brief Construct a new Pacman:: Pacman object
 * 
 * @param maze 			- The maze Pacman is to be drawn "in"
 * @param shader 		- Pacman's shader
 * @param renderer 		- Pacman's renderer
 * @param ID 			- Pacman's identifier
 * @param window 		- The window that is going to be used for input detection later.
 * @param spritePaths 	- The path to the file containing the filepaths of the rest of the sprites.
 * @param ghosts		- vector containing all ghosts, used for collision detection
 */
Pacman::Pacman(Maze3D* maze, Shader* shader, Renderer* renderer, const int ID, const std::string spritePaths, std::vector <MovableObject*> ghosts, Camera* player)
	:	MovableObject(maze, shader, renderer, ID, spritePaths),
		m_Ghosts(ghosts),
		m_Player(player)
{
}

/**
 * @brief Destroy the Pacman:: Pacman object
 * 
 */
Pacman::~Pacman()
{
}

/**
 * @brief	Sets the direction pacman is facing based on the player's (Camera) 
 *			currently facing direction.
 * 
 */
void Pacman::setDirection()
{
	if (abs(m_Player->Front.x) > abs(m_Player->Front.z)) {
		if (m_Player->Front.x > 0.f && direction != MovableObject::Direction2D::right) {
			direction = MovableObject::Direction2D::right;
			newDirection = true;
		}
		if (m_Player->Front.x < 0.f && direction != MovableObject::Direction2D::left) {
			direction = MovableObject::Direction2D::left;
			newDirection = true;
		}
	} else {
		if (m_Player->Front.z > 0.f && direction != MovableObject::Direction2D::down) {
			direction = MovableObject::Direction2D::down;
			newDirection = true;
		}
		if (m_Player->Front.z < 0.f && direction != MovableObject::Direction2D::up) {
			direction = MovableObject::Direction2D::up;
			newDirection = true;
		}
	}
}

/**
 * @brief Sets pacmans position based on the players position in the 3d maze
 * 
 * @param dt 	- Delta time
 * @see			- setDirection();
 */
void Pacman::setPosition(float dt)
{
	this->setDirection();

	switch (direction)
	{
	case MovableObject::Direction2D::up:	posX = floor(m_Player->Position.x); posY = m_Player->Position.z -0.5f;	break;
	case MovableObject::Direction2D::down:	posX = floor(m_Player->Position.x); posY = m_Player->Position.z -0.5f;	break;
	case MovableObject::Direction2D::right:	posX = m_Player->Position.x-.5f; posY = floor(m_Player->Position.z);	break;
	case MovableObject::Direction2D::left:	posX = m_Player->Position.x-.5f; posY = floor(m_Player->Position.z);	break;
	default: break;
	}

	MovableObject::setPosition(dt);
}
