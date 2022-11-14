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
	$(SRC)/version.cpp \
	$(SRC)/fonts.cpp \
	$(SRC)/svg.cpp \
	$(SRC)/Figure.cpp \
	$(SRC)/Plot2D.cpp \
	$(SRC)/BarPlotBase.cpp \
	$(SRC)/BarPlot.cpp \
	$(SRC)/HistogramPlot.cpp


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
	$(TEST)/UtilityTest.cpp \

tests: build_dir compiledb
	# Unit tests
	$(CXX) $(CXX_FLAGS) -g \
		$(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(LIB_LDFLAGS) -lpthread -lgtest_main -lgtest \
		$(TEST_SOURCES) \
		$(LIB_SOURCES) \
		-o $(BUILD)/tests

	./$(BUILD)/tests


figure-tester: build_dir compiledb
	# Figure tester
	$(CXX) $(CXX_FLAGS) -g \
		$(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(LIB_LDFLAGS) \
		$(LIB_SOURCES) \
		$(TEST)/figure_tester.cpp \
		-o $(BUILD)/figure_tester

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
