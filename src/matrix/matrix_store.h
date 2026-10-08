#pragma once

#include "matrix/matrix.h"

#include <map>
#include <string>
#include <vector>

class MatrixStore
{
public:
    void put(const std::string &name, const Matrix &matrix);
    const Matrix *find(const std::string &name) const;
    bool contains(const std::string &name) const;
    bool remove(const std::string &name);
    std::vector<std::string> names() const;

private:
    std::map<std::string, Matrix> matrices;
};
