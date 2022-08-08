/**
 * @file Maze3D.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source code for the Maze3D class
 * @version 0.1
 * @date 2020-10-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "Maze3D.h"
#include "../Core/ScenarioLoader.h"

#include "../Core/VertexBuffer.h"
#include "../Core/IndexBuffer.h"
#include "../Core/VertexArray.h"
#include "../Core/Renderer.h"

#include <iostream>

 /**
  * @brief Construct a new Maze3D:: Maze3D object
  *
  * @param loadedLevel 	- A ScenarioLoader containing the level file
  * @param shader 	  	- The Maze3D's shader
  * @param renderer 	- The Maze3D's renderer
  *
  * @see	make2dArray();
  * @see	generateMaze3D();
  * @see	countPellets();
  */
Maze3D::Maze3D(ScenarioLoader* loadedLevel, Shader* shader, Renderer* renderer)
	: m_LoadedLevel(loadedLevel),
	m_Renderer(renderer),
	m_Shader(shader),
	pelletCount(0)
{
	width = m_LoadedLevel->getHorizontalSize();
	height = m_LoadedLevel->getVerticalSize();

	map2d.resize(height, std::vector<int>(width));
	make2dArray();
	generateMaze3D();
	countPellets();
}

/**
 * @brief Destroy the Maze3D:: Maze3D object
 *
 */
Maze3D::~Maze3D() {
	free(Maze3DVAO);
	free(Maze3DVBO);
	free(Maze3DVBLayout);
	free(Maze3DIBO);
}

/**
 * @brief Converts the 1D vector of the map from the ScenarioLoader into a 2D vector.
 *
 */
void Maze3D::make2dArray()
{
	int index = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			map2d[i][j] = m_LoadedLevel->mazeMap[index++];
		}
	}
}


/**
 * @brief Loops through the maze to find the players spawning point.
 * 
 * @return glm::vec3 
 */
glm::vec3 Maze3D::findSpawn()
{
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (map2d[y][x] == 2)
			{
				return glm::vec3((float)x+.5f,0.5f,(float)y+.5f);
			}
				
}

/**
 * @brief Sets the different lighting uniforms in the maze. 
 * 
 * @param dt 	 - Delta time
 * @param camera - The camera being controlled by the player. 
 */
void Maze3D::Light(const float dt, Camera camera)
{
	
	m_Shader->setVec3("pointLight.position", glm::vec3(14, 3, 18));
	m_Shader->setVec3("pointLight.ambient", 1.0f, 1.0f, 1.0f);
	m_Shader->setVec3("pointLight.diffuse", 0.1f, 0.1f, 0.1f);
	m_Shader->setVec3("pointLight.specular", 0.1f, 0.1f, 0.1f);
	m_Shader->setFloat("pointLight.constant", 1.f);
	m_Shader->setFloat("pointLight.linear", 0.09);
	m_Shader->setFloat("pointLight.quadratic", 0.032);



	m_Shader->setVec3("spotLight.position", camera.Position);
	m_Shader->setVec3("spotLight.direction", camera.Front);
	m_Shader->setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
	m_Shader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	m_Shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	m_Shader->setFloat("spotLight.constant", 1.0f);
	m_Shader->setFloat("spotLight.linear", 0.09);
	m_Shader->setFloat("spotLight.quadratic", 0.032);
	m_Shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	m_Shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}

/**
 * @brief Transforms the 3d Maze. 
 * 
 * @param dt - Delta time, will be used if one would want to rotate the maze
 */
void Maze3D::Transform(float dt)
{
	glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(0.f));
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(1.f));
	glm::mat4 transformation = translation  * scale;
	m_Shader->setMat4("u_TransformationMat", transformation);
}

/**
 * @brief Generates the indices for each square in the Maze3D.
 *
 */
