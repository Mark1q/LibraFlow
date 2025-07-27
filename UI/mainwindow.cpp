#include "mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <algorithm>

#include "csvrepository.h"
#include "jsonrepository.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , selectedBookId(-1)
    , isUpdating(false)
{

    QVBoxLayout* leftLayout = nullptr;

    // Initialize with CSV repository by default
    controller = std::make_unique<Controller>(
        std::make_unique<CSVRepository>("library.csv")
        );

    setupUI();
    refreshTable();
    updateButtonStates();

    setWindowTitle("Library Management System");
    setMinimumSize(1000, 700);
    resize(1200, 800);

    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setupMenuBar();
    setupMainLayout();

    setupRepositoryGroup();     // ✅ 1. Repository selection at top
    setupBookFormGroup();       // ✅ 2. Book information
    setupActionsGroup();        // ✅ 3. Action buttons
    setupFilterGroup();         // ✅ 4. Filters
    setupTableWidget();         // ✅ 5. Books table (right panel)

    leftLayout->addStretch();   // ✅ Pushes everything to the top


    csvRepoRadio->setChecked(true); // Default to CSV

    repoGroup = new QButtonGroup(this);
    repoGroup->addButton(csvRepoRadio);
    repoGroup->addButton(jsonRepoRadio);

    connect(csvRepoRadio, &QRadioButton::toggled, this, &MainWindow::onRepositoryTypeChanged);
    connect(jsonRepoRadio, &QRadioButton::toggled, this, &MainWindow::onRepositoryTypeChanged);
}

void MainWindow::setupRepositoryGroup()
{
    repositoryGroup = new QGroupBox("Repository Type", leftPanel);
    QHBoxLayout *repoLayout = new QHBoxLayout(repositoryGroup);

    csvRepoRadio = new QRadioButton("CSV Repository");
    jsonRepoRadio = new QRadioButton("JSON Repository");
    csvRepoRadio->setChecked(true);

    repoGroup = new QButtonGroup(this);
    repoGroup->addButton(csvRepoRadio);
    repoGroup->addButton(jsonRepoRadio);

    repoLayout->addWidget(csvRepoRadio);
    repoLayout->addWidget(jsonRepoRadio);

    connect(csvRepoRadio, &QRadioButton::toggled, this, &MainWindow::onRepositoryTypeChanged);
    connect(jsonRepoRadio, &QRadioButton::toggled, this, &MainWindow::onRepositoryTypeChanged);

    leftLayout->addWidget(repositoryGroup); // <--- Add it to layout HERE
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();

    QMenu *fileMenu = menuBar->addMenu("&File");
    QAction *exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu *editMenu = menuBar->addMenu("&Edit");
    QAction *undoAction = editMenu->addAction("&Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndo);

    QAction *redoAction = editMenu->addAction("&Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onRedo);
}

void MainWindow::setupMainLayout()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);

    leftPanel = new QWidget();
    leftPanel->setMaximumWidth(350);
    leftPanel->setMinimumWidth(300);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(mainSplitter);

    leftLayout = new QVBoxLayout(leftPanel); // Store this for future use
    leftLayout->setSpacing(10);              // Optional visual spacing

    mainSplitter->addWidget(leftPanel);
}

void MainWindow::setupBookFormGroup()
{
    bookFormGroup = new QGroupBox("Book Information", leftPanel);
    QGridLayout *formLayout = new QGridLayout(bookFormGroup);

    // ID (read-only, for updates)
    idLabel = new QLabel("New Book");
    idLabel->setStyleSheet("font-weight: bold; color: #0066cc;");
    formLayout->addWidget(new QLabel("Book ID:"), 0, 0);
    formLayout->addWidget(idLabel, 0, 1);

    // Title
    formLayout->addWidget(new QLabel("Title:"), 1, 0);
    titleEdit = new QLineEdit();
    titleEdit->setPlaceholderText("Enter book title");
    formLayout->addWidget(titleEdit, 1, 1);

    // Author
    formLayout->addWidget(new QLabel("Author:"), 2, 0);
    authorEdit = new QLineEdit();
    authorEdit->setPlaceholderText("Enter author name");
    formLayout->addWidget(authorEdit, 2, 1);

    // Genre
    formLayout->addWidget(new QLabel("Genre:"), 3, 0);
    genreCombo = new QComboBox();
    populateGenreComboBox();
    formLayout->addWidget(genreCombo, 3, 1);

    // Year
    formLayout->addWidget(new QLabel("Year:"), 4, 0);
    yearSpinBox = new QSpinBox();
    yearSpinBox->setRange(1, 2025);
    yearSpinBox->setValue(2024);
    formLayout->addWidget(yearSpinBox, 4, 1);

    leftLayout->addWidget(bookFormGroup);
}

