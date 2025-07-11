📂 1. Структура проекта с модулями и CI

project/
├── CMakeLists.txt
├── include/
│   ├── IShape.h
│   ├── Rectangle.h
│   └── Circle.h
├── src/
│   ├── main.cpp
│   ├── Rectangle.cpp
│   └── Circle.cpp
├── tests/
│   ├── CMakeLists.txt
│   ├── RectangleTests.cpp
│   ├── CircleTests.cpp
│   └── main_test.cpp
└── .github/
    └── workflows/
        └── ci.yml


🤖 5. Настройка CI с GitHub Actions
📁 .github/workflows/ci.yml

yaml
Копировать
Редактировать
name: CMake CI

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  build-and-test:
    runs-on: ubuntu-latest

    steps:
    - name: 🔄 Checkout repository
      uses: actions/checkout@v3

    - name: 🛠 Install dependencies
      run: sudo apt-get update && sudo apt-get install -y cmake g++ make

    - name: 🏗 Configure CMake
      run: cmake -B build -DCMAKE_BUILD_TYPE=Release

    - name: ⚙️ Build
      run: cmake --build build --config Release

    - name: ✅ Run tests
      run: ctest --test-dir build --output-on-failure