void Maze3D::makeIndices()
{

	//Indices for the plane under the maze
	Maze3DIndices.push_back(0); 
	Maze3DIndices.push_back(1);
	Maze3DIndices.push_back(2);
	Maze3DIndices.push_back(1);
	Maze3DIndices.push_back(2);
	Maze3DIndices.push_back(3);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i < map2d.size() && j < map2d[i].size() && map2d[i][j] == 1) {
				int k = 4 + (i * width + j) * 24;
				

				/*
				The top and bottom have been removed by performance reasons,
				still left in code in case anyone would like to re add them. 
				//Bottom
				Maze3DIndices.push_back(k);
				Maze3DIndices.push_back(k + 1);
				Maze3DIndices.push_back(k + 2);
				Maze3DIndices.push_back(k + 1);
				Maze3DIndices.push_back(k + 2);
				Maze3DIndices.push_back(k + 3);
				//Top
				Maze3DIndices.push_back(k + 4);
				Maze3DIndices.push_back(k + 5);
				Maze3DIndices.push_back(k + 6);
				Maze3DIndices.push_back(k + 5);
				Maze3DIndices.push_back(k + 6);
				Maze3DIndices.push_back(k + 7);
				
				*/

				if (j + 1 < width)
					//Right
					if (map2d[i][j + 1] != 1) {
					Maze3DIndices.push_back(k + 8);
					Maze3DIndices.push_back(k + 9);
					Maze3DIndices.push_back(k + 10);
					Maze3DIndices.push_back(k + 9);
					Maze3DIndices.push_back(k + 10);
					Maze3DIndices.push_back(k + 11);
					}
				
				if (j - 1 >= 0)
					if (map2d[i][j - 1] != 1) {
					//Left
					Maze3DIndices.push_back(k + 12);
					Maze3DIndices.push_back(k + 13);
					Maze3DIndices.push_back(k + 14);
					Maze3DIndices.push_back(k + 13);
					Maze3DIndices.push_back(k + 14);
					Maze3DIndices.push_back(k + 15);
					}

				if (i - 1 >= 0)
					if (map2d[i - 1][j] != 1) {
					//Back
					Maze3DIndices.push_back(k + 16);
					Maze3DIndices.push_back(k + 17);
					Maze3DIndices.push_back(k + 18);
					Maze3DIndices.push_back(k + 17);
					Maze3DIndices.push_back(k + 18);
					Maze3DIndices.push_back(k + 19);
					}


				if (i + 1 < height)
					if (map2d[i + 1][j] != 1) {
					//Front
					Maze3DIndices.push_back(k + 20);
					Maze3DIndices.push_back(k + 21);
					Maze3DIndices.push_back(k + 22);
					Maze3DIndices.push_back(k + 21);
					Maze3DIndices.push_back(k + 22);
					Maze3DIndices.push_back(k + 23);
					}
				
			}
		}
	}
}

/**
 * @brief Generates positions to be used by the squares.
 *
 */
