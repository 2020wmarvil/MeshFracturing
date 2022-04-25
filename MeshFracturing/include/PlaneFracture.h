#ifndef PLANE_FRACTURE_H
#define PLANE_FRACTURE_H

#include <glm/glm.hpp>

#include "Plane.h"
#include "FractureComponent.h"

glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float t) {
    return a + (b - a) * t;
}

glm::vec2 Lerp(glm::vec2 a, glm::vec2 b, float t) {
    return a + (b - a) * t;
}

void Fracture(const Mesh& mesh, const Plane& plane, FractureComponent& leftFracture, FractureComponent& rightFracture) {
	// clear the given fracture components

	std::vector<glm::vec3> intersectionPoints;

	for (int i = 0; i < mesh.indices.size(); i += 3) {
		// for each triangle, intersect with the plane
        Vertex v1 = mesh.vertices[mesh.indices[i + 0]];
        Vertex v2 = mesh.vertices[mesh.indices[i + 1]];
        Vertex v3 = mesh.vertices[mesh.indices[i + 2]];

        glm::vec3 p1 = v1.position;
        glm::vec3 p2 = v2.position;
        glm::vec3 p3 = v2.position;

        glm::vec3 n1 = v1.normal;
        glm::vec3 n2 = v2.normal;
        glm::vec3 n3 = v3.normal;

        glm::vec2 uv1 = v1.texCoords;
        glm::vec2 uv2 = v2.texCoords;
        glm::vec2 uv3 = v3.texCoords;

        bool p1Left = plane.GetSide(p1);
        bool p2Left = plane.GetSide(p2);
        bool p3Left = plane.GetSide(p3);

        if (p1Left == p2Left && p1Left == p3Left) { // all are to the same side
            if (p1Left) {
                leftFracture.AddTriangle(p1, p2, p3, n1, n2, n3, uv1, uv2, uv3);
			} else {
                rightFracture.AddTriangle(p1, p2, p3, n1, n2, n3, uv1, uv2, uv3);
			}
        } else if (p1Left == p2Left) { // only p1 and p2 are same side
            float t1 = plane.SegementPlaneIntersectionTime(p1, p3);
            float t2 = plane.SegementPlaneIntersectionTime(p2, p3);

            // calculate intersection values
            glm::vec3 interVert1 = Lerp(p1, p3, t1);
            glm::vec3 interVert2 = Lerp(p2, p3, t2);
            glm::vec3 interNorm1 = Lerp(n1, n3, t1);
            glm::vec3 interNorm2 = Lerp(n2, n3, t2);
            glm::vec2 interUV1 = Lerp(uv1, uv3, t1);
            glm::vec2 interUV2 = Lerp(uv2, uv3, t2);

            intersectionPoints.push_back(interVert1);
            intersectionPoints.push_back(interVert2);

            if (p1Left) {
                leftFracture.AddTriangle(p1, p2, interVert1, n1, n2, interNorm1, uv1, uv2, interUV1);
                leftFracture.AddTriangle(p2, interVert2, interVert1, n2, interNorm2, interNorm1, uv2, interUV2, interUV1);
                rightFracture.AddTriangle(p3, interVert1, interVert2, n3, interNorm1, interNorm2, uv3, interUV1, interUV2);
            } else {
                rightFracture.AddTriangle(p1, p2, interVert1, n1, n2, interNorm1, uv1, uv2, interUV1);
                rightFracture.AddTriangle(p2, interVert2, interVert1, n2, interNorm2, interNorm1, uv2, interUV2, interUV1);
                leftFracture.AddTriangle(p3, interVert1, interVert2, n3, interNorm1, interNorm2, uv3, interUV1, interUV2);
			}
        } else if (p1Left == p3Left) { // only p1 and p3 are same side
            float t1 = plane.SegementPlaneIntersectionTime(p1, p2);
            float t2 = plane.SegementPlaneIntersectionTime(p3, p2);

            // calculate intersection values
            glm::vec3 interVert1 = Lerp(p1, p2, t1);
            glm::vec3 interVert2 = Lerp(p3, p2, t2);
            glm::vec3 interNorm1 = Lerp(n1, n2, t1);
            glm::vec3 interNorm2 = Lerp(n3, n2, t2);
            glm::vec2 interUV1 = Lerp(uv1, uv2, t1);
            glm::vec2 interUV2 = Lerp(uv3, uv2, t2);

            intersectionPoints.push_back(interVert1);
            intersectionPoints.push_back(interVert2);

            if (p1Left) {
                leftFracture.AddTriangle(p1, interVert1, interVert2, n1, interNorm1, interNorm2, uv1, interUV1, interUV2);
                leftFracture.AddTriangle(p1, interVert2, p3, n1, interNorm2, n3, uv1, interUV2, uv3);
                rightFracture.AddTriangle(p2, interVert2, interVert1, n2, interNorm2, interNorm1, uv2, interUV2, interUV1);
			} else {
                rightFracture.AddTriangle(p1, interVert1, interVert2, n1, interNorm1, interNorm2, uv1, interUV1, interUV2);
                rightFracture.AddTriangle(p1, interVert2, p3, n1, interNorm2, n3, uv1, interUV2, uv3);
                leftFracture.AddTriangle(p2, interVert2, interVert1, n2, interNorm2, interNorm1, uv2, interUV2, interUV1);
			}
        } else if (p2Left == p3Left) { // only p2 and p3 are sameside
            float t1 = plane.SegementPlaneIntersectionTime(p2, p1);
            float t2 = plane.SegementPlaneIntersectionTime(p3, p1);

            // calculate intersection values
            glm::vec3 interVert1 = Lerp(p2, p1, t1);
            glm::vec3 interVert2 = Lerp(p3, p1, t2);
            glm::vec3 interNorm1 = Lerp(n2, n1, t1);
            glm::vec3 interNorm2 = Lerp(n3, n1, t2);
            glm::vec2 interUV1 = Lerp(uv2, uv1, t1);
            glm::vec2 interUV2 = Lerp(uv3, uv1, t2);

            intersectionPoints.push_back(interVert1);
            intersectionPoints.push_back(interVert2);

            if (p2Left) {
                leftFracture.AddTriangle(p2, interVert2, interVert1, n2, interNorm2, interNorm1, uv2, interUV2, interUV1);
                leftFracture.AddTriangle(p2, p3, interVert2, n2, n3, interNorm2, uv2, uv3, interUV2);
                rightFracture.AddTriangle(p1, interVert1, interVert2, n1, interNorm1, interNorm2, uv1, interUV1, interUV2);
			} else {
                rightFracture.AddTriangle(p2, interVert2, interVert1, n2, interNorm2, interNorm1, uv2, interUV2, interUV1);
                rightFracture.AddTriangle(p2, p3, interVert2, n2, n3, interNorm2, uv2, uv3, interUV2);
                leftFracture.AddTriangle(p1, interVert1, interVert2, n1, interNorm1, interNorm2, uv1, interUV1, interUV2);
			}
        } 
	} // end for loop

    // do i need to sort the intersection points clockwise first?
    if (intersectionPoints.size() > 0) {
        glm::vec3 center(0.0f); // find the center of mass of the cut face
        for (int i = 0; i < intersectionPoints.size(); i++) {
            center += intersectionPoints[i];
        } center /= intersectionPoints.size();

        // default uv values, not sure if there is a clever way to choose these
        glm::vec2 uv(0.0f);

        // draw a triangle fan from the center of the cut face
        for (int i=0; i<intersectionPoints.size(); i+=2) {
            int nextI = (i + 1) % intersectionPoints.size();

            glm::vec3 p1 = intersectionPoints[i];
            glm::vec3 p2 = intersectionPoints[nextI];

            glm::vec3 normal = glm::cross(p2 - p1, center - p2);

            float dot = glm::dot<3, float, glm::qualifier::highp>(normal, plane.normal);
            if (dot < 0.0f) {
                leftFracture.AddTriangle(p1, p2, center, normal, normal, normal, uv, uv, uv);
                rightFracture.AddTriangle(p1, center, p2, -normal, -normal, -normal, uv, uv, uv); // normals should be opposite of each other
			} else {
                leftFracture.AddTriangle(p1, center, p2, -normal, -normal, -normal, uv, uv, uv);
                rightFracture.AddTriangle(p1, p2, center, normal, normal, normal, uv, uv, uv); // normals should be opposite of each other
			}
		}
	}
}

#endif
