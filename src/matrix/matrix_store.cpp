#include "matrix/matrix_store.h"

void MatrixStore::put(const std::string &name, const Matrix &matrix)
{
    matrices.insert_or_assign(name, matrix);
}

const Matrix *MatrixStore::find(const std::string &name) const
{
    auto it = matrices.find(name);
    if (it == matrices.end())
        return nullptr;
    return &it->second;
}

bool MatrixStore::contains(const std::string &name) const
{
    return matrices.count(name) > 0;
}

bool MatrixStore::remove(const std::string &name)
{
    return matrices.erase(name) > 0;
}

std::vector<std::string> MatrixStore::names() const
{
    std::vector<std::string> result;
    for (const auto &entry : matrices)
        result.push_back(entry.first);
    return result;
}
