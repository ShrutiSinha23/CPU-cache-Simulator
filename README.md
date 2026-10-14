# CPU Cache Simulator

A command-line cache simulator written in C++. It reads a sequence of memory addresses from a text file and reports whether each request is a **cache hit** or **cache miss**.

## Features

- Configurable total cache-entry count and associativity.
- Set-associative cache organization.
- Tag and set-index calculation for each memory address.
- Valid bits to distinguish empty entries from occupied entries.
- Least Recently Used (LRU) replacement policy within each set.
- Prints results to the terminal and writes them to `cache_sim_output`.

## Project Structure

```text
CacheSimulator/
├── main.cpp
├── Cache.h
├── Cache.cpp
├── input.txt
├── README.md
└── exe.exe
```

- **`main.cpp`** — parses command-line arguments, reads the input file, runs the simulation, and handles errors.
- **`Cache.h`** — declares the `Entry` structure and `Cache` class.
- **`Cache.cpp`** — implements cache initialization, lookup, insertion, tag/index calculation, replacement, and cleanup.
- **`input.txt`** — sample input containing one non-negative memory address per line.
- **`cache_sim_output`** — generated output file.
- **`exe.exe`** — compiled executable generated on Windows.

## Requirements

- A C++ compiler such as GCC (`g++`).
- A terminal such as VS Code Terminal, Command Prompt, PowerShell, or a Linux shell.

## Build

Open a terminal in the project directory and compile:

```bash
g++ main.cpp Cache.cpp -o exe
```

On Windows, GCC normally creates `exe.exe`. On Linux or macOS, the executable is usually named `exe`.

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

The simulator processes addresses in order. Negative addresses are rejected.

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

For example, `4 2 input.txt` configures a cache with four total entries and two-way associativity. This creates two sets with two ways per set.

The total number of entries should be divisible by the associativity. The current implementation does not explicitly validate this condition.

## Example Output

For the sample input above and the command `4 2 input.txt`, the expected terminal output is:

```text
1 : MISS
2 : MISS
3 : MISS
1 : HIT
2 : HIT
5 : MISS
1 : HIT
```

The final request for address `1` is a hit because address `1` remains in the cache after address `5` is inserted.

The same results are written to `cache_sim_output` in the current working directory.

## How It Works

1. The program reads the total number of entries, associativity, and input filename from command-line arguments.
2. It calculates the number of sets as `total entries / associativity`.
3. The cache calculates a set index and tag for each memory address using modulo and integer division.
4. It searches the selected set for a valid entry with a matching tag.
5. If a matching entry is found, the request is a **HIT**, and its access timestamp is updated.
6. Otherwise, the request is a **MISS**. The address is inserted into an invalid way if available; if the set is full, the least recently used entry is replaced.
7. The result is printed to the terminal and written to `cache_sim_output`.

## Replacement Policy

The simulator implements **Least Recently Used (LRU)** replacement within each set. Each cache entry stores a `lastChanged` timestamp that records its most recent access. The timestamp is updated on both cache hits and insertions.

When a set is full and a cache miss occurs, the simulator replaces the valid entry with the smallest timestamp, representing the least recently used entry.

The replacement policy operates independently within each set.

## Design Notes and Limitations

- The set index is calculated using `address % number_of_sets`, and the tag is calculated using integer division: `address / number_of_sets`.
- The number of total entries should be divisible by associativity.
- Input addresses must be non-negative integers.
- The code declares `Cache::changeCt`, but the current implementation does not use it. A separate file-local counter named `accessCounter` tracks accesses.
- The cache uses dynamically allocated arrays and manual memory management.
- The simulator models cache lookups and replacement, not actual hardware timing, cache-line byte offsets, or memory read/write operations.

## Possible Improvements

- Validate cache configuration and handle invalid command-line arguments more robustly.
- Improve input parsing to report malformed input explicitly.
- Add unit tests for hits, misses, collisions, and replacement behavior.
- Report aggregate statistics such as hit rate and miss rate.
- Use standard containers such as `std::vector` to simplify memory management.

## License

Add a license before distributing or publishing the project. If this code is based on another repository or assignment, preserve the applicable attribution and license terms.
