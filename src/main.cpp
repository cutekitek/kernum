#include <SDL3/SDL_video.h>
#define SDL_MAIN_USE_CALLBACKS 1
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_opengl3.h"
#include "world/chunks/ChunkManager.h"
#include "world/tiles/TileManager.h"
#include "glad/glad.h"
#include "render/WorldRenderer.h"

static SDL_Window *window = NULL;
std::shared_ptr<TileManager> tileManager;
std::shared_ptr<ChunkManager> chunkManager;
std::shared_ptr<WorldRenderer> worldRenderer;
Camera *camera;


void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
              type, severity, message );
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    window  = SDL_CreateWindow("Kernum", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return SDL_APP_FAILURE;
    }
    SDL_GL_SetSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        SDL_GL_DestroyContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return SDL_APP_FAILURE;
    }
    SDL_GL_MakeCurrent(window, context);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;


    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback( MessageCallback, 0 );
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (!ImGui_ImplSDL3_InitForOpenGL(window, context)) {
        std::cout << "Failed to ImGui_ImplSDL3_InitForOpenGL" << std::endl;
        return SDL_APP_FAILURE;
    };
    if (!ImGui_ImplOpenGL3_Init("#version 330 core")) {
        std::cout << "Failed to ImGui_ImplOpenGL3_Init" << std::endl;
        return SDL_APP_FAILURE;
    };

    tileManager = std::make_shared<TileManager>();
    tileManager->CreateTile("grass", true, std::vector<std::string>{"grass1.png", "grass2.png", "grass3.png"});

    tileManager->CreateTile("water", false, std::vector<std::string>{"water1.png"} );
    auto atlas = std::make_shared<TextureAtlas>("resources/textures", 1024, 1024);
    auto copper = atlas->load_texture("tiles/copper.png");
    auto iron = atlas->load_texture("tiles/iron.png");
    auto tin = atlas->load_texture("tiles/tin.png");
    auto cfg = std::make_shared<GeneratorConfig>(tileManager);
    auto ores = std::vector{OreGenerator(10, 100, 10, 0.12, copper), OreGenerator(10, 100, 11, 0.12, tin), OreGenerator(10, 100, 12, 0.12, iron)};
    // auto ores = std::vector<OreGenerator>{};
    auto chunkGenerator = std::make_shared<ChunkGenerator>(cfg, 0, ores);
    chunkManager = std::make_shared<ChunkManager>(chunkGenerator, tileManager);


    camera =  new Camera(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1.0);
    worldRenderer = std::make_shared<WorldRenderer>(chunkManager, tileManager, atlas, camera);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    switch (event->type) {
        case SDL_EVENT_WINDOW_RESIZED:
            glViewport(0, 0, event->window.data1, event->window.data2);
    }
    camera->DispatchEvent(event);
    ImGui_ImplSDL3_ProcessEvent(event);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    camera->Update();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    auto io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Stats");
    worldRenderer->Update();
    worldRenderer->Render();


    auto bbox = camera->BoundingBox();
    ImGui::Text("camera x0=%f y0=%f w=%f h=%f z=%f", bbox.x, bbox.y, bbox.width, bbox.height, camera->GetZoom());

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (!SDL_GL_SwapWindow(window)) {
        std::cerr << "Failed to swap window:" << SDL_GetError() << std::endl;
        return SDL_APP_SUCCESS;
    };
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}