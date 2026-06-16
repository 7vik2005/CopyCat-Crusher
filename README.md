# CopyCat Crusher

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg" />
  <img src="https://img.shields.io/badge/Build-CMake-brightgreen" />
  <img src="https://img.shields.io/badge/LLVM-libclang-orange" />
  <img src="https://img.shields.io/badge/License-MIT-yellow" />
  <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-success" />
</p>

<p align="center">
  <b>Advanced Source Code Plagiarism Detection Engine using Clang Tokenization, Rolling Hashing, Winnowing Fingerprinting, Suffix Automata and Levenshtein Similarity.</b>
</p>

---

## Overview

CopyCat Crusher is a high-performance source code plagiarism detection system designed to identify copied, modified, and structurally similar programs.

Unlike simple text comparison tools, CopyCat Crusher analyzes normalized source code structures and combines multiple plagiarism detection techniques to detect similarities even when variable names, literals, formatting, or comments have been changed.

The system is particularly useful for:

- Academic plagiarism detection
- Programming assignment evaluation
- Coding competition integrity checks
- Source code similarity analysis
- Software audit and review systems

---

## Problem Statement

Traditional plagiarism detectors often fail when students perform simple obfuscation techniques such as:

- Renaming variables
- Renaming functions
- Changing numeric constants
- Modifying formatting
- Rearranging comments
- Altering whitespace

Example:

### Submission A

```cpp
int sum = 0;

for(int i=0;i<100;i++){
    sum += i;
}
```

### Submission B

```cpp
int result = 0;

for(int index=0;index<100;index++){
    result += index;
}
```

Although these programs are nearly identical, a naïve text comparison system may treat them as different.

CopyCat Crusher normalizes source code before analysis and can identify such similarities effectively.

---

## Features

- Clang-based source code tokenization
- Language-aware token normalization
- Rolling Hash based sequence matching
- Winnowing Fingerprinting algorithm
- Suffix Automaton based longest common block detection
- Levenshtein Distance similarity scoring
- Weighted similarity fusion engine
- Automated batch analysis
- HTML report generation
- Multi-file comparison support
- C++17 implementation
- LLVM/libclang integration

---

## System Architecture

```text
Source Files
      │
      ▼
Clang Tokenizer
      │
      ▼
Token Normalizer
      │
      ▼
─────────────────────────────
│ Rolling Hash             │
│ Winnowing Fingerprinting │
│ Suffix Automaton         │
│ Levenshtein Similarity   │
─────────────────────────────
      │
      ▼
Similarity Fusion Engine
      │
      ▼
HTML Report Generator
      │
      ▼
Final Plagiarism Report
```

---

## Core Algorithms

### 1. Clang Tokenization

The project uses LLVM's libclang API to generate language-aware tokens from source code.

Benefits:

- Ignores formatting differences
- Understands C++ syntax
- Produces reliable token streams
- Handles complex language constructs

---

### 2. Token Normalization

Identifiers and literals are normalized before comparison.

Example:

Original:

```cpp
int marks = 95;
```

Normalized:

```text
TYPE VAR = NUM_LITERAL ;
```

This removes superficial differences and focuses on program structure.

---

### 3. Rolling Hash

Rolling Hash is used to efficiently generate hash values for token windows.

Benefits:

- Fast similarity detection
- O(N) processing
- Supports large submissions

---

### 4. Winnowing Fingerprinting

The Winnowing algorithm selects representative fingerprints from rolling hashes.

Benefits:

- Noise reduction
- Efficient storage
- Robust plagiarism detection

---

### 5. Suffix Automaton

Used to detect the longest common token block shared between two submissions.

Benefits:

- Detects copied code segments
- Linear-time processing
- Effective against partial plagiarism

---

### 6. Levenshtein Similarity

Measures edit distance between normalized token sequences.

Benefits:

- Detects near matches
- Handles minor modifications
- Captures structural similarity

---

## Similarity Score Calculation

The final plagiarism score is computed using a weighted ensemble of multiple algorithms.

```text
Final Score =
    Exact Match Score
  + Fingerprint Score
  + Longest Match Score
  + Levenshtein Score
```

The combined score provides a more reliable plagiarism estimate than any individual technique.

---

## Project Structure

```text
CopyCatCrusher/
│
├── include/
│   ├── algorithms/
│   ├── core/
│   ├── models/
│   ├── normalizer/
│   ├── report/
│   ├── similarity/
│   └── tokenizer/
│
├── src/
│   ├── algorithms/
│   ├── core/
│   ├── normalizer/
│   ├── report/
│   ├── similarity/
│   ├── tokenizer/
│   └── main.cpp
│
├── submissions/
├── reports/
│
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

## Installation

### Prerequisites

- C++17 Compatible Compiler
- CMake 3.20+
- LLVM
- libclang

### Clone Repository

```bash
git clone https://github.com/7vik2005/CopyCat-Crushers.git

cd CopyCat-Crushers
```

---

## Build Instructions

### Windows (MSYS2 UCRT64)

```bash
cmake -G "MinGW Makefiles" -S . -B build

cmake --build build
```

---

### Linux

```bash
mkdir build

cd build

cmake ..

make
```

---

## Usage

### Step 1

Place all source files inside:

```text
submissions/
```

Example:

```text
submissions/
├── student1.cpp
├── student2.cpp
├── student3.cpp
```

---

### Step 2

Run:

```bash
./build/CopyCatCrusher.exe submissions reports
```

---

### Step 3

View Results

Console Output:

```text
============================================================
                CopyCat Crusher Results
============================================================

Comparisons Generated: 3

Source                        Target                        Score (%)

student1.cpp                  student2.cpp                  100.00
student1.cpp                  student3.cpp                  81.62
student2.cpp                  student3.cpp                  81.62
```

---

## HTML Reports

For every comparison, an HTML report is automatically generated.

Example:

```text
reports/
├── comparison_0.html
├── comparison_1.html
├── comparison_2.html
```

Each report contains:

- Similarity Score
- Risk Level
- Exact Match Score
- Fingerprint Similarity
- Longest Common Block
- Levenshtein Similarity
- Token Statistics

---

## Risk Levels

| Score      | Risk Level |
| ---------- | ---------- |
| 0% - 30%   | Low        |
| 30% - 60%  | Medium     |
| 60% - 80%  | High       |
| 80% - 100% | Critical   |

---

## Performance

Typical Complexity:

| Component        | Complexity |
| ---------------- | ---------- |
| Tokenization     | O(N)       |
| Normalization    | O(N)       |
| Rolling Hash     | O(N)       |
| Winnowing        | O(N)       |
| Suffix Automaton | O(N)       |
| Levenshtein      | O(N × M)   |

Where:

- N = Tokens in Source File
- M = Tokens in Target File

---

## Future Improvements

Potential extensions:

- AST-based similarity analysis
- Cross-language plagiarism detection
- PDF report generation
- Similarity heatmaps
- Parallel batch processing
- Web dashboard
- Machine Learning based plagiarism classification

---

## License

This project is licensed under the MIT License.

See the LICENSE file for details.

---

## Author

**Satvik Jambagi**

---

## Acknowledgements

- LLVM Project
- Clang Team
- CMake
- GNU GCC
- Open Source Community

---

<p align="center">
Built with C++17, LLVM and a passion for detecting code plagiarism.
</p>
