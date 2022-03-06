CCGREEN=$(shell echo -e -n "\033[0;32m")
CCRESET=$(shell echo -e -n "\033[0m")

CXX := clang++
CXXFLAGS :=-Wall -std=c++20

LIBS := 

TARGET := leor

INCDIR := inc
SRCDIR := src
BUILDDIR := build

all: build $(TARGET)

SRC := \
	$(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)

OBJ := \
	$(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRC))

DEPENDENCIES := \
	$(OBJECTS:.o=.d)

$(OBJ): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@echo -e "$(CCGREEN)[C++]$(CCRESET) Building $@ from $<"
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -I./$(INCDIR) -MMD -c -o $@ $<

$(TARGET): $(OBJ)
	@$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release run

build:
	@mkdir -p $(BUILDDIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(BUILDDIR)/*.o $(BUILDDIR)/*.d $(TARGET)

run:
	@echo -e "$(CCGREEN)[CMD]$(CCRESET) Running ./$(TARGET)"
	@./$(TARGET)
