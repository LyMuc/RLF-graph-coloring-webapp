# Graph Coloring Web Application

## Description

### Introduction

This application is created with the aim of providing a feasible solution for the graph coloring problem. The graph coloring problem requires assigning each vertex of a graph a color such that no two adjacent vertices share the same color. The graph coloring problem can be applied to solve several real-world problems such as scheduling, register allocation, sports scheduling, etc. (See [here](https://en.wikipedia.org/wiki/Graph_coloring) for more details on the graph coloring problem).

### Algorithms Used

The graph coloring problem is an NP-hard problem, and to date, there is no general algorithm that always finds a coloring with the minimum number of colors for any given graphs. Three well-known algorithms provide a correct solution (a valid coloring) for any input graphs: Dynamic Programming, DSatur, and RLF.

### Observations on the Algorithms:
1. For graphs with 20 or fewer vertices, the Dynamic Programming algorithm always provides the optimal result (the number of colors used equals the chromatic number of the graph). ([DP algorithm for graph coloring](https://www.cs.helsinki.fi/u/jwkangas/presentations/slides-eea.pdf))
2. The DSatur algorithm provides the optimal result if the input graph is a bipartite graph, a cycle graph, or a wheel graph. ([Dsatur algorithm](https://en.wikipedia.org/wiki/DSatur) and [Pseudocode of Dsatur](https://arxiv.org/pdf/2108.09329))
3. The RLF algorithm provides the optimal result if the input graph is a bipartite graph, a cycle graph, or a wheel graph. Generally, the RLF algorithm gives **better results than the DSatur algorithm** but takes more time. ([RLF algorithm](https://en.wikipedia.org/wiki/Recursive_largest_first_algorithm) and [Pseudocode of RLF](https://arxiv.org/pdf/2108.09329))
4. The DP, DSatur, and RLF algorithms provide a valid coloring for any input graphs. However, most results still require more colors than the chromatic number of the graph (not optimal).

### Web Application Implementation:
- This web application uses the DP and RLF algorithms to solve the problem.
  - The RLF algorithm is implemented with some improvements as stated in [this paper](https://www.gerad.ca/~alainh/RLFPaper.pdf). Specifically, if the input graph has more than 20 vertices and fewer than 2000 vertices, it will be implemented according to the improvement in section 3.2.(a). If the graph has more than 2000 vertices, it will be implemented according to the improvement in section 3.2.(b) with M=10.

### Technologies Used:
- Python Django for the backend.
- HTML, Javascript to draw graphs and create the web interface.
- All the algorithms used are implemented in C++.

### Key Features:
Although the response time is slow (due to the high complexity of the DP and RLF algorithms), it usually provides an optimal solution (using fewer colors) compared to other web applications that mainly rely on greedy algorithms to solve the graph coloring problem.

## How to Run the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo/graph-coloring.git
   cd graph-coloring
