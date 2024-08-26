CXX := clang
CXXFLAGS := -std=c++11
SRC_DIR := src
EXE_DIR := build
OBJ_DIR := obj
EXE := $(shell find src | grep '.cpp' | xargs -I {} bash -c "echo {} | gsed 's/.cpp/.so/' | gsed "s%${SRC_DIR}/%${EXE_DIR}/%"")

all: ${EXE}

.PHONY: clean all

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -fPIC -c $^ -o $@

${EXE}: ${EXE_DIR}/%.so: ${OBJ_DIR}/%.o
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -shared -o $@

clean:
	rm -r ${OBJ_DIR} ${EXE_DIR}
