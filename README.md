# BGM_assembler — Binary Genome Map Assembly

C++17 library, command-line assembler (`BGM-asm-program`), and Python extension module (`_bgm_module`) for *de novo* assembly of Optical Mapping (OM) genomic reads using binary representation.

---

## Algorithm Overview

Optical Mapping reads describe long DNA fragments (up to 1 Mbp). Instead of costly dynamic programming alignments, `BGM_assembler` represents restriction maps as compact bit vectors and performs assembly in four stages:

1. **Quantization & Binary Conversion** — converting restriction maps (`.bnx` / `.cmap`) into binary strings (`fasta01` / `.binmaps`).
2. **Overlap Detection** — fast bitwise XOR / bit-shift alignment across parallel worker threads.
3. **Layout Determination** — constructing the overlap graph and resolving map layout.
4. **Consensus Generation** — iterative folding to produce consensus genomic maps.

---

## Repository Structure

- `./` — entry points (`main.cpp` for CLI, `python_interface.cpp` for `Boost.Python` bindings) and `CMakeLists.txt`
- `src/` — C++17 core library (`data/`, `algorithms/`, `common/`)
- `python/` — Python utilities (`bgm_util`) and `bnx_convert.py` converter for `.bnx` optical map files
- `test/` — C++ unit tests (`Boost.Test`)
- `py_test/` — Python unit tests for the native module and helper package
- `external/` — bundled headers (`mt4cpp`)
- `cmake_modules/` — CMake finders and toolchain helpers

---

## Building

### Requirements

- C++17 compiler (`GCC` or `Clang`)
- `CMake >= 3.12`
- `Boost` (`program_options`, `thread`, `unit_test_framework`, `python`, `numpy`)
- `Python 3` development headers (`python3-dev`) and `bitarray`, `numpy`, `matplotlib` for Python scripts

### Compile CLI & Library

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j 4 BGM-asm-program
```

### CMake Targets

- **`BGM-asm`** — core static C++ library
- **`BGM-asm-program`** — command-line assembler executable
- **`BGM-asm-test`** / **`BGM-asm-check`** — C++ unit tests (`Boost.Test`)
- **`BGM-module-python`** (`_bgm_module.so`) — Python 3 extension module (`Boost.Python`)
- **`BGM-module-python-test`** — unit tests for the Python bindings

---

## Usage

### Command-Line Options (`BGM-asm-program`)

| Option | Type | Description |
| :--- | :---: | :--- |
| `-h [ --help ]` | flag | Display help message |
| `-i [ --input ]` | arg | Input binary maps file (`.binmaps` / `.fasta01`) |
| `-o [ --output ]` | arg | Output consensus file path |
| `--margin_quant` | arg | Minimum overlap length in quants for an alignment to be considered meaningful |
| `--margin_part` | arg | Minimum fraction of map length required in overlap |
| `--thresh` | arg | Maximum allowed fraction of differing bits in overlap |
| `-t [ --threads ]` | arg (`1`) | Number of worker threads (`0` = auto-detect hardware concurrency) |
| `-f [ --folds ]` | arg (`1`) | Number of iterative assembly folds |
| `--marker_preference` | arg (`1`) | Tie-breaking preference (`1` = marker, `0` = empty) on 50/50 consensus |
| `--reuse_maps` | flag | Reuse restriction maps with contigs in subsequent folds |
| `-v [ --verbose ]` | flag | Verbose progress output |
| `-p [ --perf ]` | flag | Print execution time in seconds |

### Quick Start Example

1. Download an example dataset from [SourceForge Datasets](https://sourceforge.net/projects/binary-genome-maps/files/datasets/), or convert a raw `.bnx` file:

   ```bash
   pip3 install bitarray numpy matplotlib
   python3 ./python/bnx_convert.py -f ./AB22.bnx -p ./
   ```

2. Run the assembler with 4 folds and 4 threads:

   ```bash
   ./BGM-asm-program \
     -i ./e.coli-enz-BspQI-cov-40.0-q-1000.binmaps \
     -o ./my_result \
     --margin_quant 10 \
     --margin_part 0.65 \
     --thresh 0.15 \
     -f 4 -t 4
   ```

---

## Publication & Companion Archives

- **Paper (arXiv):** Przemysław Stawczyk, Robert Nowak, *"Fast genomic optical map assembly algorithm using binary representation"*, arXiv:2210.06865 [q-bio.GN], 2022 — [https://arxiv.org/abs/2210.06865v1](https://arxiv.org/abs/2210.06865v1) ([DOI: 10.48550/arXiv.2210.06865](https://doi.org/10.48550/arXiv.2210.06865))
- **SourceForge Companion Repository & Datasets:** [https://sourceforge.net/p/binary-genome-maps/code/ci/master/tree/](https://sourceforge.net/p/binary-genome-maps/code/ci/master/tree/) (benchmark datasets, evaluation notebooks, and comparison scripts)

```bibtex
@article{stawczyk2022fast,
  title   = {Fast genomic optical map assembly algorithm using binary representation},
  author  = {Stawczyk, Przemys{\l}aw and Nowak, Robert},
  journal = {arXiv preprint arXiv:2210.06865},
  year    = {2022},
  doi     = {10.48550/arXiv.2210.06865}
}
```
