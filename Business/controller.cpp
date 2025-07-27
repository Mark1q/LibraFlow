#include "controller.h"

Controller::Controller() {}

Controller::Controller(std::unique_ptr<Repository> repo)
    : repo(std::move(repo)) {}

void Controller::addBook(const Book& book) {
    repo->add(book);
    undoStack.push(std::make_unique<AddCommand>(repo.get(), book));
    while (!redoStack.empty()) redoStack.pop();
}

void Controller::removeBook(int id) {
    auto book = repo->findById(id);
    if (!book) return;
    repo->remove(id);
    undoStack.push(std::make_unique<RemoveCommand>(repo.get(), *book));
    while (!redoStack.empty()) redoStack.pop();
}

void Controller::updateBook(const Book& book) {
    auto old = repo->findById(book.getId());
    if (!old) return;
    repo->remove(book.getId());
    repo->add(book);
    undoStack.push(std::make_unique<UpdateCommand>(repo.get(), *old, book));
    while (!redoStack.empty()) redoStack.pop();
}

std::vector<Book> Controller::getAllBooks() const {
    return repo->getAll();
}

std::unique_ptr<Book> Controller::findBook(int id) const {
    return repo->findById(id);
}

void Controller::undo() {
    if (undoStack.empty()) return;
    auto cmd = std::move(undoStack.top());
    undoStack.pop();
    cmd->undo();
    redoStack.push(std::move(cmd));
}

void Controller::redo() {
    if (redoStack.empty()) return;
    auto cmd = std::move(redoStack.top());
    redoStack.pop();
    cmd->redo();
    undoStack.push(std::move(cmd));
}

std::vector<Book> Controller::filterBooks(const std::function<bool(const Book&)>& filterFn) const {
    std::vector<Book> result;
    for (const auto& book : repo->getAll()) {
        if (filterFn(book)) result.push_back(book);
    }
    return result;
}