void MainWindow::setupActionsGroup()
{
    actionsGroup = new QGroupBox("Actions", leftPanel);
    QVBoxLayout *actionsLayout = new QVBoxLayout(actionsGroup);

    addButton = new QPushButton("Add Book");
    addButton->setStyleSheet("QPushButton { background-color: #28a745; color: white; font-weight: bold; padding: 8px; }");

    updateButton = new QPushButton("Update Book");
    updateButton->setStyleSheet("QPushButton { background-color: #ffc107; color: black; font-weight: bold; padding: 8px; }");
    updateButton->setEnabled(false);

    removeButton = new QPushButton("Remove Book");
    removeButton->setStyleSheet("QPushButton { background-color: #dc3545; color: white; font-weight: bold; padding: 8px; }");
    removeButton->setEnabled(false);

    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);

    undoButton = new QPushButton("Undo");
    undoButton->setStyleSheet("QPushButton { background-color: #6c757d; color: white; padding: 6px; }");

    redoButton = new QPushButton("Redo");
    redoButton->setStyleSheet("QPushButton { background-color: #6c757d; color: white; padding: 6px; }");

    actionsLayout->addWidget(addButton);
    actionsLayout->addWidget(updateButton);
    actionsLayout->addWidget(removeButton);
    actionsLayout->addWidget(separator);

    QHBoxLayout *undoRedoLayout = new QHBoxLayout();
    undoRedoLayout->addWidget(undoButton);
    undoRedoLayout->addWidget(redoButton);
    actionsLayout->addLayout(undoRedoLayout);

    // Connect signals
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddBook);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateBook);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveBook);
    connect(undoButton, &QPushButton::clicked, this, &MainWindow::onUndo);
    connect(redoButton, &QPushButton::clicked, this, &MainWindow::onRedo);

    leftLayout->addWidget(actionsGroup);
}

