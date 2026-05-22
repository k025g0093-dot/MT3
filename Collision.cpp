#include "Collision.h"

bool IsCollision(const Sphere& s1, const Sphere& s2) {

	float distnce = Length(SubtractVector3(s2.center , s1.center));

	if (distnce <= s1.radius + s2.radius) {
		return true;
	}
	return false;
}

bool IsCollisionPlane(const Sphere& s1, const Plane& p1) {

    float distance = Dot(p1.normal, s1.center) - p1.distance;
    float absDistance = std::abs(distance);
    if (absDistance <= s1.radius) {
        return true;
    }
    return false;
}