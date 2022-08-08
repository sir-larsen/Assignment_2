/**
 * @file Pellets.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The source code for the Pellets class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Pellets.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

/**
 * @brief Construct a new Pellets:: Pellets object
 * 
 * @param maze 		- The maze the pellets are to be drawn in
 * @param shader 	- The Pellet's shader
 * @param renderer 	- The Pellet's renderer
 * @param player 	- The player responsible for "eating" the pellets. 
 * 
 * @see generatePellets()
 */
Pellets::Pellets(Maze3D* maze, Shader* shader, Renderer* renderer/*, Camera* player*/)
	:	m_Maze(maze),
		m_Renderer(renderer),
		m_Shader(shader),
		/*m_Player(player),*/
		allPelletsEaten(false)
{
	generatePellets();
	remainingPellets = m_Maze->getPelletCount();
}

/**
 * @brief Destroy the Pellets:: Pellets object
 * 
 */
Pellets::~Pellets()
{
		
	free(m_Maze);
	free(m_Renderer);
	free(m_Shader);
	free(pelletsVAO);
	free(pelletsVBO);
	free(pelletsVBLayout);
	free(pelletsIBO);
	free(pelletsTexture);
	//free(m_Player);
}

/**
 * @brief Generates the pellets, from the creation of the vertices and indices
 * 		  to the OpenGL code.
 * @see makeVertices();
 * @see makePelletsIndices();
 */
void Pellets::generatePellets()
{
	makeVertices();
	makePelletsIndices();



	pelletsVAO = new VertexArray;
	pelletsVAO->Bind();
	pelletsVBO = new VertexBuffer(&pelletVertices[0], pelletVertices.size() * sizeof(glm::vec3));
	pelletsVBO->Bind();
	pelletsVBLayout = new VertexBufferLayout;
	pelletsVBLayout->Push<float>(3);
	pelletsVBLayout->Push<float>(3);
	pelletsVAO->AddBuffer(*pelletsVBO, *pelletsVBLayout);

	pelletsIBO = new IndexBuffer(&pelletsIndices[0], pelletsIndices.size());

	//m_Shader->createShaderProgram();
	//m_Shader->setUniform4f("u_Color",.1f, .1f, .1f, 1.f);
	m_Shader->use();

	pelletsTexture = new Texture("res/smallPelletYellow.png");
	pelletsTexture->Bind(0);
	//m_Shader->setUniform1i("u_Texture", 0);
	m_Shader->setInt("u_Texture", 0);
}

/**
 * @brief Generates the indices for each pellet.
 * 
 */
void Pellets::makePelletsIndices()
{

	int indicesHeigth = m_Maze->getHeight(); int indicesWidth = m_Maze->getWidth();

	for (int i = 0; i < indicesHeigth; i++) {
		for (int j = 0; j < indicesWidth; j++) {
			if (i < m_Maze->map2d.size() && j < m_Maze->map2d[i].size() && m_Maze->map2d[i][j] == 0) {

				int k = (i * indicesWidth + j) * 4;
				pelletsIndices.push_back(k);
				pelletsIndices.push_back(k + 1);
				pelletsIndices.push_back(k + 2);
				pelletsIndices.push_back(k + 1);
				pelletsIndices.push_back(k + 2);
				pelletsIndices.push_back(k + 3);

			}
		}
	}
}

/**
 * @brief Draws all pellets. 
 * @see hasBeenEaten();
 */
void Pellets::draw()
{
	m_Shader->use();
	pelletsTexture->Bind(0);
	hasBeenEaten();
	camera(m_Maze->getWidth(), m_Maze->getHeight());
	m_Renderer->Draw(pelletsVAO, pelletsIBO, m_Shader);
}

/**
 * @brief Sets the view projection for the pellets
 * 
 * @param hSize - Horizontal size
 * @param vSize - Vertical size
 */
void Pellets::camera(int hSize, int vSize)
{
		glm::mat4 projection = glm::ortho(0.f, (float)hSize, (float)vSize, 0.f);

		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		m_Shader->setMat4("u_ProjectionMat", projection);
		m_Shader->setMat4("u_ViewMat", view);
}

/**
 * @brief Makes all the vertices for the pellets, including positions and texture coordinates. 
 * 
 */
void Pellets::makeVertices()
{
		for (int y = 0; y < m_Maze->getHeight(); y++) {
			for (int x = 0; x < m_Maze->getWidth(); x++) {
				pelletVertices.push_back(glm::vec3(x, y, 0.f));     //position
				pelletVertices.push_back(glm::vec3(0.f, 1.f, 0.f)); //texture
				
				pelletVertices.push_back(glm::vec3(x+1, y, 0.f));   //position
				pelletVertices.push_back(glm::vec3(1.f, 1.f, 0.f)); //texture
				
				pelletVertices.push_back(glm::vec3(x, y+1, 0.f));   //position
				pelletVertices.push_back(glm::vec3(0.f, 0.f, 0.f)); //texture
				
				pelletVertices.push_back(glm::vec3(x+1, y+1, 0.f)); //position
				pelletVertices.push_back(glm::vec3(1.f, 0.f, 0.f)); //texture
			}
		}
}

/**
 * @brief Checks wheter or not a pellet has been "eaten" by pacman (the player).
 *		  If the pellet has been eaten, the texture for the given pellet is set to 0
 */
void Pellets::hasBeenEaten()
{
	int height = m_Maze->getHeight(); int width = m_Maze->getWidth();
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (m_Maze->map2d[y][x] == 9) {
				int i = (y * m_Maze->getWidth() + x) * 8;
				pelletVertices[i + 1] = glm::vec3(0.f); //sets the textures for the "eaten" object to null.
				pelletVertices[i + 3] = glm::vec3(0.f);
				pelletVertices[i + 5] = glm::vec3(0.f);
				pelletVertices[i + 7] = glm::vec3(0.f);
			}
	pelletsVAO->changeData(pelletsVBO, &pelletVertices[0], pelletVertices.size() * sizeof(glm::vec3));
		if (--remainingPellets == 0)
			allPelletsEaten = true;
}
