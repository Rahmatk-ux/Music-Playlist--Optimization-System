CXX      = g++
CXXFLAGS = -std=c++17 -Wall -O2
TARGET   = playlist_optimizer.exe
SRC_DIR  = src

SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/Graph.cpp \
       $(SRC_DIR)/Dijkstra.cpp \
       $(SRC_DIR)/TSP.cpp \
       $(SRC_DIR)/MST.cpp \
       $(SRC_DIR)/TwoOpt.cpp \
       $(SRC_DIR)/Kruskal.cpp \
       $(SRC_DIR)/AStar.cpp

all: $(TARGET)
	@echo "Build complete: $(TARGET)"

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) $(SRCS) -o $(TARGET)

clean:
	del $(TARGET) 2>nul || rm -f $(TARGET)

test:
	@echo Testing dijkstra...
	echo {"algorithm":"dijkstra","songIds":[1,2,3,4,5,6,7,8,9,10],"startSongId":1,"endSongId":10} > test_input.json
	./$(TARGET) run test_input.json test_output.json
	@echo Testing twoopt...
	echo {"algorithm":"twoopt","songIds":[1,2,3,4,5,6,7,8,9,10],"startSongId":1,"endSongId":10} > test_input.json
	./$(TARGET) run test_input.json test_output.json
	@echo Testing kruskal...
	echo {"algorithm":"kruskal","songIds":[1,2,3,4,5,6,7,8,9,10],"startSongId":1,"endSongId":10} > test_input.json
	./$(TARGET) run test_input.json test_output.json
	@echo Testing astar...
	echo {"algorithm":"astar","songIds":[1,2,3,4,5,6,7,8,9,10],"startSongId":1,"endSongId":10} > test_input.json
	./$(TARGET) run test_input.json test_output.json
	@echo All tests passed!
