#include "Grid.hpp"
#include "primitives/solid/Cube.hpp"

// debug
//#include <iostream>
//#include <glm/ext.hpp>
//using std::cout;
//using std::endl;

using glm::dvec3;

Grid::Grid(const std::vector<Primitive*>& primitives)
    : point1(std::numeric_limits<double>::max()),
      point2(std::numeric_limits<double>::lowest()) {
    initCells(primitives);
}

void Grid::initCells(const std::vector<Primitive*>& primitives) {
    // find bounding box for all primitives
    for (Primitive* primitive: primitives) {
        AABB aabb = primitive->getAABB();
        for (uint i = 0; i < 3; i++) {
            point1[i] = std::min(point1[i], aabb.p1[i]);
            point2[i] = std::max(point2[i], aabb.p2[i]);
        }
    }

    cellSize = std::min(
        point2.x - point1.x,
        std::min(
            point2.y - point1.y,
            point2.z - point1.z
        )
    ) / primitives.size();

    xCells = uint(ceil((point2.x - point1.x) / cellSize));
    yCells = uint(ceil((point2.y - point1.y) / cellSize));
    zCells = uint(ceil((point2.z - point1.z) / cellSize));

    double x = point1.x;
    for (uint i = 0; i < xCells; i++, x += cellSize) {
        grid.push_back({});
        double y = point1.y;

        for (uint j = 0; j < yCells; j++, y += cellSize) {
            grid.back().push_back({});
            double z = point1.z;

            for (uint k = 0; k < zCells; k++, z += cellSize) {
                grid.back().back().push_back(
                    Cell(
                        dvec3(x, y, z),
                        dvec3(x, y, z) + dvec3(cellSize)
                    )
                );
            }
        }
    }

    for (Primitive* primitive: primitives) {
        int insertedCount = 0;
        for (auto& xs: grid) {
            for (auto& ys: xs) {
                for (Cell& cell: ys) {
                    if (cell.intersects(primitive->getAABB())) {
                        cell.primitives.push_back(primitive);
                        insertedCount++;
                    }
                }
            }
        }
//        cout << "Primitive inserted times: " << insertedCount << endl;
        assert(insertedCount > 0 && "Every primitive must be inserted!!!");
    }
}

int sgn(double x) {
    if (x < 0) return -1;
    else if (x > 0) return 1;
    else return 0;
}

/**
 * init + dir * t = minVal
 * => t = (minVal - init) / dir
 * OR
 * init + dir * t = maxVal
 * => t = (maxVal - init) / dir
 * whichever results in a positive t
 */
double solver(double init, double dir, double minVal, double maxVal) {
    double t = (minVal - init) / dir;
    double t2 = (maxVal - init) / dir;

    if (t >= 0 && t2 >= 0) {
        return std::min(t, t2);
    }
    else if (t >= 0 && t2 < 0) {
        return t;
    }
    else if (t < 0 && t2 >= 0) {
        return t2;
    }
    else {
        return std::numeric_limits<double>::max();
    }
}

PhongMaterial dummyMaterial2;

Intersection Grid::trace(glm::dvec3 rayOrigin, glm::dvec3 rayDirection) const {
    // rayOrigin inside grid
    if (point1.x <= rayOrigin.x && rayOrigin.x <= point2.x &&
        point1.y <= rayOrigin.y && rayOrigin.y <= point2.y &&
        point1.z <= rayOrigin.z && rayOrigin.z <= point2.z) {
        ;
    }
    // rayOrigin not inside grid
    else {
        Cube cubeAABB(point1, point2 - point1);
        cubeAABB.setMaterial(&dummyMaterial2);

        const Intersection gridIntersection = cubeAABB.getClosestIntersection(rayOrigin, rayDirection);
        if (!gridIntersection.intersected) {
            return Intersection();
        }
        rayOrigin = gridIntersection.point;
    }

    const dvec3 o = (rayOrigin - point1) / cellSize;

    int xPos = int(floor(o.x));
    int yPos = int(floor(o.y));
    int zPos = int(floor(o.z));

    xPos = std::max(0, std::min(xPos, int(xCells - 1)));
    yPos = std::max(0, std::min(yPos, int(yCells - 1)));
    zPos = std::max(0, std::min(zPos, int(zCells - 1)));

//    cout << endl;
//    cout << "(x, y, z) = " << xPos << ", " << yPos << ", " << zPos << endl;

    // http://www.cs.yorku.ca/~amana/research/grid.pdf
    const int stepX = sgn(rayDirection.x);
    const int stepY = sgn(rayDirection.y);
    const int stepZ = sgn(rayDirection.z);

//    cout << "rayDirection = " << rayDirection.x << ", " << rayDirection.y << ", " << rayDirection.z << endl;
//    cout << "(stepX, stepY, stepZ) = " << stepX << ", " << stepY << ", " << stepZ << endl;


    const Cell& initCell = grid.at(xPos).at(yPos).at(zPos);

    double tMaxX = solver(rayOrigin.x, rayDirection.x, initCell.p1.x, initCell.p2.x);
    double tMaxY = solver(rayOrigin.y, rayDirection.y, initCell.p1.y, initCell.p2.y);
    double tMaxZ = solver(rayOrigin.z, rayDirection.z, initCell.p1.z, initCell.p2.z);

    const double tDeltaX = fabs(cellSize / rayDirection.x);
    const double tDeltaY = fabs(cellSize / rayDirection.y);
    const double tDeltaZ = fabs(cellSize / rayDirection.z);

    Intersection closest;
    while (true) {
//        cout << "(tMaxX, tMaxY, tMaxZ) = " << tMaxX << ", " << tMaxY << ", " << tMaxZ << endl;
//        cout << "(x, y, z) = " << xPos << ", " << yPos << ", " << zPos << endl;

        const Cell& cell = grid.at(xPos).at(yPos).at(zPos);
        for (Primitive* primitive: cell.primitives) {
            closest = Intersection::min(
                rayOrigin,
                closest,
                primitive->getClosestIntersection(rayOrigin, rayDirection)
            );
        }

        if (tMaxX <= tMaxY && tMaxX <= tMaxZ) {
            xPos += stepX;
            if (xPos < 0 || xPos >= xCells) {
                return closest;
            }
            tMaxX += tDeltaX;
        }
        else if (tMaxY <= tMaxX && tMaxY <= tMaxZ) {
            yPos += stepY;
            if (yPos < 0 || yPos >= yCells) {
                return closest;
            }
            tMaxY += tDeltaY;
        }
        else if (tMaxZ <= tMaxX && tMaxZ <= tMaxY) {
            zPos += stepZ;
            if (zPos < 0 || zPos >= zCells) {
                return closest;
            }
            tMaxZ += tDeltaZ;
        }
        else {
            assert(false);
        }
    }
}