void MainWindow::setupFilterGroup()
{
    filterGroup = new QGroupBox("Filters", leftPanel);
    QVBoxLayout *filterMainLayout = new QVBoxLayout(filterGroup);

    QGridLayout *filterLayout = new QGridLayout();

    // Title filter
    enableTitleFilter = new QCheckBox("Title contains:");
    filterTitleEdit = new QLineEdit();
    filterTitleEdit->setEnabled(false);
    filterLayout->addWidget(enableTitleFilter, 0, 0);
    filterLayout->addWidget(filterTitleEdit, 0, 1);

    // Author filter
    enableAuthorFilter = new QCheckBox("Author contains:");
    filterAuthorEdit = new QLineEdit();
    filterAuthorEdit->setEnabled(false);
    filterLayout->addWidget(enableAuthorFilter, 1, 0);
    filterLayout->addWidget(filterAuthorEdit, 1, 1);

    // Genre filter
    enableGenreFilter = new QCheckBox("Genre:");
    filterGenreCombo = new QComboBox();
    filterGenreCombo->addItem("Any");
    filterGenreCombo->addItems({"SF", "Romance", "Drama", "Fantasy", "History"});
    filterGenreCombo->setEnabled(false);
    filterLayout->addWidget(enableGenreFilter, 2, 0);
    filterLayout->addWidget(filterGenreCombo, 2, 1);

    // Year range filter
    enableYearFilter = new QCheckBox("Year range:");
    QHBoxLayout *yearRangeLayout = new QHBoxLayout();
    filterYearFromSpinBox = new QSpinBox();
    filterYearFromSpinBox->setRange(1, 2025);
    filterYearFromSpinBox->setValue(1900);
    filterYearFromSpinBox->setEnabled(false);

    filterYearToSpinBox = new QSpinBox();
    filterYearToSpinBox->setRange(1, 2025);
    filterYearToSpinBox->setValue(2025);
    filterYearToSpinBox->setEnabled(false);

    yearRangeLayout->addWidget(filterYearFromSpinBox);
    yearRangeLayout->addWidget(new QLabel("to"));
    yearRangeLayout->addWidget(filterYearToSpinBox);

    filterLayout->addWidget(enableYearFilter, 3, 0);
    filterLayout->addLayout(yearRangeLayout, 3, 1);

    filterMainLayout->addLayout(filterLayout);

    // Filter logic (AND/OR)
    QHBoxLayout *logicLayout = new QHBoxLayout();
    logicLayout->addWidget(new QLabel("Logic:"));
    andFilterRadio = new QRadioButton("AND");
    orFilterRadio = new QRadioButton("OR");
    andFilterRadio->setChecked(true);

    filterLogicGroup = new QButtonGroup(this);
    filterLogicGroup->addButton(andFilterRadio);
    filterLogicGroup->addButton(orFilterRadio);

    logicLayout->addWidget(andFilterRadio);
    logicLayout->addWidget(orFilterRadio);
    logicLayout->addStretch();
    filterMainLayout->addLayout(logicLayout);

    // Filter buttons
    QHBoxLayout *filterButtonLayout = new QHBoxLayout();
    applyFilterButton = new QPushButton("Apply Filters");
    applyFilterButton->setStyleSheet("QPushButton { background-color: #007bff; color: white; padding: 6px; }");
    clearFilterButton = new QPushButton("Clear Filters");
    clearFilterButton->setStyleSheet("QPushButton { background-color: #6c757d; color: white; padding: 6px; }");

    filterButtonLayout->addWidget(applyFilterButton);
    filterButtonLayout->addWidget(clearFilterButton);
    filterMainLayout->addLayout(filterButtonLayout);

    // Connect filter enable/disable signals
    connect(enableTitleFilter, &QCheckBox::toggled, filterTitleEdit, &QLineEdit::setEnabled);
    connect(enableAuthorFilter, &QCheckBox::toggled, filterAuthorEdit, &QLineEdit::setEnabled);
    connect(enableGenreFilter, &QCheckBox::toggled, filterGenreCombo, &QComboBox::setEnabled);
    connect(enableYearFilter, &QCheckBox::toggled, filterYearFromSpinBox, &QSpinBox::setEnabled);
    connect(enableYearFilter, &QCheckBox::toggled, filterYearToSpinBox, &QSpinBox::setEnabled);

    connect(applyFilterButton, &QPushButton::clicked, this, &MainWindow::onFilterBooks);
    connect(clearFilterButton, &QPushButton::clicked, this, &MainWindow::onClearFilters);

    leftLayout->addWidget(filterGroup);
    leftLayout->addWidget(repositoryGroup);
    leftLayout->addStretch();
}

void MainWindow::setupTableWidget()
{
    booksTable = new QTableWidget();
    booksTable->setColumnCount(5);
    booksTable->setHorizontalHeaderLabels({"ID", "Title", "Author", "Genre", "Year"});

    // Configure table appearance
    booksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    booksTable->setSelectionMode(QAbstractItemView::SingleSelection);
    booksTable->setAlternatingRowColors(true);
    booksTable->setSortingEnabled(true);

    // Configure column widths
    booksTable->horizontalHeader()->setStretchLastSection(true);
    booksTable->setColumnWidth(0, 60);  // ID
    booksTable->setColumnWidth(1, 200); // Title
    booksTable->setColumnWidth(2, 150); // Author
    booksTable->setColumnWidth(3, 100); // Genre

    connect(booksTable, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onTableSelectionChanged);

    mainSplitter->addWidget(booksTable);
    mainSplitter->setSizes({300, 700});
}

void MainWindow::populateGenreComboBox()
{
    genreCombo->addItems({"SF", "Romance", "Drama", "Fantasy", "History"});
}

void MainWindow::refreshTable()
{
    auto books = controller->getAllBooks();
    refreshTable(books);
}

