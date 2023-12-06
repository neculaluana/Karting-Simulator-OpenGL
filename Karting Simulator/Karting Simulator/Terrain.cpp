#include "Terrain.h"
Terrain::Terrain(int width, int height, const std::string& heightmapPath, float scale)
    : width(width), height(height), scale(scale)
{
    terrainShader = new Shader("terrainVertexShader.vs", "terrainFragmentShader.fs");
    loadHeightmap(heightmapPath);
    generateTerrain();
    setupMesh();
}

Terrain::~Terrain()
{
    delete terrainShader;
}