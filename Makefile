CXX := g++
CXXFLAGS := --std=c++11
SRC_DIR := src
EXE_DIR := build
OBJ_DIR := obj
MAIN_CPP := main.cpp
EXE := $(shell find src | grep '.cpp' | xargs -I {} bash -c "echo {} | gsed 's/.cpp/.exe/' | gsed "s%${SRC_DIR}/%${EXE_DIR}/%"")

all: ${EXE}

.PHONY: clean all

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

${EXE}: ${EXE_DIR}/%.exe: ${OBJ_DIR}/%.o ${MAIN_CPP}
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -r ${OBJ_DIR} ${EXE_DIR}
