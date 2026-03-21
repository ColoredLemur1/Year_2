# COMP3221 Coursework 1 – Explanation & Plan

## Compiling (Mac with Clang + OpenMP)

```bash
clang -Xpreprocessor -fopenmp -I$(brew --prefix libomp)/include -L$(brew --prefix libomp)/lib -lomp -Wall -o cwk1 cwk1.c -lm
```

---

## 1. What the coursework is about

You have a C program that loads a square greyscale PGM image and runs one of four operations (chosen by a command-line option). Your job is to **parallelise those four operations with OpenMP** so they run correctly and efficiently on a shared-memory CPU. You may only use material up to and including Lecture 6.

- **Input:** Filename of a square PGM (e.g. `image.pgm`) and an option number 1–4.
- **Output:** Depends on the option (see below). All outputs are written to fixed filenames.
- **Constraints:** Do not change `cwk1_extra.h`; only edit `cwk1.c` (and the makefile if you add files). The autograder replaces `cwk1_extra.h`, so your solution must work with the provided structs and functions.

**Marking:** 4 marks for correct functionality (1 per task), 5 for efficient parallelisation of tasks 1 & 2, 6 for efficient parallelisation of tasks 3 & 4. The autograder on Gradescope must pass; code is then inspected for efficiency.

---

## 2. The four operations (what each must do)

| # | Function               | What it does | Output file      |
|---|------------------------|--------------|------------------|
| 1 | `saveThresholdImage()` | Pixel > 127 → 255 (white), else 0 (black) | `threshold.pgm` |
| 2 | `saveFlippedImage()`   | Vertically flip: `pixels[row][col]` ↔ `pixels[size-row-1][col]` | `flipped.pgm` |
| 3 | `saveEdgeImage()`      | Each pixel (row,col) ← `edgeValue(row,col,img)`; use **red–black** ordering (Lecture 5) | `edge.pgm` |
| 4 | `generateHistogram()`  | Count how many pixels have value 0, 1, … 255; write counts to file | `histogram.dat` |

- **Edge:** `edgeValue()` in `cwk1_extra.h` reads only `[row±1][col]` and `[row][col±1]`; boundaries are 0. You must use the **red–black algorithm** so parallel updates don’t have read/write conflicts.
- **Histogram:** Many threads increment the same bins; you must avoid race conditions (e.g. `#pragma omp atomic` or local histograms then merge).

---

## 3. Plan: where to change code (no coding, just locations)

All edits are in **`cwk1.c`** unless you introduce new files (then update the makefile).

---

### Task 1: `saveThresholdImage()` — **cwk1.c, lines 42–53**

**Current code:** A double loop over `row` and `col` that sets each pixel to 255 or 0 based on `> 127`. No dependencies between iterations.

**What to do:**

- Add OpenMP so the loop(s) run in parallel. Options: parallelise the outer loop with `#pragma omp parallel for`, or use a single `#pragma omp parallel for collapse(2)` over both indices. Declare loop variables in the pragma or as private where needed.
- Keep the existing logic (threshold and assignment) and the call to `writeThresholdImage( img );` unchanged.

**Area to modify:** The two nested `for` loops (and optionally variable declarations) so they execute in parallel; do not change the body of the inner loop or the write call.

---

### Task 2: `saveFlippedImage()` — **cwk1.c, lines 56–62**

**Current code:** Only the two write/comment lines; no loop yet.

**What to do:**

- Implement the vertical flip: for each `(row, col)`, swap `img->pixels[row][col]` with `img->pixels[size-row-1][col]`. You only need to iterate over the top half of rows (otherwise you swap twice and undo the flip). So: loop over `row` from `0` to `size/2 - 1` (or similar), and for each row loop over all `col`; swap `pixels[row][col]` and `pixels[size-row-1][col]`.
- Parallelise this loop (e.g. `#pragma omp parallel for` on the row loop). Each pair of rows is independent.
- Leave the call `writeFlippedImage( img );` as is.

**Area to modify:** The whole body of `saveFlippedImage()` between the opening brace and the `writeFlippedImage( img );` call: add the loop(s) and OpenMP directives.

---

### Task 3: `saveEdgeImage()` — **cwk1.c, lines 65–75**

