#include <iostream>
#include <vector>
#include <map>

class MatrixGraph;

class IGraph {

public:

    virtual ~IGraph() {}

    IGraph() {}; // нужно реализовать в наследниках тоже

    IGraph(const IGraph &_oth) {}; // нужно реализовать в наследниках тоже

    virtual void AddEdge(int from, int to) = 0; // Метод принимает вершины начала и конца ребра и добавляет ребро

    virtual int VerticesCount() const = 0; // Метод должен считать текущее количество вершин

    virtual std::vector<int> Vertices() const = 0; // Метод должен вернуть текущие список текущих вершин

    virtual std::vector<int> GetNextVertices(
            int vertex) const = 0; // Для конкретной вершины метод возвращает вектор с вершинами, в которые можно дойти по ребру из данной

    virtual std::vector<int> GetPrevVertices(
            int vertex) const = 0; // Для конкретной вершины метод возвращает вектор с вершинами, из которых можно дойти по ребру в данную
};

class ListGraph : public IGraph {
    std::vector<int> _index2Vertex; // все вершины
    std::map<int, int> _vertex2Index; //вершина и индекс в _index2Vertex

    std::map<int, std::vector<int>> _nextVert;
    std::map<int, std::vector<int>> _prevVert;

    void fillVartex(int vartex){
    if (auto pos = _vertex2Index.find(vartex); pos == _vertex2Index.end()) {
        const int res = _index2Vertex.size();
        _index2Vertex.push_back(vartex);
        _vertex2Index[vartex] = res;
    }
}
public:
    ListGraph(){}
    void AddEdge(int from, int to) override {
        bool add = true;
        if (auto pos = _nextVert.find(from); pos != _nextVert.end()) {
            for (int i = 0; i < pos->second.size(); ++i) {
                if (pos->second[i] == to){
                    add = false;
                }
            }
        }

        if(add){
            fillVartex(from);
            fillVartex(to);
            _nextVert[from].push_back(to);
            _prevVert[to].push_back(from);
        }
    }

    ListGraph(const IGraph &_oth){
        std::vector<int> index2Vertex = _oth.Vertices();
        for (int i = 0; i < index2Vertex.size(); ++i) {
            std::vector<int> nextVertices = _oth.GetNextVertices(index2Vertex[i]);
            for (int j = 0; j < nextVertices.size(); ++j) {
                AddEdge(index2Vertex[i], nextVertices[j]);
            }
        }
    }

    ListGraph(const ListGraph &_oth){
        _index2Vertex = _oth._index2Vertex;
        _vertex2Index = _oth._vertex2Index;
        _nextVert = _oth._nextVert;
        _prevVert = _oth._prevVert;
    }

    ListGraph& operator=(const ListGraph &_oth){
        if (this == &_oth) return * this;
        _index2Vertex = _oth._index2Vertex;
        _vertex2Index = _oth._vertex2Index;
        _nextVert = _oth._nextVert;
        _prevVert = _oth._prevVert;
        return *this;
    }

    IGraph& operator=(const IGraph &_oth){
       std::cout << "=IGraph=" << std::endl;
        if (this == &_oth) return * this;
        std::vector<int> index2Vertex = _oth.Vertices();
        for (int i = 0; i < index2Vertex.size(); ++i) {
            std::vector<int> nextVertices = _oth.GetNextVertices(index2Vertex[i]);
            for (int j = 0; j < nextVertices.size(); ++j) {
                AddEdge(index2Vertex[i], nextVertices[j]);
            }
        }
        return *this;
    }

    virtual int VerticesCount() const {
        return _index2Vertex.size();
    };

    virtual std::vector<int> Vertices() const {
        return _index2Vertex;
    };

    virtual std::vector<int> GetNextVertices(int vertex) const {
        std::vector<int> nextVert;
        if (auto pos = _nextVert.find(vertex); pos != _nextVert.end()) {
            for (int i = 0; i < pos->second.size(); ++i) {
                nextVert.push_back(pos->second[i]);
            }
        }
        return nextVert;
    };

    virtual std::vector<int> GetPrevVertices(int vertex) const {
        std::vector<int> prevVert;
        if (auto pos = _prevVert.find(vertex); pos != _prevVert.end()) {
            for (int i = 0; i < pos->second.size(); ++i) {
                prevVert.push_back(pos->second[i]);
            }
        }
        return prevVert;
    };
};

