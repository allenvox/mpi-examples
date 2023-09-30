CC = mpicc
CFLAGS = -Wall -Wextra -Werror -Wshadow
CSRC = $(wildcard */*.c)
CEXEC = $(wildcard */*.cout)

CXX = mpicxx
CXXFLAGS = -pedantic -std=c++17 -ld_classic
CXXSRC = $(wildcard */*.cpp)
CXXEXEC = $(wildcard */*.cppout)

.PHONY: clean
.SILENT: clean
clean:
	rm -f $(CEXEC) $(CXXEXEC)

.PHONY: format
.SILENT: format
format:
	clang-format -i $(CSRC) $(CXXSRC)

%.cout: %.c
	$(CC) $(CFLAGS) -o $@ $<

%.cppout: %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) -o $@ $<