
all:
	pip install -e .[dev]

format:
	black -l 82 setup.py piconumpy/*.py
	clang-format-7 -i piconumpy/*.c

tests:
	pytest piconumpy/test.py -s