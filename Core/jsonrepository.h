#ifndef JSONREPOSITORY_H
#define JSONREPOSITORY_H

#include "repository.h"

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

class JSONRepository : public Repository
{
public:
    JSONRepository();
    explicit JSONRepository(const QString& fileName);
    ~JSONRepository() override = default;

    void add(const Book& book) override;
    void remove(int id) override;
    std::vector<Book> getAll() const override;
    std::unique_ptr<Book> findById(int id) const override;
private:
    QString fileName;
    std::vector<Book> books;

    void loadFromFile();
    void saveToFile() const;
};

#endif // JSONREPOSITORY_H
