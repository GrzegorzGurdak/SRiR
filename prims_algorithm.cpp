#include <cassert>
#include <optional>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include <algorithm>

#include <fstream>
#include <iostream>

#define assertm(exp, msg) assert((void(msg), (exp)))

class Matrix{
public:
    Matrix(int _xSize, int _ySize): size{_xSize*_ySize}, xSize{_xSize}, ySize{_ySize}, data{new std::optional<double>[_xSize*_ySize]} {}
    ~Matrix() {delete[] data;}

    Matrix(Matrix &other) : Matrix(other.xSize, other.ySize) {
        std::copy(other.data, other.data + other.getSize(), data);
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    Matrix(Matrix &&other) : size{other.getSize()}, xSize{other.getXSize()}, ySize{other.getYSize()}, data{other.data}
    {
        other.data = nullptr;
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    std::optional<double> &get(int x, int y){
        assertm(x+y*xSize < size, "out of bounds");
        return data[x + y*xSize];
    }

    int getXSize(){return xSize;}
    int getYSize(){return ySize;}
    int getSize(){return size;}

private:
    const int size;
    const int xSize, ySize;
    std::optional<double> *data;
};

class Graph{
public:
    Graph(int n): size{n}, adjacency_matrix(n,n) {}
    Graph(int n, Matrix &matrix) : size {n}, adjacency_matrix(matrix) {assertm(matrix.getXSize() == matrix.getYSize(), "matrix needs to be square");}
    Graph(int n, Matrix &&matrix) : size {n}, adjacency_matrix(matrix) {assertm(matrix.getXSize() == matrix.getYSize(), "matrix needs to be square");}

    Graph(Graph&) = delete;
    Graph(Graph&&) = delete;

    std::optional<double> &get(int x, int y) {return adjacency_matrix.get(x,y);}

    std::vector<std::pair<int,double>> getNeighbors(int n) {
        std::vector<std::pair<int,double>> neighbors;
        for(int i=0; i<size; i++)
        {
            auto adjacency = adjacency_matrix.get(n,i);
            if(adjacency.has_value())
                neighbors.push_back(std::make_pair(i, adjacency.value()));
        }
        return neighbors;
    };

    void addEdge(int firstNode, int secondNode, double weight) {
        adjacency_matrix.get(firstNode, secondNode) = adjacency_matrix.get(secondNode, firstNode) = weight;
    }

private:
    int size;
    Matrix adjacency_matrix;
};

int main(){
    std::ifstream testMatrix("example_matrix");
    int m,n;

    testMatrix >> m >> n;

    assertm(m==n, "m must be equal to n (square matrix)");

    Matrix matrix(m,n);

    for (int i=0; i<n; i++){
        for (int j=0; j<m; j++){
            double num;
            testMatrix >> num;
            matrix.get(j,i) = (num < 0 ? std::nullopt : std::make_optional(num));
        }
    }

    Graph graph(m, matrix);

    std::set<int> unassignedNodes;
    std::set<int> assignedNodes;
    for(int i = 0; i<m; i++) unassignedNodes.insert(i);

    assignedNodes.insert(unassignedNodes.extract(0));
    int lastInsert = 0;

    std::map<int, std::pair<int,double>> possible_edges; //<node to add, <optimal node from with, min weight>>

    Graph spanning_tree(m);

    while(!unassignedNodes.empty()){
        std::vector<std::pair<int, double>> new_neighbors = graph.getNeighbors(lastInsert);
        for(auto &&new_neighbor : new_neighbors){
            if(assignedNodes.count(new_neighbor.first)) continue;
            auto registered_edge = possible_edges.find(new_neighbor.first);
            if(registered_edge == possible_edges.end() || registered_edge->second.second > new_neighbor.second)
                possible_edges.insert_or_assign(new_neighbor.first,std::make_pair(lastInsert, new_neighbor.second));
        }
        auto minEdge = std::min_element(possible_edges.begin(), possible_edges.end(),
                                        [](decltype(possible_edges)::value_type& l, decltype(possible_edges)::value_type& r){
                                            return l.second.second < r.second.second;
                                        });
        lastInsert = minEdge->first;
        possible_edges.erase(lastInsert);
        assignedNodes.insert(unassignedNodes.extract(lastInsert));
        // std::cout << "I AM HERE!!!" << std::endl;
        auto  des_wei = minEdge->second;//causes valgrind error
        spanning_tree.addEdge(des_wei.first, lastInsert, des_wei.second);
        // std::cout << "YOU ARE THERE???" << std::endl;
        // int a = t.first;
        // double b = t.second;
        // spanning_tree.addEdge(a, lastInsert, b);
        // break;
    }

    for(int i{}; i<m; i++)
    {
        auto neigh = spanning_tree.getNeighbors(i);
        for (auto&& i : neigh)
            std::cout << i.first << "->" << i.second << ";  ";

        std::cout << std::endl;
    }
    return 0;
}