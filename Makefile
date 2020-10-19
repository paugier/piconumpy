
ifeq ($(PYTHON),)
PYTHON := python
endif

all:
	$(PYTHON) -m pip install -e .[dev]

build_ext_universal:
	$(PYTHON) setup.py --hpy-abi=universal build_ext -if

build_ext:
	$(PYTHON) setup.py build_ext -if

full:
	$(PYTHON) -m pip install -e .[full]

format:
	black -l 82 setup.py piconumpy/*.py
	clang-format-7 -i piconumpy/*cpython_capi.c

tests:
	$(PYTHON) -m pytest piconumpy -s

clean:
	rm -f piconumpy/*.so
	rm -rf build dist

black:
	black -l 82 .