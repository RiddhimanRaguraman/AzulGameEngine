//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef RITTER_H
#define RITTER_H

#include "MathEngine.h"
#include "Mesh.h"

namespace Azul
{
	struct Sphere
	{
		Vec3f cntr;
		float rad;
		float pad[3];  // love and hate this new compiler
	};

	void MostSeparatedPointsOnAABB(int &min, int &max, Vec3f *pt, int numPts);

	void SphereOfSphereAndPt(Sphere &s, Vec3f &p);

	void SphereFromDistantPoints(Sphere &s, Vec3f *pt, int numPts);

	void RitterSphere(Sphere &s, Vec3f *pt, int numPts);

}

#endif

// --- End of File ---