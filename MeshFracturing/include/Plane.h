#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>

// a Plane is represented as N dot X + d = 0
class Plane {
public:
	glm::vec3 normal;
    float d;

    Plane(glm::vec3 normal, glm::vec3 origin) {
        this->normal = normal;
        this->d = glm::length(origin);
	}

    // point as a function of the plane equation. returns the height above the plane.
    float F(glm::vec3 point) const { 
        return glm::dot<3, float, glm::qualifier::highp>(normal, point) + d;
	}

    bool GetSide(glm::vec3 point) const {
        return F(point) > 0.0f;
	}

    // get intersection time of line segment with plane
    float SegementPlaneIntersectionTime(glm::vec3 p1, glm::vec3 p2) const {
        float d1 = F(p1);
        float d2 = F(p2);

        float t = d1 / (d1 - d2);

        return t;
	}
};

#endif
