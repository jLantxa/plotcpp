INCLUDE := include
SRC := src
TEST := test
DOC := doc
BUILD := build

LIBXML2_LIBS := $(shell xml2-config --libs)
LIBXML2_CFLAGS := $(shell xml2-config --cflags)

CXX_FLAGS += \
	-std=c++20 \
	-O2 \
	-Wall -Werror

LIB_SOURCES += \
	$(SRC)/Figure.cpp \
	$(SRC)/Plot2D.cpp \
	$(SRC)/svg.cpp


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
	$(TEST)/Plot2DTest.cpp \

tests: build_dir
	# Unit tests
	$(CXX) $(CXX_FLAGS) $(LIBXML2_CFLAGS) \
		-I$(INCLUDE) \
		$(LIB_SOURCES) $(TEST_SOURCES) \
		$(LIBXML2_LIBS) -lgtest_main -lgtest \
		-o $(BUILD)/tests

	# SVG Tester
	$(CXX) $(CXX_FLAGS) $(LIBXML2_CFLAGS) \
		-I$(INCLUDE) \
		$(TEST)/svg_tester.cpp $(SRC)/svg.cpp \
		$(LIBXML2_LIBS) \
		-o $(BUILD)/svg_tester

	./$(BUILD)/tests
