
all: tmp_result_julia.txt
	@python -c "import sys; from pprint import pprint as p; p({key: sys.implementation.__dict__[key] for key in ('cache_tag', 'version')})"
	@python bench.py list
	@python bench.py purepy
	@python bench.py numpy
	@python bench.py _piconumpy_hpy
	@python bench.py _piconumpy_cpython_capi

tmp_result_julia.txt: bench.jl
	@julia bench.jl > tmp_result_julia.txt

clean:
	rm -f tmp_*.txt

produce_traces: tmp_result_julia.txt
	PYPYLOG=jit-log-opt,jit-summary,jit-backend-counts:tmp_traces_list.txt pypy bench.py list
	PYPYLOG=jit-log-opt,jit-summary,jit-backend-counts:tmp_traces_piconumpy_list.txt pypy bench.py purepy
	PYPYLOG=jit-log-opt,jit-summary,jit-backend-counts:tmp_traces_piconumpy_hpy.txt pypy bench.py _piconumpy_hpy
