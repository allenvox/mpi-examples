DIRGUARD = @mkdir -p $(@D)

CFLAGS = -Wall -Wextra -Werror
CXX = mpicxx
CXXFLAGS = -pedantic -std=c++17
CXXSRC = $(wildcard src/practice-3/*.cpp)
CXXEXEC = $(patsubst src/practice-3/%.cpp,bin/%,$(CXXSRC))

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