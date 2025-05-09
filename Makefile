
ifeq ($(PYTHON),)
PYTHON := python
endif

all:
	make editable_universal
ifeq ($(PYTHON),python)
	make editable
endif

editable:
	$(PYTHON) -m pip install -e .

editable_universal:
	$(PYTHON) -m pip install -e . --config-settings="--global-option=--hpy-abi=universal"
	rm -f piconumpy/_piconumpy_hpy.py

develop:
	$(PYTHON) setup.py develop

develop_universal:
	$(PYTHON) setup.py --hpy-abi=universal develop
	rm -f piconumpy/_piconumpy_hpy.py

build_ext_universal:
	$(PYTHON) setup.py --hpy-abi=universal build_ext -if

build_ext:
	$(PYTHON) setup.py build_ext -if
	rm -f piconumpy/_piconumpy_hpy.py

full:
	$(PYTHON) -m pip install -e .[full]

format:
	black -l 82 setup.py piconumpy/*.py
	clang-format-7 -i piconumpy/*cpython_capi.c

tests:
	$(PYTHON) -m pytest piconumpy -s

clean:
	rm -f piconumpy/*.so
	rm -rf build dist piconumpy.egg-info

black:
	black -l 82 .