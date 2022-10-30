INCLUDE := include
SRC := src
TEST := test
DOC := doc
BUILD := build
TARGET := plotcpp

LIBXML2_LIBS := $(shell xml2-config --libs)
LIBXML2_CFLAGS := $(shell xml2-config --cflags)

CXX_FLAGS += \
	-std=c++20 \
	-O3 \
	-Wall -Werror \
	-Wextra -Wpedantic -Wconversion

LIB_CXXFLAGS += \
	$(LIBXML2_CFLAGS)

LIB_LDFLAGS += \
	$(LIBXML2_LIBS) \
	-lfmt

LIB_SOURCES += \
	$(SRC)/Figure.cpp \
	$(SRC)/Plot2D.cpp \
	$(SRC)/fonts.cpp \
	$(SRC)/svg.cpp


.PHONY: doc compiledb format syntax library tests

all: library

compiledb: build_dir
	compiledb make -n

format:
	clang-format -i --style=Google \
		$(INCLUDE)/* $(SRC)/* $(TEST)/*

build_dir:
	mkdir -p $(BUILD)

doc:
	doxygen

clean:
	rm -r $(BUILD)
	rm -r $(DOC)

cloc:
	@cloc $(INCLUDE) $(SRC) $(TEST)

syntax: compiledb
	$(CXX) $(CXX_FLAGS) \
		$(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(LIB_SOURCES) \
		-fsyntax-only

TEST_SOURCES += \
	$(TEST)/Plot2DTest.cpp \
	$(TEST)/UtilityTest.cpp \

tests: build_dir compiledb
	# Unit tests
	$(CXX) $(CXX_FLAGS) -g \
		$(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(LIB_LDFLAGS) -lgtest_main -lgtest \
		$(TEST_SOURCES) \
		$(LIB_SOURCES) \
		-o $(BUILD)/tests

	# plot2d tester
	$(CXX) $(CXX_FLAGS) -g \
		$(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(LIB_LDFLAGS) \
		$(LIB_SOURCES) \
		$(TEST)/figure_tester.cpp \
		-o $(BUILD)/figure_tester

	./$(BUILD)/tests


SHARED_LIB := lib$(TARGET).so
library: build_dir
	$(CXX) $(CXX_FLAGS) \
		-fPIC -shared \
		$(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(LIB_LDFLAGS) \
		$(LIB_SOURCES) \
		-o $(BUILD)/$(SHARED_LIB)

install:
	cp -r $(INCLUDE) /usr/include/$(TARGET)
	cp $(BUILD)/$(SHARED_LIB) /usr/lib64/$(SHARED_LIB)

uninstall:
	rm -r /usr/include/$(TARGET)
	rm /usr/lib64/$(SHARED_LIB)
