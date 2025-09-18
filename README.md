# Technical Write-Up

## Problems Discovered in the Original Code

### 1. Memory Management and Ownership
- Used a raw pointer (`returns_helper m_helper`) with manual `delete` in the destructor.  
- Risk of double deletion, dangling pointers, and unclear ownership semantics.  
- Destructor threw exceptions, which is a dangerous anti-pattern in C++ since destructors often run during stack unwinding.  

### 2. Broken `get_returns` Implementation
- Took a `std::string out` by value and modified it locally without returning anything.  
- The resulting string was discarded.  
- Produced an unwanted trailing comma.  

### 3. Unsafe `erase_values`
- Erased elements from a vector while iterating, invalidating the iterator.  
- Could cause skipped values, undefined behaviour, or crashes.  

### 4. Incorrect Maximum Calculation
- `get_max` initialised result to `0.0`.  
- For vectors with only negative numbers, returned `0.0` instead of the actual maximum (e.g., `-0.2`).  

### 5. Divide-by-Zero Risk in `get_mean`
- Divided by `m_returns.size()` without checking for zero.  
- Caused undefined behaviour on empty vectors.  

### 6. Inefficient `returns_optimal`
- Iterated through the entire vector even after detecting a value below the threshold.  
- No early exit causes wasted computation.  

### 7. Process Function Semantics
- `process_returns` was not marked `const`.  
- Allowed processing on objects expected to be immutable violates const-correctness.  

### 8. General Readability & Maintainability Issues
- Inconsistent naming (`returns_analyzer` vs `returns_analyser`).  
- Lack of explanatory comments.  
- Manual loops where STL algorithms would have been clearer and safer.  

---

## Changes Made

### 1. Introduced Smart Pointers
- Replaced raw `returns_helper` with `std::unique_ptr<returns_helper>`.  
- Removed manual `delete` and exception-throwing destructor.  

### 2. Fixed `get_returns`
- Now returns a `std::string` instead of modifying a discarded copy.  
- Used `std::ostringstream` with iterators to avoid trailing commas.  

### 3. Safe Value Erasure
- Replaced manual erase loop with the erase–remove idiom:  
  ```cpp
  m_returns.erase(std::remove(m_returns.begin(), m_returns.end(), value), m_returns.end());
  ```

### 4. Correct Maximum Calculation

- Replaced manual loop with std::max_element.
- Correctly handles negative-only vectors.
- Returns 0.0 if empty.

### 5. Safe Mean Calculation

- Used std::accumulate for clarity.
- Early return added for empty vectors (avoids divide-by-zero).

### 6. Optimised Optimality Check

- Added early exit - returns false immediately when threshold is violated.
- Marked noexcept for safety.

### 7. Const-Correctness

- Methods that do not modify state are marked const.
- process_returns is now const-safe.

###  8. Modern C++ Practices

- Replaced manual loops with range-based loops & STL algorithms.
- Added comments to explain design choices.
- Standardised naming convention (returns_analyser).

---

### Trade-Offs

- std::unique_ptr: enforces single ownership; if shared ownership is needed, std::shared_ptr would be required (but adds runtime cost).
- Returning 0.0 for empty collections: simple but ambiguous (could use std::optional<double> instead).
- Minimal logging: sufficient for now, but production code would benefit from structured logging with the capacity to control verbosity (e.g., spdlog).

---

### Checklist

- Fixes correctness and safety issues.
- Improves readability, maintainability, and idiomatic C++ usage.
- Establishes a foundation for future performance and extensibility improvements.

---

## Build & Test Guide

This project uses CMake as its build system, wrapped with a `Makefile` for convenience.  
The Makefile ensures that common commands function consistently across Windows, Linux, and macOS.  

### Quick Start

From the project root:

```bash
make      
