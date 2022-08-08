/**
 * @file Ghost.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source code for the Ghost class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "Ghost.h"
#include "GLFW/glfw3.h"

#include <iostream>

/**
 * @brief Construct a new Ghost:: Ghost object
 * 
 * @param maze 			- The maze the Ghost is "in"
 * @param shader 		- The Ghost's shader
 * @param renderer 		- The Ghost's renderer
 * @param ID 			- The Ghost's identifier in the level
 * @param spritePaths 	- The path to the file containing the paths to the rest of the sprites.
 */
Ghost::Ghost(Maze3D* maze, Shader* shader, Renderer* renderer, const int ID,
			 const std::string spritePaths, Ghost3D* ghost)
	:	MovableObject(maze,shader,renderer,ID,spritePaths),
		m_Ghost(ghost)
{
	elapsedTime = 0;
	dir = 3;
}

/**
 * @brief Sets a direction based on the corresponding 3d ghost's direction
 */
void Ghost::setDirection()
{
	if (m_Ghost->direction == North && direction != MovableObject::Direction2D::up) {
		direction = MovableObject::Direction2D::up;
		newDirection = true;
	}
	if (m_Ghost->direction == South && direction != MovableObject::Direction2D::down) {
		direction = MovableObject::Direction2D::down;
		newDirection = true;
	}
	if (m_Ghost->direction == West && direction != MovableObject::Direction2D::right) {
		direction = MovableObject::Direction2D::right;
		newDirection = true;
	}
	if (m_Ghost->direction == East && direction != MovableObject::Direction2D::left) {
		direction = MovableObject::Direction2D::left;
		newDirection = true;
	}
}

/**
 * @brief Sets a new position on the minimap based on the corresponding 3d ghost's position
 * 
 * @param dt 
 */
void Ghost::setPosition(float dt)
{
	this->setDirection();

	switch (direction)
	{
	case MovableObject::Direction2D::up:	posX = floor(m_Ghost->posX); posY = m_Ghost->posY;	break;
	case MovableObject::Direction2D::down:	posX = floor(m_Ghost->posX); posY = m_Ghost->posY;	break;
	case MovableObject::Direction2D::right:	posX = m_Ghost->posX; posY = floor(m_Ghost->posY);	break;
	case MovableObject::Direction2D::left:	posX = m_Ghost->posX; posY = floor(m_Ghost->posY);	break;
	default: break;
	}

	MovableObject::setPosition(dt);
}