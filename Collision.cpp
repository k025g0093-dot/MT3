#include "Collision.h"

bool IsCollision(const Sphere& s1, const Sphere& s2) {

	float distnce = Length(SubtractVector3(s2.center , s1.center));

	if (distnce <= s1.radius + s2.radius) {
		return true;
	}
	return false;
}