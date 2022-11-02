#include "..\..\inc\algorithms\DynamicProgramming.hpp"

#include <climits>
#include <cmath>

PEA::Path* PEA::DynamicProgramming::execute(AdjanencyMatrix& matrix)
{
    this->allocateMemory(matrix.getVerticesNumber());
    Path* solution = this->examineNextLevel(matrix, 0, 0b1);
    this->deallocateMemory(matrix.getVerticesNumber());

	return new Path(*solution);
}

void PEA::DynamicProgramming::allocateMemory(size_t verticesNumber)
{
    _subsolutions = new Path ** [verticesNumber];
    for (size_t i = 0; i < verticesNumber; i++)
    {
        _subsolutions[i] = new Path * [(1 << verticesNumber) - 1]{};
    }
    _finalVerticesMask = pow(2, verticesNumber) - 1;
}

void PEA::DynamicProgramming::deallocateMemory(size_t verticesNumber)
{
    for (size_t i = 0; i < verticesNumber; i++)
    {
        for (size_t j = 0; j < (1 << verticesNumber) - 1; j++)
        {
            delete _subsolutions[i][j];
        }
        delete[] _subsolutions[i];
    }
    delete[] _subsolutions;

    _subsolutions = nullptr;
    _finalVerticesMask = 0;
}

PEA::Path* PEA::DynamicProgramming::examineNextLevel(AdjanencyMatrix& matrix, int currentVertex, int visitedVerticesMask)
{
    if (visitedVerticesMask == _finalVerticesMask)
    {
        Path* path = new Path();
        path->addVertex(currentVertex, matrix.getCost(currentVertex, 0));
        return path;
    }

    Path* solutionToExamine = _subsolutions[currentVertex][visitedVerticesMask];
    if (solutionToExamine != nullptr)
        return new Path(*solutionToExamine);

    solutionToExamine = new Path();
    solutionToExamine->addVertex(-1, INT_MAX);
    auto cost = solutionToExamine->getTotalCost();

    for (size_t i = 0; i < matrix.getVerticesNumber(); i++)
    {
        if (i == currentVertex || (visitedVerticesMask & (1 << i)))
            continue;

        Path* path = this->examineNextLevel(matrix, i, visitedVerticesMask | (1 << i));
        int cost = matrix.getCost(currentVertex, i);
        
        if (path->getTotalCost() + cost < solutionToExamine->getTotalCost())
        {
            delete solutionToExamine;
            solutionToExamine = path;
            solutionToExamine->addVertex(currentVertex, cost);
        }
        else
        {
            delete path;
        }
    }

    _subsolutions[currentVertex][visitedVerticesMask] = solutionToExamine;
    return new Path(*solutionToExamine);
}