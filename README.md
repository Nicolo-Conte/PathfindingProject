# PathfindingProject
PathfindingProgect is a tech demo that provides three different pathfinding algorithms, showing their behaviours and how they work

the program allows:
- customize the 2D grid where to run the pathfinding algorithms, having the ability to add and remove rows and columns, enlarge or shrink the grid tiles;
- choose one of the following pathfinding algorithms from: A*, BFS, DFS;
- enable or disable cross connections between tiles;
- add or remove obstacles on the grid;
- add an agent that walks the calculated path;
- the program tracks and writes the execution time for the path calculation, writing it to the console as a debug print
The program in addition to drawing the calculated path(with a white line), marks in gray the tiles that the algorithm checks during the calculation


For this project I used the following external libraries:
- FSML as the graphics engine 
- ImGui for UI
- glm for mathematical functions
