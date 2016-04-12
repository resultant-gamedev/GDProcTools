/**
 * Perlin.h
 * v. 1.0.0
 *
 * Definition for Perlin class. An instantiated Perlin object can generate smoothed Perlin noise by calling the noise() function.
 *
 * Copyright Chris Little 2012
 * Author: Chris Little
 */

#ifndef GDPERLIN_H_
#define GDPERLIN_H_

#include "reference.h"
#include "variant.h"
#include "dictionary.h"
#include "math_2d.h"

#include "color.h"
#include "image.h"
#include "scene/resources/material.h"
#include "scene/resources/texture.h"

class GDPerlin : public Reference {
    OBJ_TYPE(GDPerlin, Reference);

public:
	GDPerlin();
	~GDPerlin();

	// Generates a Perlin (smoothed) noise value between -1 and 1, at the given 3D position.
	float noise(float sample_x, float sample_y, float sample_z);
    Dictionary getSimpleNoiseMap(int width, int height, float scale, int seed);
    Image getNoiseImage(int width, int height, Dictionary noiseMap);
    Variant getTextureFromImage(Image img);
    Variant getMaterialFromTexture(Variant texture);

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
