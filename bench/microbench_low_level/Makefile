
.PHONY : clean bench_sum_loop bench_sum_loop_index bench_cort bench_init_zeros

bench_sum_loop: NAME_BENCH=sum_loop
bench_sum_loop: tmp_julia_sum_loop.txt _bench

bench_sum_loop_index: NAME_BENCH=sum_loop_index
bench_sum_loop_index: tmp_julia_sum_loop_index.txt _bench

bench_cort: NAME_BENCH=cort
bench_cort: tmp_julia_cort.txt _bench

bench_init_zeros: NAME_BENCH=init_zeros
bench_init_zeros: tmp_julia_init_zeros.txt _bench

_bench:
	@echo bench $(NAME_BENCH)
	@python -c "from socket import gethostname as f; print('hostname:', f())"
	@python -c "import sys; from pprint import pprint as p; p({key: sys.implementation.__dict__[key] for key in ('cache_tag', 'version')})"
	@python bench.py list $(NAME_BENCH)
	@python bench.py purepy $(NAME_BENCH)
	@python bench.py numpy $(NAME_BENCH)
	@python bench.py _piconumpy_hpy $(NAME_BENCH)
	@python bench.py _piconumpy_cpython_capi $(NAME_BENCH)

tmp_julia_sum_loop.txt: bench_sum_loop.jl
	@julia bench_sum_loop.jl > tmp_julia_sum_loop.txt

tmp_julia_sum_loop_index.txt: bench_sum_loop_index.jl
	@julia bench_sum_loop_index.jl > tmp_julia_sum_loop_index.txt

tmp_julia_cort.txt: bench_cort.jl
	@julia bench_cort.jl > tmp_julia_cort.txt

tmp_julia_init_zeros.txt: bench_init_zeros.jl
	@julia bench_init_zeros.jl > tmp_julia_init_zeros.txt

clean:
	rm -f tmp_julia_*.txt