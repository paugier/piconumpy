
all: tmp.py
	python tmp.py

tmp.py: bench_array1d.py make_bench_piconumpy.py
	python make_bench_piconumpy.py

clean:
	transonic -cc bench_array1d.py -f
	transonic -cc tmp.py -f
	rm -f tmp.py
