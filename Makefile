
ifeq ($(PYTHON),)
PYTHON := python
endif

all:
	$(PYTHON) -m pip install -e .[dev]

full:
	$(PYTHON) -m pip install -e .[full]

format:
	black -l 82 setup.py piconumpy/*.py
	clang-format-7 -i piconumpy/*cpython_capi.c

tests:
	$(PYTHON) -m pytest piconumpy -s

clean:
	rm -f piconumpy/*.so

black:
	black -l 82 .