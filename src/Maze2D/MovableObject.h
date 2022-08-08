/**
 * @file MovableObject.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the MovableObject class 
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once
#include "../Maze3D/Maze3D.h"
#include "../Core/VertexArray.h"
#include "../Core/VertexBuffer.h"
#include "../Core/VertexBufferLayout.h"
#include "../Core/IndexBuffer.h"
#include "../Core/Renderer.h"
#include "../Core/shader.h"
#include "../Core/Animator.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

/**
 * @class MovableObject
 * @brief The class that is responsible for everything related to MovableObjects, such as pacman, ghosts, etc. 
 */
class MovableObject
{
private:

	int height,
		width,
		mapID;

	double	startX,
			startY;

	std::vector <unsigned int>	movableObjectIndices;
	std::vector <glm::vec3>		movableObjectVertices;

	Maze3D* m_Maze;
	Renderer* m_Renderer;
	Shader* m_Shader;

	VertexArray*		movableObjectVAO;
	VertexBuffer*		movableObjectVBO;
	VertexBufferLayout* movableObjectVBLayout;
	IndexBuffer*		movableObjectIBO;

	Animator*			movableObjectAnimator;
	std::string			m_spritePaths;


public:
	enum class Direction2D
	{
		up = 0,
		down = 1,
		right = 2,
		left = 3
	};

	MovableObject(Maze3D* maze, Shader* shader, Renderer* renderer,const int ID, const std::string spritePaths);
	~MovableObject();
	
	void generateMovableObject();
	void draw();
	void camera(int hSize, int vSize);
	virtual void setPosition(float dt);
	virtual void setDirection() = 0;

private:
	void generateMovableObjectVertices();
	void makeMovableObjectIndices();

public:
	Direction2D	direction;
	bool		newDirection, alive;
	double		posX,
				posY;
};
