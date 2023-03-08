#include "Engine.h"

void Engine::startGL()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);*/

}

bool Engine::startGlew()
{
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "failed to start glew" << std::endl;
		return 0;
	}
	return 1;
}

void Engine::makeViewport()
{
	int width, height;

	glfwGetFramebufferSize(this->window, &width, &height);

	glViewport(0, 0, width, height);
}



bool Engine::startWindow()
{
	this->window = glfwCreateWindow(this->width, this->height, 
		"my window", /*nullptr */ glfwGetPrimaryMonitor(), nullptr);

	if (this->window == nullptr)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return 0;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwMakeContextCurrent(this->window);

	return 1;
}


void Engine::keyCallback(GLFWwindow* window, GLint key,
	GLint scancode, GLint action, GLint mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (action == GLFW_PRESS)
	{
		Engine::keyBuffer[key] = 1;
	}
	if (action == GLFW_RELEASE)
	{
		Engine::keyBuffer[key] = 0;
	}
}


int Engine::start()
{
	startGL();

	if (!startWindow()) return -1;

	if (!startGlew()) return -1;

	makeViewport();

	glfwSetKeyCallback(this->window, this->keyCallback);
	
	glfwSetCursorPosCallback(this->window, this->cursorCallback);

	glEnable(GL_DEPTH_TEST);

}

void Engine::bindCameraKeys()
{
	GLfloat cameraSpeed = 0.02f;
	if (Engine::keyBuffer[GLFW_KEY_W])
	{
		camera->position += cameraSpeed * camera->cameraFront;
	}
	if (Engine::keyBuffer[GLFW_KEY_S])
	{
		camera->position -= cameraSpeed * camera->cameraFront;
	}
	if (Engine::keyBuffer[GLFW_KEY_D])
	{
		camera->position += normalize(cross(camera->cameraFront, camera->up)) * cameraSpeed;
	}
	if (Engine::keyBuffer[GLFW_KEY_A])
	{
		camera->position -= normalize(cross(camera->cameraFront, camera->up)) * cameraSpeed;;
	}
	if (Engine::keyBuffer[GLFW_KEY_SPACE])
	{
		camera->position.y += 0.01f;
	}
	if (Engine::keyBuffer[GLFW_KEY_LEFT_SHIFT])
	{
		camera->position.y -= 0.01f;
	}
}
void Engine::bufferKeyCallback()
{
	bindCameraKeys(); 	// стандартная обработка камеры
	void(*callback)() = nullptr;
	for (int i = 0; i < 512; i++)
	{
		callback = this->mapKeyCallback[i];
		if (callback != nullptr && Engine::keyBuffer[i] == 1)
		{
			(*callback)();
		}
	}
}


void Engine::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (Engine::lastCursorPosX != (double)INT_MAX || Engine::lastCursorPosY != (double)INT_MAX)
	{


		GLfloat sensitivity = 0.05f;

		camera->yaw += (xpos - Engine::lastCursorPosX) * sensitivity;
		camera->pitch += (Engine::lastCursorPosY - ypos) * sensitivity;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (camera->pitch > 89.0f)
			camera->pitch = 89.0f;
		if (camera->pitch < -89.0f)
			camera->pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(radians(camera->yaw)) * cos(radians(camera->pitch));
		front.y = sin(radians(camera->pitch));
		front.z = sin(radians(camera->yaw)) * cos(radians(camera->pitch));
		camera->cameraFront = glm::normalize(front);
	}



	Engine::lastCursorPosX = xpos;
	Engine::lastCursorPosY = ypos;
}

bool Engine::mainLoop()
{
	while (!glfwWindowShouldClose(this->window))
	{
		glfwPollEvents();
		
		bufferKeyCallback();

		glClearColor(0.1f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (!this->scene.empty())
		{
			for (auto gameObject = scene.begin(); gameObject != scene.end(); gameObject++)
			{
				(*gameObject)->update();
			}
		}
		glfwSwapBuffers(this->window);

	}
	glfwTerminate();
	return 0;
}

Engine::Engine()
{
	this->start();
}


GLint Engine::keyBuffer[512]{ 0 }; // инициализируем буфер

double Engine::lastCursorPosX = (double)INT_MAX;
double Engine::lastCursorPosY = (double)INT_MAX;
Camera* Engine::camera = nullptr;

