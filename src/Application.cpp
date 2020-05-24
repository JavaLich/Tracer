#include "Application.h"
#include <math.h>

Application::Application(AppConfig config)
{
	this->config = config;
	init();
}

Application::Application()
{
	this->config = AppConfig();
	init();
}

void Application::init()
{
	initCL();
	initSDL();
}


void Application::run()
{
	running = true;

	uint32_t* pixels = new uint32_t[(size_t)config.width * config.height];
	int pitch = config.width * sizeof(uint32_t);
	uint64_t sphere_count = 8;
	Sphere* sphere = new Sphere[8]; // I dont know why it needs to add 1
	sphere[0].position = cl_float3{ 0.0f, 0.0f, 65.0f };
	sphere[0].color = cl_uint3{ 255, 255, 255 };
	sphere[0].radius = 10.0f;
	sphere[1].position = cl_float3{ 1e5f, 0.0f, -2500.0f };
	sphere[1].color = cl_uint3{ 255, 0, 0 };
	sphere[1].radius = 1e5f;
	sphere[2].position = cl_float3{ -1e5f, 0.0f, -2500.0f };
	sphere[2].color = cl_uint3{ 0, 255, 0 };
	sphere[2].radius = 1e5f;
	sphere[3].position = cl_float3{ 0.0f, 0.0f, 1e5f + 100 };
	sphere[3].color = cl_uint3{ 0, 0, 255 };
	sphere[3].radius = 1e5f;
	sphere[4].position = cl_float3{ 0.0f, -1e5f, -2500.0f };
	sphere[4].color = cl_uint3{ 200, 55, 0 };
	sphere[4].radius = 1e5f;
	sphere[5].position = cl_float3{ 0.0f, 1e5f, 2500.0f };
	sphere[5].color = cl_uint3{ 55, 200, 0 };
	sphere[5].radius = 1e5f;
	sphere[6].position = cl_float3{ 0.0f, 0.0f, -1e5f - 100.0f };
	sphere[6].color = cl_uint3{0, 55, 0 };
	sphere[6].radius = 1e5f;
	sphere[7].position = cl_float3{ 20.0f, -10.0f, 60.0f };
	sphere[7].color = cl_uint3{ 255, 255, 255 };
	sphere[7].radius = 10.0f;
	
	
	uint64_t light_count = 2;
	Light* lights = new Light[light_count];
	lights[0].position = cl_float3{ 10.0f, 10.0f, -50.0f };
	lights[1].position = cl_float3{ -10.0f, -10.0f, -50.0f };

	cl_float3* rot = new cl_float3[3];
	Scene scene;
	SDL_WarpMouseInWindow(window, APP_WIDTH / 2, APP_HEIGHT / 2);

	SDL_Event event;	 // used to store any events from the OS
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	lastFrame = SDL_GetTicks();
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	while (running)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
		if (benchmark_count >= 1000) {
			SDL_SetWindowTitle(window, ("RayTracer | Frametime: " + std::to_string(averageFrameTime) + " | FPS: " + std::to_string(averageFPS)).c_str());
			benchmark_count = 0;
		}
		// poll for events from SDL
		while (SDL_PollEvent(&event))
		{
			running = event.type != SDL_QUIT;
			if (event.type = SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
				running = false;
			}
		}

		rot = updateCamera(scene, (float)deltaTime);
		api->render(pixels, sphere, sphere_count, config.width, config.height, scene, lights, light_count,  rot);
		SDL_UpdateTexture(buffer, NULL, pixels, pitch);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, buffer, NULL, NULL);
		SDL_RenderPresent(renderer);
		benchmark();
	}
}

void Application::initSDL()
{
	// Initialize SDL with video
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(config.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.width, config.height, SDL_WINDOW_OPENGL);

	// if failed to create a window
	if (!window)
	{
		// we'll print an error message and exit
		std::cerr << "Error failed to create window!\n";
	}

	// Create an OpenGL context (so we can use OpenGL functions)
	context = SDL_GL_CreateContext(window);

	// if we failed to create a context
	if (!context)
	{
		// we'll print out an error message and exit
		std::cerr << "Error failed to create a context\n!";
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, config.width, config.height);
}
void Application::initCL()
{
	api = new ComputeAPI();
}

void Application::destroy()
{
	SDL_DestroyTexture(buffer);
	SDL_DestroyRenderer(renderer);

	// Destroy the context
	SDL_GL_DeleteContext(context);

	// Destroy the window
	SDL_DestroyWindow(window);

	// And quit SDL
	SDL_Quit();
}

void Application::benchmark()
{
	uint32_t current = SDL_GetTicks();
	frametimes[currentIndex] = current - lastFrame;
	lastFrame = current;
	benchmark_count += frametimes[currentIndex];
	double total = 0;
	for (uint32_t i = 0; i <= currentIndex; i++) {
		total += frametimes[i];
	}
	total /= (double)currentIndex + 1.0f;
	if (currentIndex < FRAME_SAMPLE -1) {
		currentIndex++;
	}
	else {
		currentIndex = 0;
	}
	averageFrameTime = total;
	averageFPS = 1000.f / averageFrameTime;
}

