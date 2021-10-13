
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
	rm -f tmp_result_julia.txt