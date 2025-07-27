#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "book.h"
#include <vector>
#include <memory>

class Repository
{
public:
    Repository();
    virtual ~Repository() = default;

    virtual void add(const Book& book) = 0;
    virtual void remove(int id) = 0;
    virtual std::vector<Book> getAll() const = 0;
    virtual std::unique_ptr<Book> findById(int id) const = 0;
};

#endif // REPOSITORY_H
