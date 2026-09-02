**E-Commerce Behavior Analytics**

A C++20 application designed to manage user behavior tracking, compile-time profile evaluation using `if constexpr`, variadic data insertion via fold expressions, and dynamic promotion banner ranking.

**Features**

* **Variadic Data Stream Insertion**: Efficiently handles real-time user behavior logs using fold expressions.
* **Compile-Time Branching**: Evaluates user profile thresholds dynamically at compile time utilizing `if constexpr` and tag dispatching (`NewUser`, `VIPUser`, `RegularUser`).
* **CSV Database Integration**: Parses structured behavior logs directly from CSV files.
* **Promotion Ranking & Weighting**: Automatically sorts high-interest user sessions and applies loyalty weighting to top-performing entries.

**Prerequisites**

* A modern C++ compiler supporting **C++20** (MSVC, GCC, or Clang).
* CMake (optional, for project building).

**Project Structure**

```text
├── main.cpp
└── data/
    └── behavior_log.csv

```

**CSV Format Requirements**
Ensure your input CSV file contains a header row followed by comma-separated values matching this format:

```csv
NamaUser,Kategori,Durasi
Wawan,Pakaian,249.38

```

**Build & Execution**

1. Clone or copy the source files into your local development workspace.
2. Ensure the CSV path in `main.cpp` matches your local environment directory:
```cpp
load_behavior_from_csv("path/to/your/behavior_log.csv", analytics);

```


3. Compile using a C++20 compatible compiler:
```bash
g++ -std=c++20 main.cpp -o ecommerce_analytics
./ecommerce_analytics

```