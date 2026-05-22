#pragma once
#include "Vector.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include "Sphere.h"
#include "plane.h"

bool IsCollision(const Sphere& s1, const Sphere& s2);

bool IsCollisionPlane(const Sphere& s1, const Plane& p1);