void MainWindow::refreshTable(const std::vector<Book>& books)
{
    booksTable->setRowCount(books.size());

    for (size_t i = 0; i < books.size(); ++i) {
        const Book& book = books[i];

        booksTable->setItem(i, 0, new QTableWidgetItem(QString::number(book.getId())));
        booksTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(book.getTitle())));
        booksTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(book.getAuthor())));
        booksTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(book.getGenre())));
        booksTable->setItem(i, 4, new QTableWidgetItem(QString::number(book.getYear())));

        // Make ID column read-only and centered
        booksTable->item(i, 0)->setFlags(booksTable->item(i, 0)->flags() & ~Qt::ItemIsEditable);
        booksTable->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        booksTable->item(i, 4)->setTextAlignment(Qt::AlignCenter);
    }

    statusBar()->showMessage(QString("Total books: %1").arg(books.size()));
}

void MainWindow::clearForm()
{
    titleEdit->clear();
    authorEdit->clear();
    genreCombo->setCurrentIndex(0);
    yearSpinBox->setValue(2024);
    idLabel->setText("New Book");
    idLabel->setStyleSheet("font-weight: bold; color: #0066cc;");
    selectedBookId = -1;
    isUpdating = false;
}

void MainWindow::populateFormFromSelection()
{
    if (selectedBookId == -1) return;

    auto book = controller->findBook(selectedBookId);
    if (!book) return;

    titleEdit->setText(QString::fromStdString(book->getTitle()));
    authorEdit->setText(QString::fromStdString(book->getAuthor()));

    int genreIndex = genreCombo->findText(QString::fromStdString(book->getGenre()));
    if (genreIndex != -1) {
        genreCombo->setCurrentIndex(genreIndex);
    }

    yearSpinBox->setValue(book->getYear());
    idLabel->setText(QString("ID: %1").arg(book->getId()));
    idLabel->setStyleSheet("font-weight: bold; color: #dc3545;");
    isUpdating = true;
}

bool MainWindow::validateForm()
{
    if (titleEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Title cannot be empty.");
        titleEdit->setFocus();
        return false;
    }

    if (authorEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Author cannot be empty.");
        authorEdit->setFocus();
        return false;
    }

    return true;
}

int MainWindow::getNextAvailableId()
{
    auto books = controller->getAllBooks();
    int maxId = 0;
    for (const auto& book : books) {
        if (book.getId() > maxId) {
            maxId = book.getId();
        }
    }
    return maxId + 1;
}

void MainWindow::updateButtonStates()
{
    bool hasSelection = (selectedBookId != -1);
    updateButton->setEnabled(hasSelection);
    removeButton->setEnabled(hasSelection);
}

std::function<bool(const Book&)> MainWindow::createFilterFunction()
{
    return [this](const Book& book) -> bool {
        std::vector<bool> conditions;

        // Title filter
        if (enableTitleFilter->isChecked()) {
            QString filterText = filterTitleEdit->text().trimmed().toLower();
            QString bookTitle = QString::fromStdString(book.getTitle()).toLower();
            conditions.push_back(bookTitle.contains(filterText));
        }

        // Author filter
        if (enableAuthorFilter->isChecked()) {
            QString filterText = filterAuthorEdit->text().trimmed().toLower();
            QString bookAuthor = QString::fromStdString(book.getAuthor()).toLower();
            conditions.push_back(bookAuthor.contains(filterText));
        }

        // Genre filter
        if (enableGenreFilter->isChecked()) {
            QString selectedGenre = filterGenreCombo->currentText();
            if (selectedGenre != "Any") {
                conditions.push_back(QString::fromStdString(book.getGenre()) == selectedGenre);
            }
        }

        // Year range filter
        if (enableYearFilter->isChecked()) {
            int yearFrom = filterYearFromSpinBox->value();
            int yearTo = filterYearToSpinBox->value();
            conditions.push_back(book.getYear() >= yearFrom && book.getYear() <= yearTo);
        }

        if (conditions.empty()) return true;

        // Apply AND/OR logic
        if (andFilterRadio->isChecked()) {
            return std::all_of(conditions.begin(), conditions.end(), [](bool c) { return c; });
        } else {
            return std::any_of(conditions.begin(), conditions.end(), [](bool c) { return c; });
        }
    };
}

