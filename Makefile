INCLUDE := include
SRC := src
TEST := test
DOC := doc
BUILD := build

CXX_FLAGS += \
	-std=c++20 \
	-O2 \
	-Wall -Werror

LIB_SOURCES += \
	$(SRC)/Figure.cpp \
	$(SRC)/Plot2D.cpp


.PHONY: doc

all: compiledb tests

compiledb: build_dir
	compiledb make -n

build_dir:
	mkdir -p $(BUILD)

doc:
	doxygen

clean:
	rm -r $(BUILD)
	rm -r $(DOC)

TEST_SOURCES += \
	$(TEST)/Plot2DTest.cpp

tests: build_dir
	$(CXX) $(CXX_FLAGS) \
		-I $(INCLUDE) \
		$(LIB_SOURCES) $(TEST_SOURCES) \
		-lgtest_main -lgtest \
		-o $(BUILD)/tests

	./$(BUILD)/tests