float yaw = 0.0f;
float pitch = 0.0f;

cl_float3* Application::updateCamera(Scene& scene, float deltaTime)
{
	int xpos, ypos;
	SDL_GetMouseState(&xpos, &ypos);
	SDL_WarpMouseInWindow(window, APP_WIDTH/2, APP_HEIGHT/2);
	float xoffset = xpos-APP_WIDTH/2;
	float yoffset =  APP_HEIGHT/2-ypos;
	float sensitivity = 0.1f;
	xoffset *= -sensitivity;
	yoffset *= -sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	cl_float3 rotYaw[3] = { cl_float3{1, 0, 0},
							cl_float3{0, cosf((pitch*M_PI)/180), -sinf((pitch*M_PI)/180)},
							cl_float3{0, sinf((pitch*M_PI/180)), cosf((pitch*M_PI)/180)} };
	cl_float3 rotPitch[3] = { cl_float3{cosf((yaw*M_PI)/180), 0, -sinf((yaw * M_PI) / 180)},
							cl_float3{0, 1, 0},
							cl_float3{sinf((yaw * M_PI) / 180), 0, cosf((yaw * M_PI) / 180)}};
	cl_float3* rot = new cl_float3[3];
	rot[0] = cl_float3{ rotPitch[0].x * rotYaw[0].x + rotPitch[0].y * rotYaw[1].x + rotPitch[0].z * rotYaw[2].x, rotPitch[0].x * rotYaw[0].y + rotPitch[0].y * rotYaw[1].y + rotPitch[0].z * rotYaw[2].y, rotPitch[0].x * rotYaw[0].z + rotPitch[0].y * rotYaw[1].z + rotPitch[0].z * rotYaw[2].z };
	rot[1] = cl_float3{ rotPitch[1].x * rotYaw[0].x + rotPitch[1].y * rotYaw[1].x + rotPitch[1].z * rotYaw[2].x, rotPitch[1].x * rotYaw[0].y + rotPitch[1].y * rotYaw[1].y + rotPitch[1].z * rotYaw[2].y, rotPitch[1].x * rotYaw[0].z + rotPitch[1].y * rotYaw[1].z + rotPitch[1].z * rotYaw[2].z };
	rot[2] = cl_float3{ rotPitch[2].x * rotYaw[0].x + rotPitch[2].y * rotYaw[1].x + rotPitch[2].z * rotYaw[2].x, rotPitch[2].x * rotYaw[0].y + rotPitch[2].y * rotYaw[1].y + rotPitch[2].z * rotYaw[2].y, rotPitch[2].x * rotYaw[0].z + rotPitch[2].y * rotYaw[1].z + rotPitch[2].z * rotYaw[2].z };

	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_W]) {
		scene.cameraPos = cl_float3{ (rot[0].x * 0.0f + rot[0].y * 0.0f + rot[0].z * CAMERA_SPEED * deltaTime)+scene.cameraPos.x,
	  (rot[1].x * 0.0f + rot[1].y * 0.0f + rot[1].z * CAMERA_SPEED *deltaTime)+scene.cameraPos.y,
	  (rot[2].x * 0.0f + rot[2].y * 0.0f + rot[2].z * CAMERA_SPEED *deltaTime)+scene.cameraPos.z };
	}
	if (state[SDL_SCANCODE_S]) {
		scene.cameraPos = cl_float3{ (rot[0].x * 0.0f + rot[0].y * 0.0f + rot[0].z * -CAMERA_SPEED * deltaTime) + scene.cameraPos.x,
	  (rot[1].x * 0.0f + rot[1].y * 0.0f + rot[1].z * -CAMERA_SPEED * deltaTime) + scene.cameraPos.y,
	  (rot[2].x * 0.0f + rot[2].y * 0.0f + rot[2].z * -CAMERA_SPEED * deltaTime) + scene.cameraPos.z };
	}
	if (state[SDL_SCANCODE_D]) {
		scene.cameraPos = cl_float3{ (rot[0].x * CAMERA_SPEED * deltaTime + rot[0].y * 0.0f + rot[0].z * 0.0f) + scene.cameraPos.x,
	  (rot[1].x * CAMERA_SPEED * deltaTime + rot[1].y * 0.0f + rot[1].z * 0.0f) + scene.cameraPos.y,
	  (rot[2].x * CAMERA_SPEED * deltaTime + rot[2].y * 0.0f + rot[2].z * 0.0f) + scene.cameraPos.z };
	}
	if (state[SDL_SCANCODE_A]) {
		scene.cameraPos = cl_float3{ (rot[0].x * -CAMERA_SPEED * deltaTime + rot[0].y * 0.0f + rot[0].z * 0.0f) + scene.cameraPos.x,
	  (rot[1].x * -CAMERA_SPEED * deltaTime + rot[1].y * 0.0f + rot[1].z * 0.0f) + scene.cameraPos.y,
	  (rot[2].x * -CAMERA_SPEED * deltaTime + rot[2].y * 0.0f + rot[2].z * 0.0f) + scene.cameraPos.z };
	}
	return rot;
}
