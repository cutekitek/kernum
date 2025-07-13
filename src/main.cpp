#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "vector"
#include "include/noise.h"
#include "include/imgui/imgui_impl_sdl3.h"
#include "include/imgui/imgui_impl_sdlrenderer3.h"


static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

FastNoiseLite noise;
SDL_Texture* texture;
std::vector<uint8_t> noiseData;
float freq = 0.01;
int seed = 0;
int octaves = 4;
float shift = 0;



auto init_imgui(SDL_Window *window, SDL_Renderer *renderer) -> auto {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup scaling
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(
            main_scale); // Bake a fixed style scale. (until we have a
    // solution for dynamic style scaling, changing this
    // requires resetting Style + calling this again)
    // makes this unnecessary. We leave both here for
    // documentation purpose)
    style.FontScaleDpi = main_scale;

    style.FontSizeBase = 20.f;
    io.Fonts->AddFontDefault();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}


void generate_world(double x, double y) {
    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        for (int j = 0; j < SCREEN_HEIGHT; ++j) {
            auto height = noise.GetNoise(i + x, j + y);
            if (height < shift) {
                noiseData[(j * SCREEN_WIDTH + i) * 3] = 0;
                noiseData[(j * SCREEN_WIDTH + i) * 3 + 1] = 255;
                noiseData[(j * SCREEN_WIDTH + i) * 3 + 2] = 0;
            } else {
                noiseData[(j * SCREEN_WIDTH + i) * 3] = 3;
                noiseData[(j * SCREEN_WIDTH + i) * 3 + 1] = 136;
                noiseData[(j * SCREEN_WIDTH + i) * 3 + 2] = 252;
            }

        }
    }
    SDL_UpdateTexture(texture, nullptr, noiseData.data(), texture->w*3);
}

void renderUI() {
    ImGui::Begin("Params");
    if (ImGui::InputFloat("frequency", &freq, 0.01, 0.1)) {
        noise.SetFrequency(freq);
        generate_world(0, 0);
    }
    if (ImGui::InputInt("seed", &seed)) {
        noise.SetSeed(seed);
        generate_world(0, 0);
    }

    if (ImGui::InputInt("octaves", &octaves)) {
        noise.SetFractalOctaves(octaves);
        generate_world(0, 0);
    }
    if (ImGui::InputFloat("shift", &shift)) {
        generate_world(0, 0);
    };
    ImGui::End();

}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    init_imgui(window, renderer);

    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetSeed(0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    noiseData = std::vector<uint8_t>(SCREEN_WIDTH * SCREEN_HEIGHT * 3);
    generate_world(0, 0);
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    ImGui_ImplSDL3_ProcessEvent(event);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    renderUI();
    ImGui::Render();

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
