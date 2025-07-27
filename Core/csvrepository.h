#ifndef CSVREPOSITORY_H
#define CSVREPOSITORY_H

#include "repository.h"

class CSVRepository : public Repository
{
public:
    CSVRepository();
    CSVRepository(std::string fileName);
    ~CSVRepository() override = default;

    void add(const Book& book) override;
    void remove(int id) override;
    std::vector<Book> getAll() const override;
    std::unique_ptr<Book> findById(int id) const override;
private:
    std::string fileName;
    std::vector<Book> books;

    void loadFromFile();
    void saveToFile() const;
};

#endif // CSVREPOSITORY_H
