# CPU Cache Simulator

A command-line cache simulator written in C++. It reads a sequence of memory addresses from a text file and reports whether each request is a **cache hit** or **cache miss**.

## Features

- Configurable total cache-entry count and associativity.
- Set-associative cache organization.
- Tag and set-index calculation for each memory address.
- Valid bits to distinguish empty entries from occupied entries.
- Timestamp-based replacement when a set is full.
- Prints results to the terminal and writes them to `cache_sim_output`.

## Project Structure

```text
CacheSimulator/
├── main.cpp
├── Cache.h
├── Cache.cpp
├── input.txt
└── exe.exe
```

- **`main.cpp`** — parses command-line arguments, reads the input file, runs the simulation, and handles errors.
- **`Cache.h`** — declares the `Entry` structure and `Cache` class.
- **`Cache.cpp`** — implements cache initialization, lookup, insertion, tag/index calculation, and cleanup.
- **`input.txt`** — sample input containing one non-negative memory address per line.
- **`cache_sim_output`** — generated output file.

## Requirements

- A C++ compiler such as GCC (`g++`).
- A terminal such as VS Code Terminal, Command Prompt, PowerShell, or a Linux shell.

## Build

Open a terminal in the project directory and compile:

```bash
g++ main.cpp Cache.cpp -o exe
```

On Windows, GCC normally creates `exe.exe`. On Linux/macOS, the executable is usually named `exe`.

## Input Format

Create a text file named `input.txt` in the project directory. Put one non-negative integer memory address per line:

```text
1
2
3
1
2
5
1
```

The simulator reads addresses in order. Negative addresses are rejected.

## Run

### Windows PowerShell

```powershell
.\exe.exe 4 2 input.txt
```

### Linux or macOS

```bash
./exe 4 2 input.txt
```

The command-line arguments are:

```text
<total_number_of_entries> <associativity> <memory_reference_file>
```

For example, `4 2 input.txt` requests a cache with four total entries and two-way associativity. This produces two sets with two ways per set.

The total entry count should be divisible by the associativity. The current implementation does not explicitly validate this condition.

## Example Output

For the sample input above and the command `4 2 input.txt`, the expected terminal output is:

```text
1 : MISS
2 : MISS
3 : MISS
1 : HIT
2 : HIT
5 : MISS
1 : MISS
```

The final request for address `1` is a miss because address `1` was evicted when address `5` was inserted.

The same results are written to a file named `cache_sim_output` in the current working directory.

## How It Works

1. The program reads the total number of entries, associativity, and input filename from command-line arguments.
2. It calculates the number of sets as `total entries / associativity`.
3. The cache calculates a set index and tag for each address.
4. It searches the selected set for a valid entry with a matching tag.
5. If a matching entry is found, the request is a **HIT**.
6. Otherwise, the request is a **MISS**. The address is inserted into an invalid way if available; if the set is full, an existing entry is replaced.
7. The result is printed to the terminal and written to `cache_sim_output`.

## Replacement Policy

Each entry stores a `lastChanged` timestamp. When a miss inserts an address, the simulator increments a counter and stores the new timestamp. If a set is full, the entry with the smallest timestamp is selected for replacement.

**Limitation:** The timestamp is updated on insertion, not on a cache hit. Therefore, this is not a true Least Recently Used (LRU) implementation. It is closer to insertion-time-based replacement.

## Design Notes and Limitations

- The implementation treats each input integer as a cacheable address-sized item; it does not simulate block offsets, actual memory data, writes, or hardware timing.
- The tag calculation uses `log2(number_of_sets)` and a right shift, so the number of sets should be a power of two for the index/tag scheme to behave as intended.
- The number of total entries should be divisible by associativity.
- Input addresses must be non-negative integers.
- The code declares `Cache::changeCt`, but the current implementation does not use it; a separate file-local counter named `modCount` tracks insertions.
- The current timestamp policy does not update recency on hits, so it should not be described as LRU.

## Possible Improvements

- Implement true LRU by updating recency on every hit.
- Validate that the number of entries is divisible by associativity and that the set count is a power of two.
- Improve command-line parsing and report malformed input explicitly.
- Add unit tests for hits, misses, collisions, and replacement behavior.
- Report aggregate statistics such as hit rate and miss rate.

