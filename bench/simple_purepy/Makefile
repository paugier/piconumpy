
ifeq ($(PYTHON),)
PYTHON := python
endif

profile_with_arrays:
	$(PYTHON) profile_array_classes.py
	# with gprof2dot and graphviz (command dot)
	gprof2dot -f pstats tmp_array.pstats | dot -Tpng -o tmp_array.png
	eog tmp_array.png

profile_with_lists:
	$(PYTHON) profile_array_classes.py list
	# with gprof2dot and graphviz (command dot)
	gprof2dot -f pstats tmp_list.pstats | dot -Tpng -o tmp_list.png
	eog tmp_list.png

tmp_result_julia.txt:
	julia bench.jl > tmp_result_julia.txt

bench_with_arrays: tmp_result_julia.txt
	$(PYTHON) bench.py

bench_with_lists: tmp_result_julia.txt
	$(PYTHON) bench.py list

clean:
	rm -f tmp*