**Current code:** Comments describing that each pixel should get `edgeValue(row,col,img)`; no loop.

**What to do:**

- You must not overwrite the image in place while other pixels are still being read, because `edgeValue(row,col,img)` reads neighbouring pixels. So use a **separate output buffer** (e.g. a second 2D array of size `size×size`) and write the result of `edgeValue(row,col,img)` into that buffer; then copy the buffer back into `img->pixels` (or into the image struct) before calling `writeEdgeImage( img )`. Alternatively, use the red–black pattern directly on the image if you can guarantee no read-after-write conflicts (spec asks for red–black).
- **Red–black algorithm (Lecture 5):** Partition pixels into “red” and “black” (e.g. `(row+col) % 2`). In one parallel phase, update all red pixels from the current image; in a second phase, update all black pixels. Then all reads see the same “previous” values, so no race. If you use a separate output buffer and write only to that buffer, you can parallelise over all interior pixels in one go (no red–black needed), but the spec explicitly asks for red–black, so implement two phases (red then black) or equivalent.
- Restrict updates to interior pixels (row and col not on the boundary) since `edgeValue` returns 0 on the boundary; set boundary in the output to 0.
- Allocate/free any extra buffer in this function; do not change `cwk1_extra.h`. You can use the pattern from `cwk1_extra.h` (e.g. `allocSquareGrid`) for a temporary grid if you need one.
- Call `writeEdgeImage( img );` at the end with the image containing the final edge result.

**Area to modify:** The whole body of `saveEdgeImage()` between the opening brace and `writeEdgeImage( img );`: add buffer/red–black loops, OpenMP directives, and copy-back if needed.

---

### Task 4: `generateHistogram()` — **cwk1.c, lines 77–100**

**Current code:** `calloc` for `hist`, then a double loop over all pixels that does `hist[val]++` (with a range check). The `calloc` and `saveHistogram( hist );` / `free( hist );` should stay.

**What to do:**

- The critical issue is that `hist[val]++` is a read-modify-write; multiple threads must not update the same bin without synchronisation. Two standard approaches:
  - **Option A:** Use `#pragma omp atomic` (or a critical section) around the increment. Simple but can be a bottleneck.
  - **Option B (usually better):** Give each thread a private copy of the histogram (e.g. `hist_local[0..MAXVALUE]`), initialise to 0, then in parallel each thread updates its own copy while iterating over a subset of pixels (e.g. parallelise over rows). After the parallel region, merge all private histograms into `hist` in a single thread (or with a small parallel loop over bins).
- Parallelise the loop that goes over pixels (e.g. parallel for on the row loop). Keep the condition `if( val>=0 && val<=MAXVALUE )` and the logic that maps `val` to a bin.
- Do not parallelise the initialisation of `hist` (spec says you don’t need to). Do not remove the `saveHistogram( hist );` or `free( hist );` calls.

**Area to modify:** The double `for` loop (lines 86–93) and the code immediately around it: add OpenMP and either atomic/critical updates or private histograms + merge. Leave the rest of the function (calloc, saveHistogram, free) as is.

---

## 4. Summary table: where to edit

| Task | Function               | File    | Lines / region to change |
|------|------------------------|---------|---------------------------|
| 1    | `saveThresholdImage()` | cwk1.c  | 44–50: parallelise the two nested loops |
| 2    | `saveFlippedImage()`   | cwk1.c  | 58–61: add flip loop(s) and OpenMP (before `writeFlippedImage`) |
| 3    | `saveEdgeImage()`      | cwk1.c  | 67–73: add red–black (or buffer) + OpenMP, then call write |
| 4    | `generateHistogram()`  | cwk1.c  | 83–93: parallelise pixel loop and make histogram updates safe (atomic or private + merge) |

---

## 5. Things to remember

- **Do not modify** `cwk1_extra.h` or copy its content elsewhere and change it; it will be replaced for assessment.
- Run and test on the **school Unix environment** (e.g. feng-linux.leeds.ac.uk) so behaviour matches the autograder; avoid relying on “it works on my machine” if that’s a different OS/compiler.
- If you use AI tools, add a short comment in the code stating what you used and how (amber category).
- Submit **cwk1.c** (and any extra files + makefile if you added them) in one flat directory to Gradescope; ensure the autograder passes before the deadline.
