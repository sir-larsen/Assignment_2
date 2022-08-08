/**
 * @file Minimap.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source files for handling everything related to the minimap. 
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Minimap.h"

/**
 * @brief Construct a new Minimap:: Minimap object
 * 
 * @param loadedLevel   - The ScenarioLoader containing the level file
 * @param maze2DShader  - The 2d maze shader
 * @param renderer      - The renderer object
 * @param minimapShader - The minimap's shader
 * @param maze3D        - The 3d maze in which the game is being played
 * @param player        - The camera object controlled by the player
 * @param ghosts        - The ghosts in the 3d world. 
 */
Minimap::Minimap(ScenarioLoader* loadedLevel, Shader* maze2DShader, Renderer* renderer, 
                 Shader* minimapShader, Maze3D* maze3D, Camera* player, std::vector<Ghost3D*> ghosts)
    :   m_Shader(minimapShader),
        ghosts3D(ghosts)
{
    this->maze3D = maze3D;
    generateQuad();
	maze2D = new Maze(loadedLevel, maze2DShader, renderer);
    Shader* pellet2DShader = new Shader("shaders/pellet2D.vs","shaders/pellet2D.fs");
    Shader* pacman2DShader = new Shader("shaders/pacman2D.vs", "shaders/pacman2D.fs");
    std::vector <Shader*> ghostShaders;


    
    for (int i = 0; i < 3; i++)
    {
        ghostShaders.push_back(new Shader("shaders/ghost2D.vs","shaders/ghost2D.fs"));
        ghosts2D.push_back(new Ghost(maze3D, ghostShaders[i], renderer, 3 + i, "res/ghost/ghost",ghosts3D[i]));
    }


    pacman2D = new Pacman(maze3D, pacman2DShader, renderer, 2, "res/pacman/pacman", ghosts2D, player );
    pellets2D = new Pellets(maze3D, pellet2DShader, renderer);
}

/**
 * @brief Draws the minimap
 * 
 * @param shader - The shader for the minimap
 * @param dt     - Delta time
 */
void Minimap::Draw(Shader* shader, float dt)
{
    minimapFB->Bind(); //Binding framebuffer, updating the data on the minimap
    glDisable(GL_DEPTH_TEST);

    glClearColor(.1f, .1f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

//==============================
//The following will be rendered on the minimap:

	maze2D->draw();
    pellets2D->draw();
    pacman2D->setPosition(dt);
    pacman2D->draw();

    for (int i = 0; i < 3; i++) {
        ghosts2D[i]->setPosition(dt);
        ghosts2D[i]->draw();
    }
    
//=============================    

    minimapFB->Unbind(); //Unbinding the framebuffer, we are now updating the default framebuffer

    shader->use();
    minimapTex->Bind();
    minimapVAO->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

/**
 * @brief Generates the quad on which the minimap resides. 
 * 
 */
void Minimap::generateQuad()
{
    //Minimap coordinates are hardcoded as this position is constant, therefore
    //independant on a level file. 
    float quadVertices[] = {
    0.3f, 1.0f, 0.0f, 1.0f,
    0.3f, 0.1f, 0.0f, 0.0f,
    1.0f, 0.1f, 1.0f, 0.0f,

    0.3f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.1f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f
    };

    minimapVAO = new VertexArray();
    minimapVAO->Bind();

    minimapVBO = new VertexBuffer(&quadVertices, sizeof(quadVertices));
    minimapVBO->Bind();

    minimapVBLayout = new VertexBufferLayout;
    minimapVBLayout->Push<float>(2);
    minimapVBLayout->Push<float>(2);

    minimapVAO->AddBuffer(*minimapVBO, *minimapVBLayout);

    
    minimapFB = new Framebuffer(); //Creating the framebuffer for our minimap
    minimapFB->Bind();

    m_Shader->use();
    minimapTex = new Texture(); //using the overloaded constructor with no param
                                //to generate a empty texture object for our framebuffer
                                //to draw to.

    minimapRB = new Renderbuffer;
    minimapRB->setStorage(1024);

    minimapFB->addRenderBuffer(minimapRB->getID());
    minimapFB->Unbind();
}
