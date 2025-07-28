# LibraFlow 📚
*A sophisticated Qt-powered Library Management System showcasing modern C++ design patterns and architecture.*

## 🎯 Project Overview

LibraFlow is a feature-rich desktop library management application built with C++ and Qt. This project demonstrates professional software engineering practices through clean architectural separation, robust design patterns, and comprehensive testing. It's designed as both a functional library tool and a showcase of advanced C++ development techniques.

---

## 🏗️ Architecture & Design Patterns

### **Repository Pattern**
- **Abstract Repository Interface**: Clean separation between data access and business logic
- **Multiple Storage Backends**: 
  - `CSVRepository`: Human-readable CSV file storage
  - `JSONRepository`: Structured JSON storage with Qt's JSON framework
- **Pluggable Architecture**: Easy to extend with new storage types (database, cloud, etc.)

### **Command Pattern**
- **Full Undo/Redo System**: Complete operation history with stack-based management
- **Atomic Operations**: `AddCommand`, `RemoveCommand`, `UpdateCommand`
- **Memory-Safe Command Stack**: Smart pointer management for command objects

### **Strategy Pattern**
- **Flexible Filtering System**: Modular filter classes (`GenreFilter`, `AuthorFilter`, `YearFilter`)
- **Composable Logic**: AND/OR filter combinations
- **Runtime Filter Selection**: Dynamic filter application based on user preferences

### **MVC Architecture**
- **Model**: `Book` entity with validation and JSON serialization
- **View**: Qt-based GUI with responsive layouts and user feedback
- **Controller**: Centralized business logic with clean API

---

## ✨ Core Features

### 📚 **Book Management**
- **CRUD Operations**: Create, Read, Update, Delete books with validation
- **Smart ID Management**: Automatic ID assignment and conflict resolution
- **Data Validation**: Author name validation, genre restrictions, year bounds
- **JSON Serialization**: Complete object persistence with `toJson()`/`fromJson()`

### 🔄 **Advanced Undo/Redo**
- **Complete Operation History**: Every add, remove, and update operation
- **Memory Efficient**: Command objects store only necessary state
- **UI Integration**: Real-time button state management

### 🔍 **Powerful Filtering System**
- **Multi-Criteria Filtering**: Filter by title, author, genre, year ranges
- **Logical Operators**: Combine filters with AND/OR logic
- **Real-Time Updates**: Instant table refresh on filter changes
- **Filter State Management**: Enable/disable individual filter criteria

### 🖥️ **Professional GUI**
- **Responsive Design**: Splitter-based layout with resizable panels
- **Form Validation**: Real-time input validation with user feedback
- **Table Integration**: Selection-based editing with automatic form population
- **Repository Switching**: Runtime switching between CSV and JSON storage
- **Modern Qt Widgets**: Professional look with grouped controls

### 🧪 **Comprehensive Testing**
- **Custom Test Framework**: Purpose-built testing infrastructure
- **Full Coverage**: Tests for all major components
  - `BookTests`: Entity validation and serialization
  - `CSVRepositoryTests` & `JSONRepositoryTests`: Storage layer testing
  - `ControllerTests`: Business logic and command pattern testing
  - `FilterTests`: Strategy pattern and filtering logic
- **Exception Handling**: Robust error catching and reporting

---

## 🛠️ Technical Implementation

### **Memory Management**
- **RAII Principles**: Automatic resource management
- **Smart Pointers**: `std::unique_ptr` for safe memory handling
- **Zero Memory Leaks**: Valgrind-verified memory safety

### **Modern C++ Features**
- **C++17 Standards**: Modern language features and best practices
- **STL Integration**: `std::vector`, `std::stack`, `std::function`, `std::unordered_set`
- **Lambda Functions**: Flexible filtering with function objects
- **Move Semantics**: Efficient object handling

### **Qt Framework Integration**
- **Qt Widgets**: Professional desktop UI components
- **Qt JSON**: Native JSON parsing and generation
- **Qt Layouts**: Responsive and maintainable UI structure
- **Qt Signals/Slots**: Event-driven programming model

---

## 📁 Project Structure

