# Microbenchmarks low level Python code

We measure the performance for functions containing low level Python code.

- `sum_loop` (command `make bench`): `for value in arr` and summation

- `sum_loop_index` (command `make bench_sum_loop_index`):
  `for index in range(5000)` and summation

- `init_zeros` (command `make bench_init_zeros`): set values to zeros

- `cort` (command `make bench_cort`): normalized cosine similarity measure
  between derivatives

The files result_*.txt contain few results.
