// Based on http://stackoverflow.com/questions/4753055/perlin-noise-generation-for-terrain

/* GDPerlin.h */
#ifndef GDPERLIN_H
#define GDPERLIN_H

#include "reference.h"

class GDPerlin : public Reference
{
    OBJ_TYPE(GDPerlin, Reference);
    
public:
    // Constructor
    GDPerlin();
    GDPerlin(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed);

    // Get Height
    double GetHeight(double x, double y) const;

    // Get
    double Persistence() const { return persistence; }
    double Frequency()   const { return frequency;   }
    double Amplitude()   const { return amplitude;   }
    int    Octaves()     const { return octaves;     }
    int    RandomSeed()  const { return randomseed;  }

    // Set
    void Set(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed);

    void SetPersistence(double _persistence) { persistence = _persistence; }
    void SetFrequency(  double _frequency)   { frequency = _frequency;     }
    void SetAmplitude(  double _amplitude)   { amplitude = _amplitude;     }
    void SetOctaves(    int    _octaves)     { octaves = _octaves;         }
    void SetRandomSeed( int    _randomseed)  { randomseed = _randomseed;   }

protected:
    static void _bind_methods();

private:

    double Total(double i, double j) const;
    double GetValue(double x, double y) const;
    double Interpolate(double x, double y, double a) const;
    double Noise(int x, int y) const;

    double persistence, frequency, amplitude;
    int octaves, randomseed;
};

 #endif
