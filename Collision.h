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

bool IsCollision(const Sphere& s1, const Sphere& s2);

bool IsCollisionPlane(const Sphere& s1, const Plane& p1);

bool isLineCollisionPlame(const Segment& segment, const Plane& plane);

bool isTraiangleCollisionLine(const Triangle& tri, const Segment& segment);

bool isAABBToAABBCollision(const AABB& aabb1, const AABB& aabb2);

bool isAABBToSphereCollision(const AABB& aabb, const Sphere& sphere);

bool isAABBToLineCollision(const AABB& aabb, const Segment& segment);
