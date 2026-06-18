#pragma once
#include "Vector.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <algorithm>
#include "Sphere.h"
#include "plane.h"
#include "Segment.h"
#include "Triangle.h"
#include "DarwBox.h"
#include "OBB.h"
bool IsCollision(const Sphere& s1, const Sphere& s2);

bool IsCollisionPlane(const Sphere& s1, const Plane& p1);

bool isLineCollisionPlame(const Segment& segment, const Plane& plane);

bool isTraiangleCollisionLine(const Triangle& tri, const Segment& segment);


// AABB
bool isAABBToAABBCollision(const AABB& aabb1, const AABB& aabb2);
bool isAABBToSphereCollision(const AABB& aabb, const Sphere& sphere);   
bool isAABBToSegmentCollision(const AABB& aabb, const Segment& segment);
bool isAABBToLineCollision(const AABB& obb, const Line& line);

//OBB
bool isOBBToSphereCollision(const OBB& obb, const Sphere& sphere);
bool isOBBToSegmentCollsion(const OBB& obb, const Segment& segment);
//bool isOBBToRayCollision(const OBB&obb,const Ray&ray)
bool isOBBToLineCollision(const OBB& obb, const Line& line);
bool isOBBToOBBCollision(const OBB& obb1, const OBB& obb2);