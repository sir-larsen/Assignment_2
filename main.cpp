#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "src/Core/ScenarioLoader.h"
#include "src/Core/Renderer.h"
#include "src/Maze3D/Pellet3D.h"
#include "src/Maze3D/Ghost3D.h"
#include "src/Core/Minimap.h"

#include <set>
#include <iostream>
#include <iomanip>

void GLAPIENTRY MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, std::vector<std::vector<int>> m_Map2d, bool m_ConstrainMovement);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Camera* camera;

const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 1200;

float lastX = SCREEN_WIDTH / 2.f;
float lastY = SCREEN_HEIGHT / 2.f;
bool firstMouse = true;
bool constrainMovement = true;
bool gameover = false;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main() {
    // Initialization of GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed." << '\n';
        std::cin.get();

        return EXIT_FAILURE;
    }

    // Setting window hints
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(1200, 1200, "Pacman 3D", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "GLFW failed on window creation." << '\n';
        std::cin.get();

        glfwTerminate();

        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);


    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Setting the OpenGL context.
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization failure." << '\n';
        std::cin.get();

        return EXIT_FAILURE;
    }

    //Enable capture of debug output.
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);


    glEnable(GL_DEPTH_TEST);

    const GLuint SHADOW_WIDTH = 1024;
    const GLuint SHADOW_HEIGHT = 1024;

    ScenarioLoader  scenario("levels/level0");
    Shader          shader("shaders/maze.vs", "shaders/maze.fs");
    Renderer        renderer;
    Maze3D          maze(&scenario,&shader,&renderer);
    camera =        new Camera(maze.findSpawn());

    Shader pelletShader("shaders/pellet.vs", "shaders/pellet.fs");
    Model pellet("res/pellet/pellet.obj");
    Pellet3D pellets(&pellet, &maze);

    Model ghost("res/ghost/Ghost.obj");

    AStar pathfinder(&maze);

    std::vector<Shader*>     ghostShaders;
    std::vector<Ghost3D*>    ghosts;

    for (int i = 0; i < 3; i++) {
        ghostShaders.push_back(new Shader("shaders/ghost.vs", "shaders/ghost.fs"));
        ghosts.push_back(new Ghost3D(&ghost, &maze, 3 + i, camera, &pathfinder));
    }


    Shader minimapShader("shaders/minimap.vs", "shaders/minimap.fs");
    Shader maze2DShader("shaders/maze2D.vs", "shaders/maze2D.fs");

    Minimap minimap(&scenario,&maze2DShader,&renderer, &minimapShader, &maze, camera, ghosts);



    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //draw in wireframe mode

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, maze.getMap(),constrainMovement);

        glClearColor(0.1f, 0.1f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        // camera/view transformation
        glm::mat4 view = camera->GetViewMatrix();

        maze.m_Shader->use();
        maze.Light(deltaTime, *camera);
        maze.draw(projection, view, deltaTime);

        pellets.eatPellet(camera, &maze);
        pellets.Draw(&pelletShader, projection, view);
        if (pellets.allEaten)
            gameover = true;

        for (int i = 0; i < 3; i++) {
            ghosts[i]->Draw(ghostShaders[i], projection, view, deltaTime,constrainMovement);
            if (ghosts[i]->playerEaten)
                gameover = true;
        }

        minimap.Draw(&minimapShader, deltaTime);

        if (gameover) {
            std::cout << "\nYou ate " << maze.getPelletCount() - pellets.pelletCount 
                      << '/' << maze.getPelletCount() << " pellets!" << std::endl;
            break;
        }

        glfwSwapBuffers(window);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            break;
        }
        glfwPollEvents();

    }
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, std::vector<std::vector<int>> m_Map2d, bool m_ConstrainMovement)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime, m_Map2d, m_ConstrainMovement);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime, m_Map2d, m_ConstrainMovement);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime, m_Map2d, m_ConstrainMovement);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime, m_Map2d, m_ConstrainMovement);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        constrainMovement = !constrainMovement;
}

void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    std::cerr << "GL CALLBACK:" << (type == GL_DEBUG_TYPE_ERROR ? " GL ERROR " : "") <<
        "type = 0x" << type <<
        ", severity = 0x" << severity <<
        ", message =" << message << "\n";
}