```
LibraFlow/
├── Core/
│   ├── book.h/.cpp           # Book entity with validation and JSON serialization
│   ├── repository.h/.cpp     # Abstract repository interface
│   ├── csvrepository.h/.cpp  # CSV file storage implementation
│   └── jsonrepository.h/.cpp # JSON file storage implementation
├── Business/
│   ├── controller.h/.cpp     # Main business logic controller  
│   ├── commands.h/.cpp       # Command pattern for undo/redo operations
│   └── filter.h/.cpp         # Strategy pattern filtering system
├── UI/
│   ├── mainwindow.h/.cpp     # Main Qt application window
│   └── mainwindow.ui         # Qt Designer UI layout file
├── Testing/
│   ├── testframework.h/.cpp  # Custom testing infrastructure
│   ├── librarytests.h/.cpp   # Test suite definitions and implementations
│   └── tests.cpp             # Test execution implementations
└── main.cpp                  # Application entry point
```

---

## 🚀 Getting Started

### Prerequisites
```bash
Qt 5.12+ or Qt 6.x
C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
Qt Creator (recommended) or your preferred IDE
```

### Build Instructions

#### Using Qt Creator (Recommended)
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/libraflow.git
   cd libraflow
   ```
2. Open `LibraFlow.pro` in Qt Creator
3. Configure your kit (Desktop Qt version)
4. Build and run (Ctrl+R)

#### Command Line Build
```bash
# Generate Makefile
qmake LibraFlow.pro

# Compile
make

# Run
./LibraFlow
```

#### CMake Build (Alternative)
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---

## 🧪 Running Tests

The project includes a comprehensive test suite built with a custom testing framework:

```bash
# Tests are integrated into the application
# Run the executable and access tests through the application menu
# Or modify main.cpp to run tests directly at startup

# Example integration in main.cpp:
std::vector<std::unique_ptr<TestFramework>> testSuites;
testSuites.emplace_back(std::make_unique<BookTests>());
testSuites.emplace_back(std::make_unique<CSVRepositoryTests>());
testSuites.emplace_back(std::make_unique<JSONRepositoryTests>());
testSuites.emplace_back(std::make_unique<ControllerTests>());
testSuites.emplace_back(std::make_unique<FilterTests>());

for (auto& suite : testSuites) {
    suite->runAllTests();
    suite->printResults();
}
```

**Test Coverage:**
- ✅ Book entity validation and serialization
- ✅ CSV/JSON repository operations  
- ✅ Controller business logic
- ✅ Command pattern undo/redo functionality
- ✅ Filter strategy implementations
- ✅ Memory leak detection

---

## 🎮 Usage Guide

### **Basic Operations**
1. **Add Books**: Fill the form fields and click "Add Book"
2. **Edit Books**: Select a book from the table, modify fields, click "Update"
3. **Delete Books**: Select a book and click "Remove Book"
4. **Undo/Redo**: Use the undo/redo buttons to revert operations

### **Advanced Filtering**
1. **Enable Filters**: Check the filter criteria you want to use
2. **Set Values**: Enter filter values (title, author, genre, year range)
3. **Choose Logic**: Select AND (all criteria) or OR (any criteria)
4. **Apply**: Click "Apply Filter" to see results
5. **Clear**: Click "Clear Filters" to reset

### **Repository Management**
- **Switch Storage**: Choose between CSV and JSON storage formats
- **Data Persistence**: Your data is automatically saved to the selected format
- **File Location**: Data files are created in the application directory

---

## 🤝 Contributing

This project welcomes contributions! Here are some areas for enhancement:

- **Database Integration**: Add SQL database repository
- **Import/Export**: Bulk data import from external sources
- **Advanced Search**: Full-text search capabilities
- **Book Categories**: Hierarchical genre classification
- **User Management**: Multi-user support with permissions
- **Cloud Storage**: Online backup and synchronization

### Development Guidelines
- Follow existing code style and patterns
- Add tests for new functionality
- Update documentation for new features
- Ensure memory safety with smart pointers

---

## 📈 Performance & Quality

- **Memory Efficient**: Smart pointer usage eliminates memory leaks
- **Fast Operations**: O(1) ID-based lookups with efficient data structures
- **Scalable Architecture**: Repository pattern supports different storage backends
- **Robust Error Handling**: Comprehensive exception handling and user feedback
- **Professional UI**: Responsive design with proper input validation

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🏆 Acknowledgments

- **Qt Framework**: For providing excellent cross-platform GUI capabilities
- **Modern C++**: For enabling clean, safe, and efficient code
- **Design Patterns**: Gang of Four patterns for maintainable architecture

---

*LibraFlow demonstrates that desktop applications can be both powerful and elegantly architected. It serves as an excellent example of applying software engineering principles to create maintainable, extensible, and user-friendly applications.*
