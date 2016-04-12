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


class GDPerlin : public Reference {
    OBJ_TYPE(Sumator,Reference);

public:
	Perlin();
	~Perlin();

	// Generates a Perlin (smoothed) noise value between -1 and 1, at the given 3D position.
	float noise(float sample_x, float sample_y, float sample_z);

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
