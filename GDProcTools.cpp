/**
 * GDProcTools.cpp
 *
 * Copyright Renato Carvalho 2016
 * Author: Renato Carvalho
 */

#include "GDProcTools.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

GDProcTools::GDProcTools() {
	perlin = memnew(Perlin);
}

GDProcTools::~GDProcTools() {
	memdelete(perlin);
}

void GDProcTools::setSeed(int seed)
{
	perlin->setSeed(seed);
}

float GDProcTools::perlinNoise(float sample_x, float sample_y, float sample_z)
{
	return perlin->noise(sample_x, sample_y, sample_z);
}

Dictionary GDProcTools::getSimpleNoiseMap(int width, int height, float scale, int seed) {
    Dictionary noiseMap = memnew(Dictionary);

    if (scale <= 0.0f) {
        scale = 0.0001f;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float sampleX = x / scale;
            float sampleY = y / scale;

            float perlinValue = perlinNoise(sampleX, sampleY, seed);
			noiseMap[Vector2(x, y)] = perlinValue;
        }
    }

    return noiseMap;
}

Image GDProcTools::getNoiseImage(int width, int height, Dictionary noiseMap) {
    Image noiseImage = Image(width, height, false, Image::FORMAT_RGB);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            Color color = Color(0, 0, 0, 1).linear_interpolate(Color(1, 1, 1, 1), noiseMap[Vector2(i, j)]);
            noiseImage.put_pixel(i, j, color);
        }
    }

    return noiseImage;
}

Ref<ImageTexture> GDProcTools::getTextureFromImage(Image img)
{
	Ref<ImageTexture> imgTexture = memnew(ImageTexture);
	imgTexture->set_flags(0);
    imgTexture->create_from_image(img);

    return imgTexture;
}

Ref<FixedMaterial> GDProcTools::getMaterialFromTexture(Ref<ImageTexture> texture)
{
    Ref<FixedMaterial> material = memnew(FixedMaterial);
    material->set_texture(FixedMaterial::PARAM_DIFFUSE, texture);

    return material;
}

Ref<FixedMaterial> GDProcTools::getNoiseMaterial(int width, int height, Dictionary noiseMap)
{
	Image noiseImage = getNoiseImage(width, height, noiseMap);
	Ref<ImageTexture> noiseTexture = getTextureFromImage(noiseImage);
	Ref<FixedMaterial> noiseMaterial = getMaterialFromTexture(noiseTexture);

	return noiseMaterial;
}

Ref<Mesh> GDProcTools::generateMeshFromMap(int width, int height, Dictionary heightMap, int heightMultiplier, float heightCurve)
{
	Ref<SurfaceTool> surf_tool = memnew(SurfaceTool);
	Ref<Mesh> mesh = memnew(Mesh);

	surf_tool->begin(Mesh::PRIMITIVE_TRIANGLES);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (x < width - 1 && y < height - 1)
			{
				surf_tool->add_uv(Vector2(x / float(width), y / float(height)));
				surf_tool->add_vertex(Vector3(x, Math::ease(heightMap[Vector2(x, y)], heightCurve) * heightMultiplier, y));

				surf_tool->add_uv(Vector2((x+1) / float(width), (y+1) / float(height)));
				surf_tool->add_vertex(Vector3(x+1, Math::ease(heightMap[Vector2(x+1, y+1)], heightCurve) * heightMultiplier, y+1));

				surf_tool->add_uv(Vector2(x / float(width), (y+1) / float(height)));
				surf_tool->add_vertex(Vector3(x, Math::ease(heightMap[Vector2(x, y+1)], heightCurve) * heightMultiplier, y+1));

				surf_tool->add_uv(Vector2((x+1) / float(width), (y) / float(height)));
				surf_tool->add_vertex(Vector3(x+1, Math::ease(heightMap[Vector2(x+1, y)], heightCurve) * heightMultiplier, y));

				surf_tool->add_uv(Vector2((x+1) / float(width), (y+1) / float(height)));
				surf_tool->add_vertex(Vector3(x+1, Math::ease(heightMap[Vector2(x+1, y+1)], heightCurve) * heightMultiplier, y+1));

				surf_tool->add_uv(Vector2(x / float(width), y / float(height)));
				surf_tool->add_vertex(Vector3(x, Math::ease(heightMap[Vector2(x, y)], heightCurve) * heightMultiplier, y));
			}
		}
	}

	surf_tool->index();
	surf_tool->generate_normals();
	surf_tool->commit(mesh);

	return mesh;
}

void GDProcTools::_bind_methods()
{
	ObjectTypeDB::bind_method(_MD("setSeed","seed"),&GDProcTools::setSeed);

	ObjectTypeDB::bind_method(_MD("perlinNoise","x","y","z"),&GDProcTools::perlinNoise);
    ObjectTypeDB::bind_method(_MD("getSimpleNoiseMap","width", "height", "scale", "seed"),&GDProcTools::getSimpleNoiseMap);
	ObjectTypeDB::bind_method(_MD("getNoiseMaterial","width", "height", "noise map"),&GDProcTools::getNoiseMaterial);
	ObjectTypeDB::bind_method(_MD("generateMeshFromMap", "width", "height", "height map", "height multiplier", "height curve"),&GDProcTools::generateMeshFromMap);

	ObjectTypeDB::bind_method(_MD("getNoiseImage", "width", "height", "noise map"),&GDProcTools::getNoiseImage);
    ObjectTypeDB::bind_method(_MD("getTextureFromImage", "noise"),&GDProcTools::getTextureFromImage);
    ObjectTypeDB::bind_method(_MD("getMaterialFromTexture", "texture"),&GDProcTools::getMaterialFromTexture);
}
