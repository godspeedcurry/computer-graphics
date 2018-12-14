#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader sunShader("sun_shader.vs", "v1_shader.fs");
	Shader earthShader("earth_shader.vs", "v1_shader.fs");
	Shader moonShader("moon_shader.vs", "v1_shader.fs");
	Shader planetShader("planet_shader.vs", "v1_shader.fs");

	// load models
	// -----------
	Model earth(FileSystem::getPath("resources/objects/earth/earth.obj"));
	Model sun(FileSystem::getPath("resources/objects/sun/sun.obj"));
	Model moon(FileSystem::getPath("resources/objects/moon/moon.obj"));
	Model planet(FileSystem::getPath("resources/objects/planet/planet.obj"));


	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);
		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		sunShader.use();
		earthShader.use();
		moonShader.use();
		planetShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		sunShader.setMat4("projection", projection);
		sunShader.setMat4("view", view);

		earthShader.setMat4("projection", projection);
		earthShader.setMat4("view", view);

		moonShader.setMat4("projection", projection);
		moonShader.setMat4("view", view);

		planetShader.setMat4("projection", projection);
		planetShader.setMat4("view", view);
		float currentTime = (float)glfwGetTime();
		// render the loaded model
		glm::mat4 sunModel;

		sunModel = glm::translate(sunModel, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		sunModel = glm::scale(sunModel, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
		sunModel = glm::rotate(sunModel, currentTime, glm::vec3(0.0f, 0.1f, 0.0f));
		sunShader.setMat4("sunModel", sunModel);
		sun.Draw(sunShader);
		float r = 3.0f;

		glm::mat4 earthModel;
		earthModel = glm::translate(earthModel, glm::vec3(r*sin(currentTime/2), 0.2f, r*cos(currentTime/2))); // translate it down so it's at the center of the scene
		earthModel = glm::scale(earthModel, glm::vec3(0.001f, 0.001f, 0.001f));	// it's a bit too big for our scene, so scale it down
		earthModel = glm::rotate(earthModel, currentTime/2, glm::vec3(0.0f, 0.1f, 0.0f));
		earthShader.setMat4("earthModel", earthModel);
		earth.Draw(earthShader);
		//r *= 1.3;
		float d = 1.0f;
		glm::mat4 moonModel;
		moonModel = glm::translate(moonModel, glm::vec3(r*sin(currentTime/2) + d * sin(currentTime*1.5), 0.2f, r*cos(currentTime/2)+d*cos(currentTime*1.5))); // translate it down so it's at the center of the scene
		moonModel = glm::scale(moonModel, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		moonModel = glm::rotate(moonModel, currentTime*2, glm::vec3(0.0f, 0.1f, 0.0f));
		moonShader.setMat4("moonModel", moonModel);
		moon.Draw(moonShader);
		r *= 1.5;
		glm::mat4 planetModel;

		//利用3D空间圆的参数方程
		planetModel = glm::translate(planetModel, glm::vec3(-r*sqrt(2)/2*cos(currentTime / 2) , r*sqrt(2) / 2 * cos(currentTime / 2),r*sin(currentTime / 2)  )); // translate it down so it's at the center of the scene
		planetModel = glm::scale(planetModel, glm::vec3(0.22f, 0.22f, 0.22f));	// it's a bit too big for our scene, so scale it down
		planetModel = glm::rotate(planetModel, currentTime * 2, glm::vec3(0.0f, 0.1f, 0.0f));


		planetShader.setMat4("planetModel", planetModel);
		planet.Draw(planetShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
