/**
 * Perlin.cpp
 *
 * Copyright Chris Little 2012
 * Author: Chris Little
 */

#include "GDProcTools.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

GDProcTools::GDProcTools() {
	srand(time(NULL));

	p = new int[256];
	Gx = new float[256];
	Gy = new float[256];
	Gz = new float[256];

	for (int i=0; i<256; ++i) {
		p[i] = i;

		Gx[i] = (float(rand()) / (RAND_MAX/2)) - 1.0f;
		Gy[i] = (float(rand()) / (RAND_MAX/2)) - 1.0f;
		Gz[i] = (float(rand()) / (RAND_MAX/2)) - 1.0f;
	}

	int j=0;
	int swp=0;
	for (int i=0; i<256; i++) {
		j = rand() & 255;

		swp = p[i];
		p[i] = p[j];
		p[j] = swp;
	}
}

GDProcTools::~GDProcTools()
{
	delete p;
	delete Gx;
	delete Gy;
	delete Gz;
}


float GDProcTools::noise(float sample_x, float sample_y, float sample_z)
{
	// Unit cube vertex coordinates surrounding the sample point
	int x0 = int(floorf(sample_x));
	int x1 = x0 + 1;
	int y0 = int(floorf(sample_y));
	int y1 = y0 + 1;
	int z0 = int(floorf(sample_z));
	int z1 = z0 + 1;

	// Determine sample point position within unit cube
	float px0 = sample_x - float(x0);
	float px1 = px0 - 1.0f;
	float py0 = sample_y - float(y0);
	float py1 = py0 - 1.0f;
	float pz0 = sample_z - float(z0);
	float pz1 = pz0 - 1.0f;

	// Compute dot product between gradient and sample position vector
	int gIndex = p[(x0 + p[(y0 + p[z0 & 255]) & 255]) & 255];
	float d000 = Gx[gIndex]*px0 + Gy[gIndex]*py0 + Gz[gIndex]*pz0;
	gIndex = p[(x1 + p[(y0 + p[z0 & 255]) & 255]) & 255];
	float d001 = Gx[gIndex]*px1 + Gy[gIndex]*py0 + Gz[gIndex]*pz0;

	gIndex = p[(x0 + p[(y1 + p[z0 & 255]) & 255]) & 255];
	float d010 = Gx[gIndex]*px0 + Gy[gIndex]*py1 + Gz[gIndex]*pz0;
	gIndex = p[(x1 + p[(y1 + p[z0 & 255]) & 255]) & 255];
	float d011 = Gx[gIndex]*px1 + Gy[gIndex]*py1 + Gz[gIndex]*pz0;

	gIndex = p[(x0 + p[(y0 + p[z1 & 255]) & 255]) & 255];
	float d100 = Gx[gIndex]*px0 + Gy[gIndex]*py0 + Gz[gIndex]*pz1;
	gIndex = p[(x1 + p[(y0 + p[z1 & 255]) & 255]) & 255];
	float d101 = Gx[gIndex]*px1 + Gy[gIndex]*py0 + Gz[gIndex]*pz1;

	gIndex = p[(x0 + p[(y1 + p[z1 & 255]) & 255]) & 255];
	float d110 = Gx[gIndex]*px0 + Gy[gIndex]*py1 + Gz[gIndex]*pz1;
	gIndex = p[(x1 + p[(y1 + p[z1 & 255]) & 255]) & 255];
	float d111 = Gx[gIndex]*px1 + Gy[gIndex]*py1 + Gz[gIndex]*pz1;

	// Interpolate dot product values at sample point using polynomial interpolation 6x^5 - 15x^4 + 10x^3
	float wx = ((6*px0 - 15)*px0 + 10)*px0*px0*px0;
	float wy = ((6*py0 - 15)*py0 + 10)*py0*py0*py0;
	float wz = ((6*pz0 - 15)*pz0 + 10)*pz0*pz0*pz0;

	float xa = d000 + wx*(d001 - d000);
	float xb = d010 + wx*(d011 - d010);
	float xc = d100 + wx*(d101 - d100);
	float xd = d110 + wx*(d111 - d110);
	float ya = xa + wy*(xb - xa);
	float yb = xc + wy*(xd - xc);
	float value = ya + wz*(yb - ya);

	return value;
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

            float perlinValue = noise(sampleX, sampleY, seed);
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
    ObjectTypeDB::bind_method(_MD("noise","x","y","z"),&GDProcTools::noise);
    ObjectTypeDB::bind_method(_MD("getSimpleNoiseMap","width", "height", "scale", "seed"),&GDProcTools::getSimpleNoiseMap);
	ObjectTypeDB::bind_method(_MD("getNoiseMaterial","width", "height", "noise map"),&GDProcTools::getNoiseMaterial);
	ObjectTypeDB::bind_method(_MD("generateMeshFromMap", "width", "height", "height map", "height multiplier", "height curve"),&GDProcTools::generateMeshFromMap);

	ObjectTypeDB::bind_method(_MD("getNoiseImage", "width", "height", "noise map"),&GDProcTools::getNoiseImage);
    ObjectTypeDB::bind_method(_MD("getTextureFromImage", "noise"),&GDProcTools::getTextureFromImage);
    ObjectTypeDB::bind_method(_MD("getMaterialFromTexture", "texture"),&GDProcTools::getMaterialFromTexture);
}
