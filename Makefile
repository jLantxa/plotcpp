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


.PHONY: doc compiledb format syntax library app tests

all: library app

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

APP := app
APP_TARGET := plotcpp
APP_MAIN := $(APP)/plotcpp.cpp
APP_SOURCES += \
	$(APP)/SplitStringView.cpp \
	$(APP)/csv.cpp \
	$(APP)/Plot2DHandler.cpp
app: build_dir
	$(CXX) $(CXX_FLAGS) $(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(APP_MAIN) $(APP_SOURCES) \
		-L$(BUILD) -l$(TARGET) \
		-o $(BUILD)/$(APP_TARGET)

app-static: build_dir
	$(CXX) $(CXX_FLAGS) $(LIB_CXXFLAGS) \
		-I$(INCLUDE) \
		$(APP_MAIN) $(APP_SOURCES) \
		$(LIB_SOURCES) \
		$(LIB_LDFLAGS) \
		-o $(BUILD)/$(APP_TARGET)_static

install:
	cp -r $(INCLUDE) /usr/include/$(TARGET)
	cp $(BUILD)/$(SHARED_LIB) /usr/lib64/$(SHARED_LIB)
	cp $(BUILD)/$(APP_TARGET) /usr/bin/$(APP_TARGET)

uninstall:
	rm -r /usr/include/$(TARGET)
	rm /usr/lib64/$(SHARED_LIB)
	rm /usr/bin/$(APP_TARGET)

build_dir:
	mkdir -p $(BUILD)

compiledb:
	compiledb make -n
	compiledb make -n tests
	compiledb make -n figure-tester

everything: library app app-static figure-tester doc tests

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
	@cloc Makefile $(INCLUDE) $(SRC) $(TEST) $(APP) $(EXAMPLES)

LIB_TEST_SOURCES += \
	$(TEST)/UtilityTest.cpp
APP_TEST_SOURCE += \
	$(APP)/$(TEST)/SplitStringViewTest.cpp
tests: build_dir
	$(CXX) $(CXX_FLAGS) -g \
		$(LIB_CXXFLAGS) \
		-I$(INCLUDE) -I$(APP) \
		$(LIB_SOURCES) $(APP_SOURCES) \
		$(LIB_TEST_SOURCES) $(APP_TEST_SOURCE) \
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
