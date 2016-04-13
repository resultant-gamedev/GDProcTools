/**
 * GDProcTools.h
 * v. 0.0.2
 *
 * Definition for GDProcTools class.
 *
 * Copyright Renato Carvalho 2016
 * Author: Renato Carvalho
 */

#ifndef GDPROCTOOLS_H_
#define GDPROCTOOLS_H_

#include "reference.h"
#include "dictionary.h"
#include "math_2d.h"
#include "math_funcs.h"

#include "Perlin.h"

#include "color.h"
#include "image.h"
#include "scene/resources/material.h"
#include "scene/resources/texture.h"
#include "scene/resources/mesh.h"
#include "scene/resources/surface_tool.h"

class GDProcTools : public Reference {
    OBJ_TYPE(GDProcTools, Reference);

public:
	GDProcTools();
    ~GDProcTools();

    void setSeed(int seed);

	// Generates a Perlin (smoothed) noise value between -1 and 1, at the given 3D position.
	float perlinNoise(float sample_x, float sample_y, float sample_z);
    Dictionary getSimpleNoiseMap(int width, int height, float scale, int seed);
    Ref<FixedMaterial> getNoiseMaterial(int width, int height, Dictionary noiseMap);
    Ref<Mesh> generateMeshFromMap(int width, int height, Dictionary heightMap, int heightMultiplier, float heightCurve);

    Image getNoiseImage(int width, int height, Dictionary noiseMap);
    Ref<ImageTexture> getTextureFromImage(Image img);
    Ref<FixedMaterial> getMaterialFromTexture(Ref<ImageTexture> texture);

private:
    Perlin* perlin;

protected:
    static void _bind_methods();

};

#endif
