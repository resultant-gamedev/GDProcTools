/**
 * Perlin.h
 * v. 1.0.0
 *
 * Definition for Perlin class. An instantiated Perlin object can generate smoothed Perlin noise by calling the noise() function.
 *
 * Copyright Chris Little 2012
 * Author: Chris Little
 */

#ifndef GDPROCTOOLS_H_
#define GDPROCTOOLS_H_

#include "reference.h"
#include "variant.h"
#include "dictionary.h"
#include "math_2d.h"
#include "math_funcs.h"

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

	// Generates a Perlin (smoothed) noise value between -1 and 1, at the given 3D position.
	float noise(float sample_x, float sample_y, float sample_z);
    Dictionary getSimpleNoiseMap(int width, int height, float scale, int seed);
    Ref<FixedMaterial> getNoiseMaterial(int width, int height, Dictionary noiseMap);
    Ref<Mesh> generateMeshFromMap(int width, int height, Dictionary heightMap, int heightMultiplier, float heightCurve);

    Image getNoiseImage(int width, int height, Dictionary noiseMap);
    Ref<ImageTexture> getTextureFromImage(Image img);
    Ref<FixedMaterial> getMaterialFromTexture(Ref<ImageTexture> texture);

protected:
    static void _bind_methods();

private:
	int *p; // Permutation table
	// Gradient vectors
	float *Gx;
	float *Gy;
	float *Gz;
};

#endif
