//
// Created by user on 02.08.2025.
//

#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H
#include "render/base/TileRenderer.h"


class ChunkRenderer {
     const Camera *camera;
     const Chunk* chunk;
     std::unique_ptr<TileRenderer> terrainRenderer;
     std::unique_ptr<TileRenderer> oreRenderer;
     std::shared_ptr<TextureAtlas> oreAtlas;


     void initTerrainRenderer(const std::shared_ptr<TileManager>& tileManager);
     void initOreRenderer(const std::shared_ptr<TileManager>& tileManager);

     public:
     ChunkRenderer(const Chunk* chunk, const std::shared_ptr<TileManager>& tileManager, const Camera *camera, const std::shared_ptr<TextureAtlas> &oreAtlas) : camera(camera), chunk(chunk), oreAtlas(oreAtlas) {
          initTerrainRenderer(tileManager);
          initOreRenderer(tileManager);
     };
     void Render();
};



#endif //CHUNKRENDERER_H
