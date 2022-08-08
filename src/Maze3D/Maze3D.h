/**
 * @file Maze3D.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the Maze3D class
 * @version 0.1
 * @date 2020-10-12
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once
#include "../Core/Camera.h"
#include "../Core/ScenarioLoader.h"
#include "../Core/VertexArray.h"
#include "../Core/VertexBuffer.h"
#include "../Core/VertexBufferLayout.h"
#include "../Core/IndexBuffer.h"
#include "../Core/Renderer.h"
#include "../Core/model.h"
#include "../Core/Texture.h"

/**
 * @brief Struct holding vertex data. 
 * 
 */
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoord;
};
 /**
  * @class Maze3D
  * @brief Handles the creation and drawing of the Maze3D.
  */
class Maze3D
{
private:
	int width,
		height,
		pelletCount;

	ScenarioLoader* m_LoadedLevel;
	std::vector <unsigned int> Maze3DIndices;
	std::vector <glm::vec3> Maze3DVertices;
	std::vector <Vertex> Maze3DVertex;

	Renderer* m_Renderer;

	VertexArray* Maze3DVAO;
	VertexBuffer* Maze3DVBO;
	VertexBufferLayout* Maze3DVBLayout;
	IndexBuffer* Maze3DIBO;
	Texture* Maze3DDiffuse;
	Texture* Maze3DSpecular;
public:

	Shader* m_Shader;
	std::vector<std::vector<int>> map2d;

	Maze3D(ScenarioLoader* loadedLevel, Shader* shader, Renderer* renderer);
	~Maze3D();

	void draw(glm::mat4 projection, glm::mat4 view, const float dt);
	inline std::vector <glm::vec3> getMaze3DPositions() { return Maze3DVertices; }
	inline int getHeight() { return height; }
	inline int getWidth() { return width; }
	inline int getPelletCount() { return pelletCount; }
	inline std::vector<std::vector<int>> Maze3D::getMap() { return map2d; }
	glm::vec3 findSpawn();
	void Light(const float dt, Camera camera);
	void Transform(float dt);
private:
	void countPellets();
	void make2dArray();
	void makeIndices();
	void makeVertices();
	void generateMaze3D();
};
