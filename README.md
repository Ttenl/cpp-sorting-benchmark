# C++ sorting benchmark

This C++ program compares selection sort and Shell sort with Knuth's gap sequence. Both algorithms sort copies of the same integer arrays. The program checks every result against `std::sort`.

The two implementations are `selectionSort` and `shellSort` in [main.cpp](main.cpp). The `timeSort` function measures each call with `std::chrono::steady_clock`.

## Build and run

Use a C++17 compiler and CMake 3.16 or later. Run these commands from the repository root.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/cpp_sorting_benchmark
```

In CLion, open the repository folder as a CMake project and run `cpp_sorting_benchmark` with a Release profile.

## How the benchmark works

- Tests five array sizes from 10,000 to 50,000 elements. For each size, it generates five arrays and sorts each array five times with each algorithm.
- Uses the fixed seed `2026` and alternates which algorithm runs first. Input generation, copying, and verification happen outside the timed sections.
- Prints average times in milliseconds for each size and writes 125 individual comparisons to `rezultatai.csv` in the working directory. Selection sort can make the larger cases take a while.

[Example CSV output](examples/benchmark-sample.csv) includes the input size (`n`), array number (`aibe`), repeat number (`testas`), and times for selection sort (`isrinkimas_ms`) and Shell sort (`selas_ms`). The console messages and CSV labels are in Lithuanian. Timings depend on the compiler and computer.

## Sample results

Average times from a Release build on macOS using Apple Clang 17.

| Array size | Selection sort (ms) | Shell sort (ms) |
| ---: | ---: | ---: |
| 10,000 | 77.233 | 0.407 |
| 20,000 | 285.225 | 0.908 |
| 30,000 | 610.805 | 1.402 |
| 40,000 | 1034.689 | 1.968 |
| 50,000 | 1530.366 | 2.555 |
