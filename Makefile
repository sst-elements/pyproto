CXX = $(shell sst-config --CXX)
CXXFLAGS = $(shell sst-config --ELEMENT_CXXFLAGS) $(shell sst-config --PYTHON_CPPFLAGS)
LDFLAGS  = $(shell sst-config --ELEMENT_LDFLAGS)

SRC = $(wildcard *.cc)
OBJ = $(SRC:%.cc=.build/%.o)
DEP = $(OBJ:%.o=%.d)

.PHONY: all install uninstall clean

all: install

-include $(DEP)
.build/%.o: %.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

libpyproto.so: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(shell sst-config --PYTHON_LDFLAGS)

install: libpyproto.so
	sst-register pyproto pyproto_LIBDIR=$(CURDIR)

uninstall:
	sst-register -u pyproto

clean: uninstall
	rm -rf .build libpyproto.so
