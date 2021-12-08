
.PHONY : clean print_info bench bench_sum_loop_index bench_cort bench_init_zeros

bench: tmp_julia_sum_loop.txt
	@$(MAKE) --no-print-directory print_info NAME_BENCH=sum_loop
	@python bench.py list
	@python bench.py purepy
	@python bench.py numpy
	@python bench.py _piconumpy_hpy
	@python bench.py _piconumpy_cpython_capi

bench_sum_loop_index: tmp_julia_sum_loop_index.txt
	@$(MAKE) --no-print-directory print_info NAME_BENCH=sum_loop_index
	@python bench.py list sum_loop_index
	@python bench.py purepy sum_loop_index
	@python bench.py numpy sum_loop_index
	@python bench.py _piconumpy_hpy sum_loop_index
	@python bench.py _piconumpy_cpython_capi sum_loop_index

bench_cort: tmp_julia_cort.txt
	@$(MAKE) --no-print-directory print_info NAME_BENCH=cort
	@python bench.py list cort
	@python bench.py purepy cort
	@python bench.py numpy cort
	@python bench.py _piconumpy_hpy cort
	@python bench.py _piconumpy_cpython_capi cort

bench_init_zeros: tmp_julia_init_zeros.txt
	@$(MAKE) --no-print-directory print_info NAME_BENCH=init_zeros
	@python bench.py list init_zeros
	@python bench.py purepy init_zeros
	@python bench.py numpy init_zeros
	@python bench.py _piconumpy_hpy init_zeros
	@python bench.py _piconumpy_cpython_capi init_zeros

print_info:
	@echo bench $(NAME_BENCH)
	@python -c "from socket import gethostname as f; print('hostname:', f())"
	@python -c "import sys; from pprint import pprint as p; p({key: sys.implementation.__dict__[key] for key in ('cache_tag', 'version')})"

tmp_julia_sum_loop.txt: bench.jl
	@julia bench.jl > tmp_julia_sum_loop.txt

tmp_julia_sum_loop_index.txt: bench_sum_loop_index.jl
	@julia bench_sum_loop_index.jl > tmp_julia_sum_loop_index.txt

tmp_julia_cort.txt: bench_cort.jl
	@julia bench_cort.jl > tmp_julia_cort.txt

tmp_julia_init_zeros.txt: bench_init_zeros.jl
	@julia bench_init_zeros.jl > tmp_julia_init_zeros.txt

clean:
	rm -f tmp_julia_*.txt