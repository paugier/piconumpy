
ifeq ($(PYTHON),)
PYTHON := python
endif

all: tmp.py tmp_result_julia.txt
	$(PYTHON) tmp.py

tmp.py: bench_array1d.py make_bench_piconumpy.py
	$(PYTHON) make_bench_piconumpy.py

clean:
	transonic -cc bench_array1d.py -f
	rm -f tmp*.py tmp*.txt

tmp_result_julia.txt:
	julia bench.jl > tmp_result_julia.txt

profile: tmp.py
	$(PYTHON) profile_piconumpy.py
	# with gprof2dot and graphviz (command dot)
	gprof2dot -f pstats tmp.pstats | dot -Tpng -o tmp.png
	eog tmp.png