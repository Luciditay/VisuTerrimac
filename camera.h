#ifndef CAMERA_H
#define CAMERA_H

struct Point3D;

Triangle *triangleCamera(Point3D camPos, Point3D lookAtVector, float fov, float zFar);

bool isInTriangle(Triangle* triangle, Point2D p1);
bool intersectionSegment(Point2D pA, Point2D pB, Point2D pC, Point2D pD);
bool intersectionCameraNode(Triangle *triangle, Node *node);

#endif