#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QCheckBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QSplitter>
#include <QFrame>
#include <memory>
#include "controller.h"
// #include "csvrepository.h"
// #include "jsonrepository.h"
#include "book.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddBook();
    void onUpdateBook();
    void onRemoveBook();
    void onUndo();
    void onRedo();
    void onFilterBooks();
    void onClearFilters();
    void onTableSelectionChanged();
    void onRepositoryTypeChanged();

private:
    void setupUI();
    void setupMenuBar();
    void setupMainLayout();
    void setupBookFormGroup();
    void setupActionsGroup();
    void setupFilterGroup();
    void setupTableWidget();

    void setupRepositoryGroup();

    void refreshTable();
    void refreshTable(const std::vector<Book>& books);
    void clearForm();
    void populateFormFromSelection();
    void populateGenreComboBox();
    void updateButtonStates();

    bool validateForm();
    int getNextAvailableId();

    std::function<bool(const Book&)> createFilterFunction();

    // UI Components
    QWidget *centralWidget;
    QSplitter *mainSplitter;

    // Left panel components
    QWidget *leftPanel;
    QGroupBox *bookFormGroup;
    QGroupBox *actionsGroup;
    QGroupBox *filterGroup;
    QGroupBox *repositoryGroup;

    // Form inputs
    QLineEdit *titleEdit;
    QLineEdit *authorEdit;
    QComboBox *genreCombo;
    QSpinBox *yearSpinBox;
    QLabel *idLabel;

    // Action buttons
    QPushButton *addButton;
    QPushButton *updateButton;
    QPushButton *removeButton;
    QPushButton *undoButton;
    QPushButton *redoButton;

    // Filter components
    QLineEdit *filterTitleEdit;
    QLineEdit *filterAuthorEdit;
    QComboBox *filterGenreCombo;
    QSpinBox *filterYearFromSpinBox;
    QSpinBox *filterYearToSpinBox;
    QCheckBox *enableTitleFilter;
    QCheckBox *enableAuthorFilter;
    QCheckBox *enableGenreFilter;
    QCheckBox *enableYearFilter;
    QRadioButton *andFilterRadio;
    QRadioButton *orFilterRadio;
    QButtonGroup *filterLogicGroup;
    QPushButton *applyFilterButton;
    QPushButton *clearFilterButton;

    // Repository selection
    QRadioButton *csvRepoRadio;
    QRadioButton *jsonRepoRadio;
    QButtonGroup *repoGroup;

    // Table
    QTableWidget *booksTable;

    // Controller and data
    std::unique_ptr<Controller> controller;
    int selectedBookId;
    bool isUpdating;

    QVBoxLayout* leftLayout;
};

#endif // MAINWINDOW_H