// Slot implementations
void MainWindow::onAddBook()
{
    if (!validateForm()) return;

    try {
        Book book(
            titleEdit->text().toStdString(),
            authorEdit->text().toStdString(),
            genreCombo->currentText().toStdString(),
            yearSpinBox->value(),
            getNextAvailableId()
            );

        controller->addBook(book);
        refreshTable();
        clearForm();

        statusBar()->showMessage("Book added successfully", 2000);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to add book: %1").arg(e.what()));
    }
}

void MainWindow::onUpdateBook()
{
    if (!isUpdating || selectedBookId == -1) return;
    if (!validateForm()) return;

    try {
        Book book(
            titleEdit->text().toStdString(),
            authorEdit->text().toStdString(),
            genreCombo->currentText().toStdString(),
            yearSpinBox->value(),
            selectedBookId
            );

        controller->updateBook(book);
        refreshTable();
        clearForm();
        updateButtonStates();

        statusBar()->showMessage("Book updated successfully", 2000);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to update book: %1").arg(e.what()));
    }
}

void MainWindow::onRemoveBook()
{
    if (selectedBookId == -1) return;

    auto book = controller->findBook(selectedBookId);
    if (!book) return;

    int ret = QMessageBox::question(this, "Confirm Removal",
                                    QString("Are you sure you want to remove the book:\n\n'%1' by %2?")
                                        .arg(QString::fromStdString(book->getTitle()))
                                        .arg(QString::fromStdString(book->getAuthor())),
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        try {
            controller->removeBook(selectedBookId);
            refreshTable();
            clearForm();
            updateButtonStates();

            statusBar()->showMessage("Book removed successfully", 2000);
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to remove book: %1").arg(e.what()));
        }
    }
}

void MainWindow::onUndo()
{
    controller->undo();
    refreshTable();
    clearForm();
    updateButtonStates();
    statusBar()->showMessage("Undo completed", 2000);
}

void MainWindow::onRedo()
{
    controller->redo();
    refreshTable();
    clearForm();
    updateButtonStates();
    statusBar()->showMessage("Redo completed", 2000);
}

void MainWindow::onFilterBooks()
{
    auto filterFn = createFilterFunction();
    auto filteredBooks = controller->filterBooks(filterFn);
    refreshTable(filteredBooks);

    statusBar()->showMessage(QString("Showing %1 filtered books").arg(filteredBooks.size()), 3000);
}

void MainWindow::onClearFilters()
{
    enableTitleFilter->setChecked(false);
    enableAuthorFilter->setChecked(false);
    enableGenreFilter->setChecked(false);
    enableYearFilter->setChecked(false);

    filterTitleEdit->clear();
    filterAuthorEdit->clear();
    filterGenreCombo->setCurrentIndex(0);
    filterYearFromSpinBox->setValue(1900);
    filterYearToSpinBox->setValue(2025);

    andFilterRadio->setChecked(true);

    refreshTable();
    statusBar()->showMessage("Filters cleared", 2000);
}

void MainWindow::onTableSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = booksTable->selectedItems();
    if (selectedItems.isEmpty()) {
        selectedBookId = -1;
        clearForm();
        updateButtonStates();
        return;
    }

    int row = selectedItems.first()->row();
    QTableWidgetItem* idItem = booksTable->item(row, 0);
    if (idItem) {
        selectedBookId = idItem->text().toInt();
        populateFormFromSelection();
        updateButtonStates();
    }
}

void MainWindow::onRepositoryTypeChanged()
{
    if (csvRepoRadio->isChecked()) {
        controller = std::make_unique<Controller>(
            std::make_unique<CSVRepository>("library.csv")
            );
        statusBar()->showMessage("Switched to CSV repository", 2000);
    } else if (jsonRepoRadio->isChecked()) {
        controller = std::make_unique<Controller>(
            std::make_unique<JSONRepository>("library.json")
            );
        statusBar()->showMessage("Switched to JSON repository", 2000);
    }

    refreshTable();
    clearForm();
    updateButtonStates();
}
