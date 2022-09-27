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
	-O2 \
	-Wall -Werror -Wpedantic -Wconversion

LIB_SOURCES += \
	$(SRC)/Figure.cpp \
	$(SRC)/Plot2D.cpp \
	$(SRC)/svg.cpp


.PHONY: doc compiledb syntax library tests

all: library

compiledb: build_dir
	compiledb make -n

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
	$(CXX) $(CXX_FLAGS) $(LIBXML2_CFLAGS) \
		-I$(INCLUDE) \
		$(LIB_SOURCES) \
		-fsyntax-only

TEST_SOURCES += \
	$(TEST)/Plot2DTest.cpp

tests: build_dir compiledb library
	# Unit tests
	$(CXX) $(CXX_FLAGS) $(LIBXML2_CFLAGS) \
		-I$(INCLUDE) \
		-L$(LD_LIBRARY_PATH) -l$(TARGET) \
		$(LIBXML2_LIBS) -lgtest_main -lgtest \
		$(TEST_SOURCES) \
		-o $(BUILD)/tests

	# plot2d tester
	$(CXX) $(CXX_FLAGS) $(LIBXML2_CFLAGS) \
		-I$(INCLUDE) \
		-L$(LD_LIBRARY_PATH) -l$(TARGET) \
		$(TEST)/plot2d_tester.cpp \
		-o $(BUILD)/plot2d_tester

	./$(BUILD)/tests


SHARED_LIB := lib$(TARGET).so
library: build_dir
	$(CXX) -fPIC -shared $(CXX_FLAGS) $(LIBXML2_CFLAGS) \
		-I$(INCLUDE) \
		$(LIBXML2_LIBS) \
		$(LIB_SOURCES) \
		-o $(BUILD)/$(SHARED_LIB)

install:
	cp -r $(INCLUDE) /usr/include/$(TARGET)
	cp $(BUILD)/$(SHARED_LIB) /usr/lib64/$(SHARED_LIB)

uninstall:
	rm -r /usr/include/$(TARGET)
	rm /usr/lib64/$(SHARED_LIB)