void Maze3D::makeVertices()
{

	//making the floor +2 bigger than the maze in size to make sure it covers it all.
	Vertex vertex;
	//Plane / Floor for the maze:
	vertex.position = glm::vec3(-2.f, -0.1f, -2.f);
	vertex.normal = glm::vec3(0, 1.0f, 0);
	vertex.textureCoord = glm::vec2(0.0f, 0.0f);
	Maze3DVertex.push_back(vertex);

	vertex.position = glm::vec3(width+2.f, -0.1f, -2.f);
	vertex.normal = glm::vec3(width+2.f, 1.0f, -2.f);
	vertex.textureCoord = glm::vec2(1.0f, 0.0f);
	Maze3DVertex.push_back(vertex);

	vertex.position = glm::vec3(-2.f, -0.1f, height+2.f);
	vertex.normal = glm::vec3(-2.f, 1.0f, height+2.f);
	vertex.textureCoord = glm::vec2(0.0f, 1.0f);
	Maze3DVertex.push_back(vertex);

	vertex.position = glm::vec3(width+2.f, -0.1f, height+2.f);
	vertex.normal = glm::vec3(width+2.f, 1.0f, height+2.f);
	vertex.textureCoord = glm::vec2(1.0f, 1.0f);
	Maze3DVertex.push_back(vertex);

	for (int z = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
				//Bottom
				vertex.position		= glm::vec3(x,		0.0f, z);		
				vertex.normal		= glm::vec3(x,		-1.0f, z);		
				vertex.textureCoord = glm::vec2(0.0f,	0.0f);			
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x + 1,	0.0f, z);		
				vertex.normal		= glm::vec3(x + 1,	-1.0f, z);		
				vertex.textureCoord = glm::vec2(1.0f,	0.0f);			
				Maze3DVertex.push_back(vertex);
				
				vertex.position		= glm::vec3(x,		0.0f, z + 1);		
				vertex.normal		= glm::vec3(x,		-1.0f, z + 1);		
				vertex.textureCoord = glm::vec2(0.0f,	1.0f);			
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x + 1,	0.0f, z + 1);	
				vertex.normal		= glm::vec3(x + 1,	-1.0f, z + 1);	
				vertex.textureCoord = glm::vec2(1.0f,	1.0f);			
				Maze3DVertex.push_back(vertex);

				//Top
				vertex.position		= glm::vec3(x,		1.0f, z + 1);
				vertex.normal		= glm::vec3(x,		2.0f, z + 1);
				vertex.textureCoord = glm::vec2(0.0f,	0.0f);
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x + 1,	1.0f, z + 1);
				vertex.normal		= glm::vec3(x + 1,	2.0f, z + 1);
				vertex.textureCoord = glm::vec2(1.0f,	0.0f);
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x,		1.0f, z);
				vertex.normal		= glm::vec3(x,		2.0f, z);
				vertex.textureCoord = glm::vec2(0.0f,	1.0f);
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x + 1,	1.0f, z);
				vertex.normal		= glm::vec3(x + 1,	2.0f, z);
				vertex.textureCoord = glm::vec2(1.0f,	1.0f);
				Maze3DVertex.push_back(vertex);

				//Right
				vertex.position		= glm::vec3(x + 1,	0.0f, z + 1);
				vertex.normal		= glm::vec3(x + 2,	0.0f, z + 1);
				vertex.textureCoord = glm::vec2(0.0f,	0.0f);
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x + 1,	0.0f, z);	
				vertex.normal		= glm::vec3(x + 2,	0.0f, z);	
				vertex.textureCoord = glm::vec2(1.0f,	0.0f);		
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x + 1,	1.0f, z + 1);
				vertex.normal		= glm::vec3(x + 2,	1.0f, z + 1);
				vertex.textureCoord = glm::vec2(0.0f,	1.0f);		
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x + 1,	1.0f, z);	
				vertex.normal		= glm::vec3(x + 2,	1.0f, z);	
				vertex.textureCoord = glm::vec2(1.0f,	1.0f);		
				Maze3DVertex.push_back(vertex);

				//Left
				vertex.position		= glm::vec3(x,		0.0f, z);		
				vertex.normal		= glm::vec3(x - 1,	0.0f, z);	
				vertex.textureCoord = glm::vec2(0.0f,	0.0f);
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x,		0.0f, z + 1);	
				vertex.normal		= glm::vec3(x - 1,	0.0f, z + 1);
				vertex.textureCoord = glm::vec2(1.0f,	0.0f);		
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x,		1.0f, z);		
				vertex.normal		= glm::vec3(x - 1,	1.0f, z);	
				vertex.textureCoord = glm::vec2(0.0f,	1.0f);		
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x,		1.0f, z + 1);	
				vertex.normal		= glm::vec3(x - 1,	1.0f, z + 1);
				vertex.textureCoord = glm::vec2(1.0f,	1.0f);		
				Maze3DVertex.push_back(vertex);

				//Back
				vertex.position		= glm::vec3(x + 1,	0.0f, z);
				vertex.normal		= glm::vec3(x + 1,	0.0f, z - 1);
				vertex.textureCoord = glm::vec2(0.0f,	0.0f);
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x,		0.0f, z);
				vertex.normal		= glm::vec3(x,		0.0f, z - 1);
				vertex.textureCoord = glm::vec2(1.0f,	0.0f);
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x + 1,	1.0f, z);
				vertex.normal		= glm::vec3(x + 1,	1.0f, z - 1);
				vertex.textureCoord = glm::vec2(0.0f,	1.0f);
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x,		1.0f, z);
				vertex.normal		= glm::vec3(x,		1.0f, z - 1);
				vertex.textureCoord = glm::vec2(1.0f,	1.0f);
				Maze3DVertex.push_back(vertex);

				//Front
				vertex.position		= glm::vec3(x,		0.0f, z + 1);
				vertex.normal		= glm::vec3(x,		0.0f, z + 2);
				vertex.textureCoord = glm::vec2(0.0f,	0.0f);
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x + 1,	0.0f, z + 1);
				vertex.normal		= glm::vec3(x + 1,	0.0f, z + 2);
				vertex.textureCoord = glm::vec2(1.0f,	0.0f);
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x,		1.0f, z + 1);
				vertex.normal		= glm::vec3(x,		1.0f, z + 2);
				vertex.textureCoord = glm::vec2(0.0f,	1.0f);
				Maze3DVertex.push_back(vertex);

				vertex.position		= glm::vec3(x + 1,	1.0f, z + 1);
				vertex.normal		= glm::vec3(x + 1,	1.0f, z + 2);
				vertex.textureCoord = glm::vec2(1.0f,	1.0f);
				Maze3DVertex.push_back(vertex);
		}
	}
}

