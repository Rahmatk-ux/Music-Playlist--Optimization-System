CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
TARGET = playlist_optimizer
SRCDIR = src
BUILDDIR = build

SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/Graph.cpp \
          $(SRCDIR)/Dijkstra.cpp \
          $(SRCDIR)/TSP.cpp \
          $(SRCDIR)/MST.cpp

OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run