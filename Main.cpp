#include "RenderingEngine.h"

#include "Input.h"
#include "Camera.h"
#include "World.h"

#include "Client.h"
#include "Server.h"
#include "ModelManager.h"
#include "Profiler.h"

#include <glew.h>
#include <iostream>
#include <thread>
#include "Human.h"

int main(int argc, char** argv)
{
#define CLIENT
#define SERVER

#ifdef CLIENT
	RenderingEngine::init();
	
	Window* window = new Window();
	window->init("Game  |  Pre-Alpha", 1280, 720);
	ModelManager::load();
	window->setVisible(true);
	
	unsigned int numThreads = std::thread::hardware_concurrency();
	if (numThreads < 3) numThreads = 3;
	ThreadManager::startThreadPool(numThreads - 2);

	std::cout << "Using " << numThreads << " threads" << std::endl;

#endif
	SDLNet_Init();
#ifdef SERVER
	Server server;
	server.init();
	server.start();
	Human* human = new Human();
	human->setPosition(Vec3(23.0f, 172.0f, 0.0f));
	human->setRotation(Vec3(0.0f, 90.0f, 0.0f));
	server.getWorld()->getEntityManager().addEntity(human);
#ifndef CLIENT
	while (true) SDL_Delay(1);
#endif
#endif
#ifdef CLIENT

	Client client;
	client.init("localhost", 25565);
	client.start();
	
	window->bindRenderTarget();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glLineWidth(8);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	World* world = new World(nullptr, &client);
	client.setWorld(world);

	Uint32 currentTime = 0;
	Uint32 timer = 0;
	Uint32 lastTime = SDL_GetTicks();
	float delta = 0.0f;
	const float ns = 1000.0f / 60.0f;

	int frames = 0;

	bool running = true;
	SDL_Event windowEvent;
	while (running) {
		currentTime = SDL_GetTicks();
		delta = (float)(currentTime - lastTime);
		timer += currentTime - lastTime;
		lastTime = currentTime;
		
		window->bindRenderTarget();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (Input::getEvent(SDLK_1, true)) {
			RenderingEngine::toggleWireframe();
		}

		world->update(delta);
		world->render(window);

		frames++;

		if (timer >= 1000) {
			//Profiler::print();
			std::cout << frames << "fps" << std::endl << std::endl;
			frames = 0;
			timer -= 1000;
		}
		Profiler::start("Swapping buffers");
		window->swapBuffers();
		Profiler::stop();
		Profiler::start("Handling window input");
		Input::refresh();
		while (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				running = false;
			} else {
				Input::processEvent(windowEvent);
			}
		}
		Profiler::stop();
	}
	
	ModelManager::cleanup();

	client.stop();
	client.cleanup();

#endif
#ifdef SERVER
	server.stop();
	server.cleanup();
#endif

	SDLNet_Quit();

#ifdef CLIENT
	world->cleanup();
	ThreadManager::stopThreadPool();
	delete world;
	delete window;

	RenderingEngine::cleanup();
#endif
	return 0;
}