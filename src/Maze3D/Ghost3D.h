/**
 * @file Ghost3D.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the Ghost class
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include "../Core/AStar.h"
#include "../Core/model.h"

/**
 * @brief Enum for specifying what direction (to be moved in) / (are moving in)
 * 
 */
enum Direction {
	North = 0,
	South = 1,
	East  = 2,
	West  = 3
};

/**
 * @class Ghost3D 
 * @brief The code for handling everyting related to the 3d Ghosts. 
 */
class Ghost3D
{
public:
	Ghost3D(Model* ghostModel, Maze3D* maze, int ghostID, Camera* player, AStar* pathfinder);

	void Draw(Shader* shader, glm::mat4 projection, glm::mat4 view, float dt, bool constrainMovement);
	
	float posX, posY;
	Direction direction;
	bool playerEaten;
private:
	int m_ID;
	float movementSpeed;
	float rotationAngle;
	bool  firstDirection, beenAdjusted;
	Camera* m_Player;
	Maze3D* m_Maze;
	Model*	m_Ghost;


	void setSpawn();

	AStar* m_Pathfinder;
	void Move(float dt, bool constrainMovement);
	void Translate(Direction dir, float dt);
};