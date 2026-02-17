.SILENT:
CXX = g++
CXXFLAGS = -O3 -g0
LDFLAGS = -lasound

TARGET := mercury

BUILD_DIR := build
INC_DIRS := include
SRC_DIRS := source

SRCS := $(shell find $(SRC_DIRS) -name '*.cc')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

CPPFLAGS := -I $(INC_DIRS)

mercury: $(BUILD_DIR)/$(TARGET)
	echo "Compiling Mercury is done."

$(BUILD_DIR)/$(TARGET): $(OBJS)
	echo "Linking Mercury."
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cc.o: %.cc
	echo "Compiling" $<
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


install:


doc:
	echo "Building Mercury's documentation."
	doxygen ./mercury.doxyfile
	cp ./docs_FSM/*.png html


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
