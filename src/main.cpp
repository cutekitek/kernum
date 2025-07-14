#define SDL_MAIN_USE_CALLBACKS 1
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "noise.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_sdlrenderer3.h"
#include "render/WorldRenderer.h"
#include "world/chunks/ChunkManager.h"
#include "world/tiles/TileManager.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

std::shared_ptr<TileManager> tileManager;
std::shared_ptr<ChunkManager> chunkManager;
std::shared_ptr<TextureManager> textureManager;
std::shared_ptr<WorldRenderer> worldRenderer;



auto init_imgui(SDL_Window *window, SDL_Renderer *renderer) -> auto {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableGamepad;

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    style.FontSizeBase = 20.f;
    io.Fonts->AddFontDefault();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    init_imgui(window, renderer);

    textureManager = std::make_shared<TextureManager>("resources/textures", renderer);
    tileManager = std::make_shared<TileManager>(textureManager);
    tileManager->CreateTile("grass", "grass.png", true);
    tileManager->CreateTile("water", "water.png", true);
    auto cfg = std::make_shared<GeneratorConfig>(tileManager);
    auto chunkGenerator = std::make_shared<ChunkGenerator>(cfg, 0);
    chunkManager = std::make_shared<ChunkManager>(chunkGenerator, tileManager);
    worldRenderer = std::make_shared<WorldRenderer>(chunkManager);
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    worldRenderer->dispatchEvent(event);
    ImGui_ImplSDL3_ProcessEvent(event);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{

    SDL_RenderClear(renderer);
    worldRenderer->render(renderer, window);
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

}
