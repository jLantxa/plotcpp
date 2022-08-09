INCLUDE := include
SRC := src
TEST := test
DOC := doc
BUILD := build

CXX_FLAGS += \
	-std=c++20 \
	-O2 \
	-Wall -Werror

build_dir:
	mkdir -p $(BUILD)

doc:
	@doxygen

clean:
	rm -r $(BUILD)
