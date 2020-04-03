# main build uses any .cpp files in src/
MAIN_SOURCES = $(wildcard src/*.cpp)
MAIN_OBJECTS = $(patsubst src/%.cpp,build/%.o,$(MAIN_SOURCES))

# test build uses any .cpp files in test/ plus any .cpp files in src/
TEST_SOURCES = $(wildcard test/*.cpp)
ALL_OBJECTS = $(patsubst test/%.cpp,build/%.o,$(TEST_SOURCES))
ALL_OBJECTS += $(MAIN_OBJECTS)

# for testing, remove main.o because it uses the main build's main()
TEST_OBJECTS = $(patsubst build/main.o,,$(ALL_OBJECTS))

MAIN_EXE = bin/solver.exe
TEST_EXE = bin/tester.exe

MAIN : $(MAIN_EXE)

TEST : $(TEST_EXE)

ALL : MAIN TEST

$(MAIN_EXE) : $(MAIN_OBJECTS)
	g++ -o $@ $^

$(TEST_EXE) : $(TEST_OBJECTS)
	g++ -o $@ $^

build/%.o : src/%.cpp
	@echo Compiling ... $@
	g++ -O3 -Wall -o $@ -c $^

build/%.o : test/%.cpp
	@echo Compiling ... $@
	g++ -O3 -Wall -o $@ -c $^