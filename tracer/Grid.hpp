#pragma once

#include "primitives/Primitive.hpp"


struct Cell : public AABB {
    Cell(
        const glm::dvec3& p1,
        const glm::dvec3& p2
    ) : AABB(p1, p2) {}

    std::vector<Primitive*> primitives;
};


class Grid {
public:
    Grid() {}
    Grid(const std::vector<Primitive*>& primitives);

    Intersection trace(
        glm::dvec3 rayOrigin,
        glm::dvec3 rayDirection
    ) const;

private:
    void initCells(const std::vector<Primitive*>& primitives);

    std::vector< std::vector< std::vector<Cell> > > grid;

    glm::dvec3 point1;
    glm::dvec3 point2;

    double cellSize;

    uint xCells;
    uint yCells;
    uint zCells;
};