class MatrixGraph : public IGraph {
    std::vector<int> _index2Vertex; // все вершины
    std::map<int, int> _vertex2Index; //вершина и индекс в _index2Vertex
    std::vector<std::vector<int>> _matrix;

    int _getIndex(int vertex) {
        if (auto pos = _vertex2Index.find(vertex); pos != _vertex2Index.end()) {
            return pos->second;
        }

        const int res = _index2Vertex.size();
        _index2Vertex.push_back(vertex);
        _vertex2Index[vertex] = res;

        for (auto &row: _matrix) {
            row.push_back(0);
        }
        _matrix.emplace_back(res + 1, 0);
        return res;
    }

public:
MatrixGraph(){}
    void AddEdge(int from, int to) override {
        auto from_idx = _getIndex(from);
        auto to_idx = _getIndex(to);
        _matrix[from_idx][to_idx] = 1;
    }

    MatrixGraph(const IGraph &_oth){
        std::cout << "MatrixGraph(const IGraph &_oth)"<< std::endl;
        std::vector<int> index2Vertex = _oth.Vertices();
        for (int i = 0; i < index2Vertex.size(); ++i) {
            std::vector<int> nextVertices = _oth.GetNextVertices(index2Vertex[i]);
            for (int j = 0; j < nextVertices.size(); ++j) {
                AddEdge(index2Vertex[i], nextVertices[j]);
            }
        }
    }

    MatrixGraph(const MatrixGraph &_oth){
        std::cout << "MatrixGraph(const MatrixGraph &_oth)"<< std::endl;
        _index2Vertex = _oth._index2Vertex;
        _vertex2Index = _oth._vertex2Index;
        _matrix = _oth._matrix;
    }
    MatrixGraph& operator=(const MatrixGraph &_oth){
        if (this == &_oth) return * this;
        std::cout << "MatrixGraph& operator=(const MatrixGraph &_oth)"<< std::endl;
        _index2Vertex = _oth._index2Vertex;
        _vertex2Index = _oth._vertex2Index;
        _matrix = _oth._matrix;
        return *this;
    }

    IGraph& operator=(const IGraph &_oth){
        std::cout << "=IGraph=" << std::endl;
        if (this == &_oth) return * this;
        std::vector<int> index2Vertex = _oth.Vertices();
        for (int i = 0; i < index2Vertex.size(); ++i) {
            std::vector<int> nextVertices = _oth.GetNextVertices(index2Vertex[i]);
            for (int j = 0; j < nextVertices.size(); ++j) {
                AddEdge(index2Vertex[i], nextVertices[j]);
            }
        }
        return *this;
    }

    virtual int VerticesCount() const {
        return _index2Vertex.size();
    };

    virtual std::vector<int> Vertices() const {
        return _index2Vertex;
    };

    virtual std::vector<int> GetNextVertices(int vertex) const {
        int row;
        if (auto pos = _vertex2Index.find(vertex); pos != _vertex2Index.end()) {
            row =  pos->second;
        }
        std::vector<int> nextVert;
        for (int i = 0; i < _index2Vertex.size(); ++i) {
            if (_matrix[row][i]){
                nextVert.push_back(_index2Vertex[i]);
            }
        }
        return nextVert;
    };

    virtual std::vector<int> GetPrevVertices(int vertex) const {
        int col;
        if (auto pos = _vertex2Index.find(vertex); pos != _vertex2Index.end()) {
            col =  pos->second;
        }
        std::vector<int> prevVert;
        for (int i = 0; i < _index2Vertex.size(); ++i) {
            if (_matrix[i][col]){
                prevVert.push_back(_index2Vertex[i]);
            }
        }
        return prevVert;
    };

};

int main() {
    MatrixGraph g1;

    g1.AddEdge(1, 2);
    g1.AddEdge(1, 4);
    g1.AddEdge(1, 2);
    g1.AddEdge(100, 100);
    g1.AddEdge(100, 100);
    g1.AddEdge(-1, 1);


    ListGraph g2 = g1;

    auto v = g2.Vertices();

    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " -> ";

        auto n = g2.GetNextVertices(v[i]);
        for (int j = 0; j < n.size(); ++j) {
            std::cout << n[j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " <- ";

        auto n = g2.GetPrevVertices(v[i]);
        for (int j = 0; j < n.size(); ++j) {
            std::cout << n[j] << " ";
        }
        std::cout << std::endl;
    }
}