/**
 * @brief Generates the Maze3D, from the generation of the positions/vertices, to the OpenGL stuff.
 * @see makePositions();
 * @see makeIndices();
 */
void Maze3D::generateMaze3D()
{
	makeVertices();
	makeIndices();

	Maze3DVAO = new VertexArray;
	Maze3DVAO->Bind();
	Maze3DVBO = new VertexBuffer(&Maze3DVertex[0], Maze3DVertex.size() * sizeof(Vertex));
	Maze3DVBO->Bind();
	Maze3DVBLayout = new VertexBufferLayout;
	Maze3DVBLayout->Push<float>(3);
	Maze3DVBLayout->Push<float>(3);
	Maze3DVBLayout->Push<float>(2);


	Maze3DVAO->AddBuffer(*Maze3DVBO, *Maze3DVBLayout);

	Maze3DIBO = new IndexBuffer(&Maze3DIndices[0], Maze3DIndices.size());

	m_Shader->use();

	Maze3DDiffuse = new Texture("res/maze.png");
	Maze3DDiffuse->Bind(0);
	Maze3DSpecular = new Texture("res/mazeSpec.png");
	Maze3DSpecular->Bind(1);
	
	m_Shader->setInt("material.diffuse", 0);
	
	m_Shader->setInt("material.specular", 1);
	
}

/**
 * @brief Counts how many !1 occour in the Maze3D, counting how many "pellets" exists
*/
void Maze3D::countPellets()
{
	for (int i = 0; i < m_LoadedLevel->mazeMap.size(); i++)
		if (m_LoadedLevel->mazeMap[i] != 1)
			pelletCount++;
}

/**
 * @brief Draws the 3d Maze
 * 
 * @param projection 	- The players projection matrix
 * @param view 			- The players view matrix
 * @param dt 			- Delta time
 */
void Maze3D::draw(glm::mat4 projection, glm::mat4 view,const float dt)
{
	
	m_Shader->use();
	m_Shader->setMat4("u_ProjectionMat", projection);
	m_Shader->setMat4("u_ViewMat", view);
	Transform(dt);
	Maze3DDiffuse->Bind(0);
	Maze3DSpecular->Bind(1);
	m_Renderer->Draw(Maze3DVAO, Maze3DIBO, m_Shader);
}
