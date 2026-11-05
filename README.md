# Error-correcting codes (Task A11)

**University Coursework** | *Error-Correcting Codes / Coding Theory*

Command-line simulator of binary linear codes over a noisy channel, written in C.
Encodes data with a generator matrix, sends it through a binary symmetric channel,
and decodes it with the step-by-step algorithm.

## Features
- Encoding with any valid binary linear code C[n,k]
- Step-by-step decoding
- Three scenarios: single vector, text, BMP image
- Reproducible runs with `--seed`

## Build
```bash
gcc -O3 -DNDEBUG -fopenmp src/*.c -o A11
```

## Usage
```bash
./A11 -p 0.1 -n 7 -k 4 -g 1000101,0100110,0010111,0001011 -s 3 --seed 123
```
## Planned improvements
- Precompute the parity-check matrix and syndrome tables once per run.
- Use an arena allocator to simplify memory management.

## Report
Full report (Lithuanian): `ataskaita/ataskaita.pdf`
