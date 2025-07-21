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
#include "render/TileRenderer.h"
#include "world/chunks/ChunkManager.h"
#include "world/tiles/TileManager.h"
#include "glad/glad.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

std::shared_ptr<TileManager> tileManager;
std::shared_ptr<ChunkManager> chunkManager;
std::shared_ptr<TileRenderer> worldRenderer;
std::vector<uint32_t> tiles = std::vector<uint32_t>(SCREEN_WIDTH * SCREEN_HEIGHT, 0);
const char* frag = R"(
#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tilesetTexture;

void main()
{
    FragColor = texture(tilesetTexture, TexCoord);
}
)";

const char* vert = R"(
#version 430 core
layout (location = 0) in vec2 aPos;         // Now in [0, 1] range
layout (location = 1) in vec4 aInstanceUVs;

out vec2 TexCoord;

uniform mat4 projection;
uniform float tileSize;
uniform int cols;

void main()
{
    float instancePosX = float(gl_InstanceID % cols) * tileSize;
    float instancePosY = float(gl_InstanceID / cols) * tileSize;

    vec2 finalPos = (aPos * tileSize) + vec2(instancePosX, instancePosY);

    gl_Position = projection * vec4(finalPos, 0.0, 1.0);

    TexCoord.x = mix(aInstanceUVs.x, aInstanceUVs.z, aPos.x);
    TexCoord.y = mix(aInstanceUVs.y, aInstanceUVs.w, aPos.y);
}
)";

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
    window  = SDL_CreateWindow("Kernum", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
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
    SDL_GL_SetSwapInterval(0);
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup scaling
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;


    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback( MessageCallback, 0 );
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!ImGui_ImplSDL3_InitForOpenGL(window, context)) {
        std::cout << "Failed to ImGui_ImplSDL3_InitForOpenGL" << std::endl;
        return SDL_APP_FAILURE;
    };
    if (!ImGui_ImplOpenGL3_Init("#version 430 core")) {
        std::cout << "Failed to ImGui_ImplOpenGL3_Init" << std::endl;
        return SDL_APP_FAILURE;
    };

    tileManager = std::make_shared<TileManager>();
    tileManager->CreateTile("grass", "grass.png", true);
    tileManager->CreateTile("water", "water.png", true);
    auto cfg = std::make_shared<GeneratorConfig>(tileManager);
    auto chunkGenerator = std::make_shared<ChunkGenerator>(cfg, 0);
    chunkManager = std::make_shared<ChunkManager>(chunkGenerator, tileManager);

    auto shader = std::make_shared<Shader>(vert, frag);
    worldRenderer = std::make_shared<TileRenderer>(shader, tileManager, 32, window);
    for (int i = 0; i < 120 * 64; i++) {
        tiles[i] = i % 2 == 0 ? 1 : 0;
    }
    worldRenderer->SetTileMap(tiles);
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    ImGui_ImplSDL3_ProcessEvent(event);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    worldRenderer->Render();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        auto io = ImGui::GetIO();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (!SDL_GL_SwapWindow(window)) {
        std::cerr << "Failed to swap window:" << SDL_GetError() << std::endl;
        return SDL_APP_FAILURE;
    };
    return SDL_APP_CONTINUE;
}


/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // --- MODIFICATION START ---
    // Add ImGui cleanup before shutting down SDL
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}