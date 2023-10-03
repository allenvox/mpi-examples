CC = mpicc
CFLAGS = -Wall -Wextra -Werror
CXX = mpicxx
CXXFLAGS = -pedantic -std=c++17
CXXSRC = $(wildcard src/practice-1/*.cpp)
CXXEXEC = $(patsubst src/practice-1/%.cpp,bin/%,$(CXXSRC))
DIRGUARD = @mkdir -p $(@D)

.PHONY: all
all: $(CXXEXEC)

bin/%: src/practice-1/%.cpp
	$(DIRGUARD)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -o $@ $<

.PHONY: clean
.SILENT: clean
clean:
	rm -f $(CXXEXEC)

.PHONY: format
.SILENT: format
format:
	clang-format -i $(CXXSRC)