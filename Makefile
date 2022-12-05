INCLUDE := include
SRC := src
TEST := test
EXAMPLES := examples
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


.PHONY: doc compiledb format syntax library cli tests

all: library cli

LIB_CXXFLAGS += \
	$(LIBXML2_CFLAGS)
LIB_LDFLAGS += \
	$(LIBXML2_LIBS) \
	-lfmt
LIB_SOURCES += \
	$(SRC)/version.cpp \
	$(SRC)/fonts.cpp \
	$(SRC)/svg.cpp \
	$(SRC)/components/Frame.cpp \
	$(SRC)/components/Legend.cpp \
	$(SRC)/Figure.cpp \
	$(SRC)/Plot2D.cpp \
	$(SRC)/BarPlotBase.cpp \
	$(SRC)/BarPlot.cpp \
	$(SRC)/HistogramPlot.cpp
SHARED_LIB := lib$(TARGET).so
library: build_dir
	$(CXX) $(CXX_FLAGS) \
		-fPIC -shared \
		$(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(LIB_SOURCES) \
		$(LIB_LDFLAGS) \
		-o $(BUILD)/$(SHARED_LIB)

CLI := cli
CLI_TARGET := plotcpp
CLI_MAIN := $(CLI)/plotcpp.cpp
CLI_SOURCES += \
	$(CLI)/SplitStringView.cpp \
	$(CLI)/csv.cpp \
	$(CLI)/Plot2DHandler.cpp \
	$(CLI)/HistogramHandler.cpp \
	$(CLI)/BarPlotHandler.cpp
cli: build_dir
	$(CXX) $(CXX_FLAGS) $(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(CLI_MAIN) $(CLI_SOURCES) \
		-L$(BUILD) -l$(TARGET) \
		-o $(BUILD)/$(CLI_TARGET)

cli-static: build_dir
	$(CXX) $(CXX_FLAGS) $(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(CLI_MAIN) $(CLI_SOURCES) \
		$(LIB_SOURCES) \
		$(LIB_LDFLAGS) \
		-o $(BUILD)/$(CLI_TARGET)_static

install:
	cp -r $(INCLUDE) /usr/include/$(TARGET)
	cp $(BUILD)/$(SHARED_LIB) /usr/lib64/$(SHARED_LIB)
	cp $(BUILD)/$(CLI_TARGET) /usr/bin/$(CLI_TARGET)

uninstall:
	rm -r /usr/include/$(TARGET)
	rm /usr/lib64/$(SHARED_LIB)
	rm /usr/bin/$(CLI_TARGET)

build_dir:
	mkdir -p $(BUILD)

compiledb:
	compiledb make -n
	compiledb make -n tests
	compiledb make -n figure-tester

everything: library cli cli-static figure-tester doc tests compiledb

format:
	clang-format -i --style=Google \
		$(shell find $(SOURCEDIR) -name '*.hpp') \
		$(shell find $(SOURCEDIR) -name '*.cpp')

tidy:
	clang-tidy \
		$(shell find $(SOURCEDIR) -name '*.hpp') \
		$(shell find $(SOURCEDIR) -name '*.cpp')

doc:
	doxygen

clean:
	rm -r $(BUILD)
	rm -r $(DOC)

cloc:
	@cloc Makefile $(INCLUDE) $(SRC) $(TEST) $(CLI) $(EXAMPLES)

LIB_TEST_SOURCES += \
	$(TEST)/UtilityTest.cpp
CLI_TEST_SOURCE += \
	$(CLI)/$(TEST)/SplitStringViewTest.cpp
tests: build_dir
	$(CXX) $(CXX_FLAGS) -g \
		$(LIB_CXXFLAGS) \
		-I$(INCLUDE) -I$(CLI) \
		$(LIB_SOURCES) $(CLI_SOURCES) \
		$(LIB_TEST_SOURCES) $(CLI_TEST_SOURCE) \
		$(LIB_LDFLAGS) -lpthread -lgtest_main -lgtest \
		-o $(BUILD)/tests

	cd $(BUILD) && ./tests

figure-tester: build_dir
	# Figure tester
	$(CXX) $(CXX_FLAGS) -g \
		$(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(LIB_SOURCES) \
		$(EXAMPLES)/figure_tester.cpp \
		$(LIB_LDFLAGS) \
		-o $(BUILD)/figure_tester
