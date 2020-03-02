
ifeq ($(PYTHON),)
PYTHON := python
endif

all: tmp.py
	$(PYTHON) tmp.py

tmp.py: bench_array1d.py make_bench_piconumpy.py
	$(PYTHON) make_bench_piconumpy.py

clean:
	transonic -cc bench_array1d.py -f
	rm -f tmp*.py
