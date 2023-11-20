DIRGUARD = @mkdir -p $(@D)

CFLAGS = -Wall -Wextra -Werror
CXX = mpicxx
CXXFLAGS = -pedantic -std=c++17
CXXSRC = $(wildcard src/*/*.cpp)
OTHEXEC = $(patsubst src/other/%.cpp,bin/%,$(CXXSRC))
P1EXEC = $(patsubst src/practice-1/%.cpp,bin/%,$(CXXSRC))
P2EXEC = $(patsubst src/practice-2/%.cpp,bin/%,$(CXXSRC))
P3EXEC = $(patsubst src/practice-3/%.cpp,bin/%,$(CXXSRC))
P4EXEC = $(patsubst src/practice-4/%.cpp,bin/%,$(CXXSRC))
CXXEXEC = $(OTHEXEC) $(P1EXEC) $(P2EXEC) $(P3EXEC) $(P4EXEC)

UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
	CXXFLAGS += -ld_classic
endif

.PHONY: all
all: $(CXXEXEC)

bin/%: src/*/%.cpp
	$(DIRGUARD)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -o $@ $<

.PHONY: clean
.SILENT: clean
clean:
	rm -f $(CXXEXEC)