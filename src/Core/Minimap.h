/**
 * @file Minimap.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the minimap class
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include "../Maze3D/Ghost3D.h"
#include "../Maze2D/Maze.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"
#include "../Maze2D/Pellets.h"
#include "../Maze2D/Ghost.h"

/**
 * @class Minimap
 * @brief Declarations for the minimap class
 */
class Minimap
{
public:
	Minimap(ScenarioLoader* loadedLevel, Shader* shader, Renderer* renderer, 
			Shader* minimapShader, Maze3D* maze3D, Camera* player, std::vector<Ghost3D*> ghosts);

	void Draw(Shader* shader, float dt);
private:
	Maze* maze2D;
	Maze3D* maze3D;
	Pellets* pellets2D;
	MovableObject* pacman2D;
	std::vector <MovableObject*> ghosts2D;
	std::vector<Ghost3D*> ghosts3D;

	void generateQuad();

	Shader* m_Shader;
	VertexArray*		minimapVAO;
    VertexBuffer*		minimapVBO;
	VertexBufferLayout*	minimapVBLayout;
	Framebuffer*		minimapFB;
	Texture*			minimapTex;
	Renderbuffer*		minimapRB;
};