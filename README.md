# LibraFlow 📚  
*A Qt-powered Library Management System built in C++.*

## Overview

LibraFlow is a desktop application designed to help you manage and organize a book library with ease. Created using C++ and the Qt framework, this project demonstrates clean architectural separation between the Repository, Controller, and User Interface layers. It applies modern design principles and patterns like Command and Strategy to ensure both flexibility and maintainability.

---

## ✨ Features

### 🗃️ Repository Layer
- Abstract repository interface
- CSV and JSON-based backends
- Uses Qt’s JSON library for parsing and serialization

### 🧠 Controller Layer
- Add / Remove / Update book records
- Full **Undo/Redo** functionality (via **Command Pattern**)
- Advanced **Filtering** logic (via **Strategy/Specification Pattern**)
  - Filter by title, author, genre, year, etc.
  - Combine filters using **AND** or **OR**

### 🖥️ GUI
- Built with **Qt Widgets** and dynamic **Layouts**
- Clean, responsive UI for:
  - Viewing the list of books
  - Adding, editing, and deleting entries
  - Filtering records
  - Undo/Redo interactions
- Input validation and user feedback included

### ✅ Testing
- Core functionalities are tested using a **custom-built testing framework**
- Tests cover:
  - Repository I/O behavior
  - Controller operations
  - Filtering logic
  - Undo/Redo command stack behavior
- Assertions and utilities implemented manually for full control and flexibility

### 💾 Memory Safety
- No memory leaks — uses **RAII** and **smart pointers**
- Verified with tools like **Valgrind**

---

## 🧑‍💻 Tech Stack

- **C++17**
- **Qt 5/6 (Widgets, JSON)**
- **Custom C++ Testing Classes**
- **CMake** (optional)

---

## 🚀 Getting Started

### Prerequisites

- Qt Creator or Qt SDK
- C++ compiler with C++17 support
- Git, CMake (optional)

### Build Instructions

```bash
git clone https://github.com/your-username/libraflow.git
cd libraflow
# Open the project in Qt Creator and build
# OR
qmake && make
