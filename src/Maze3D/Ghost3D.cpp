/**
 * @file Ghost3D.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source code for the Maze3D class
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Ghost3D.h"
#include <stack>


/**
 * @brief Construct a new Ghost 3D object
 * 
 * @param ghostModel 	- The model of the ghost
 * @param maze 			- The 3d Maze
 * @param ghostID 		- The Ghost's unique ID in the level 2d array
 * @param player 		- The camera controlled by the player
 * @param pathfinder 	- The pathfinder, Astar. 
 */
Ghost3D::Ghost3D(Model* ghostModel, Maze3D* maze, int ghostID, Camera* player,AStar* pathfinder)
	:	m_ID(ghostID),
		m_Player(player),
		m_Maze(maze),
		m_Ghost(ghostModel),
		m_Pathfinder(pathfinder),
		movementSpeed(1.f),
		rotationAngle(0.f),
		firstDirection(true),
		beenAdjusted(false),
		playerEaten(false)
{
	setSpawn();
}

/**
 * @brief Draws the Ghost3D
 * 
 * @param shader 		- The ghost's shader
 * @param projection 	- The players projection matrix
 * @param view 			- The players view matrix
 * @param dt 			- Delta time
 */
void Ghost3D::Draw(Shader* shader, glm::mat4 projection, glm::mat4 view, float dt, bool freecam)
{
	shader->use();
	shader->setMat4("u_ProjectionMat", projection);
	shader->setMat4("u_ViewMat", view);
	glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(posX + .5f, 0.5f, posY +.5f));
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(.3f));
	glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(rotationAngle), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 transformation = translation * rotation * scale;
	shader->setMat4("u_TransformationMat", transformation);

	Move(dt, freecam);

	m_Ghost->Draw(*shader);
}
/**
 * @brief 	Loops through the 2d map, finding the spawn location of the ghost 
 * 			based on its unique ID. 
 * 
 */
void Ghost3D::setSpawn()
{
	for (int y = 0; y < m_Maze->getHeight(); y++)
		for (int x = 0; x < m_Maze->getWidth(); x++)
			if (m_Maze->map2d[y][x] == m_ID)
			{
				posX = x;
				posY = y;
			}
}

/**
 * @brief 	Move the ghost, using the Astar class to calculate the shortest path
 * 			to the player
 * 
 * @param dt - Delta time
 */
void Ghost3D::Move(float dt, bool constrainMovement)
{
	if (constrainMovement)
	{
		Node ghost;
		ghost.y = floor(posY);
		ghost.x = floor(posX);

		Node player;
		player.y = floor(m_Player->Position.z);
		player.x = floor(m_Player->Position.x);

		std::vector<Node> path = m_Pathfinder->Pathfind(ghost, player);

		if (path.size() > 0) 
		{
			Direction oldDirection = direction;

			if ((floor(posY) - path[1].y) > 0)
					direction = North;
			else if ((floor(posY) - path[1].y) < 0)
					direction = South;
			else if ((floor(posX) - path[1].x) > 0)
					direction = East;
			else if ((floor(posX) - path[1].x) < 0)
					direction = West;
	
	

			switch (direction)
			{
			case North: Translate(North, dt); break;
			case South: Translate(South, dt);break;
			case East:  Translate(East, dt);break;
			case West:  Translate(West, dt);break;
			default:    break;
			}
		} else {
			playerEaten = true;
		}

	}
}

/**
 * @brief 	Translates (moves) the ghost according to the designated direction
 * 			the pathfinder has deemed fit. 
 * 
 * @param dir - The direction to be moved in
 * @param dt  - Delta time 
 */
void Ghost3D::Translate(Direction dir, float dt)
{
	float velocity = movementSpeed * dt;
	switch (dir)
	{
	case South: posY += velocity; posX = floor(posX); rotationAngle = 0.f;	 break;
	case West:  posX += velocity; posY = floor(posY); rotationAngle = 90.f;  break;
	case North: posY -= velocity; posX = floor(posX); rotationAngle = 180.f; break;
	case East:  posX -= velocity; posY = floor(posY); rotationAngle = 270.f; break;
	default:    break;
	}
}

