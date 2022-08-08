/**
 * @file Ghost.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the Ghost class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include "MovableObject.h"
#include "../Maze3D/Ghost3D.h"

/**
 * @brief Enum for determining what axis a ghost is currently moving in.
 * 
 */
/*
enum Axis
{
	vertical = 0,
	horizontal = 1
};*/

/**
 * @class Ghost : public MovableObject
 * @brief Extension of the MovableObject class, used for handling the games ghosts. 
 */
class Ghost : public MovableObject
{
public:
	Ghost(Maze3D* maze, Shader* shader, Renderer* renderer, const int ID, const std::string spritePaths, Ghost3D* ghost);

	virtual void setDirection();
	virtual void setPosition(float dt);
	//void randomize(Axis axis);
private:
	double	elapsedTime,
			prevPosX,
			prevPosY;
	int		dir;

	Ghost3D* m_Ghost;
};
