#include <bits/stdc++.h>
using namespace std;

class Grid {
public:
    int gridSize, costValue;
    float heuristicValue, priorityValue;
    vector<int> numbers;
    shared_ptr<Grid> parent;

    Grid(int gridSize, vector<int> numbers, int costValue = 0, shared_ptr<Grid> parent = nullptr)
        : gridSize(gridSize), numbers(numbers), costValue(costValue), parent(parent) {
        heuristicValue = heuristicFunction(1); // Manhattan heuristic
        priorityValue = costValue + heuristicValue;
    }

    int hammingDistance() {
        int distance = 0;
        for (int i = 0; i < numbers.size(); i++) {
            if (numbers[i] == 0) continue;
            if (numbers[i] != i + 1) distance++;
        }
        return distance;
    }

    int manhattanDistance() {
        int distance = 0;
        for (int i = 0; i < numbers.size(); i++) {
            if (numbers[i] == 0) continue;
            int currentRow = i / gridSize;
            int currentCol = i % gridSize;
            int target = numbers[i] - 1;
            int targetRow = target / gridSize;
            int targetCol = target % gridSize;
            distance += abs(currentRow - targetRow) + abs(currentCol - targetCol);
        }
        return distance;
    }

    float heuristicFunction(int type) {
        if (type == 0) return hammingDistance();
        else return manhattanDistance();
    }

    int findBlankIndex() {
        for (int i = 0; i < numbers.size(); i++) {
            if (numbers[i] == 0) return i;
        }
        return -1;
    }

    void swap(int i, int j) {
        std::swap(numbers[i], numbers[j]);
    }

    bool canMoveLeft() {
        return findBlankIndex() % gridSize != 0;
    }

    bool canMoveRight() {
        return findBlankIndex() % gridSize != gridSize - 1;
    }

    bool canMoveUp() {
        return findBlankIndex() / gridSize != 0;
    }

    bool canMoveDown() {
        return findBlankIndex() / gridSize != gridSize - 1;
    }

    Grid moveLeft() {
        Grid next = *this;
        int idx = findBlankIndex();
        next.swap(idx, idx - 1);
        next.costValue++;
        next.heuristicValue = next.heuristicFunction(1);
        next.priorityValue = next.costValue + next.heuristicValue;
        next.parent = make_shared<Grid>(*this);
        return next;
    }

    Grid moveRight() {
        Grid next = *this;
        int idx = findBlankIndex();
        next.swap(idx, idx + 1);
        next.costValue++;
        next.heuristicValue = next.heuristicFunction(1);
        next.priorityValue = next.costValue + next.heuristicValue;
        next.parent = make_shared<Grid>(*this);
        return next;
    }

    Grid moveUp() {
        Grid next = *this;
        int idx = findBlankIndex();
        next.swap(idx, idx - gridSize);
        next.costValue++;
        next.heuristicValue = next.heuristicFunction(1);
        next.priorityValue = next.costValue + next.heuristicValue;
        next.parent = make_shared<Grid>(*this);
        return next;
    }

    Grid moveDown() {
        Grid next = *this;
        int idx = findBlankIndex();
        next.swap(idx, idx + gridSize);
        next.costValue++;
        next.heuristicValue = next.heuristicFunction(1);
        next.priorityValue = next.costValue + next.heuristicValue;
        next.parent = make_shared<Grid>(*this);
        return next;
    }

    vector<Grid> getAllMoves() {
        vector<Grid> moves;
        if (canMoveDown()) moves.push_back(moveDown());
        if (canMoveLeft()) moves.push_back(moveLeft());
        if (canMoveRight()) moves.push_back(moveRight());
        if (canMoveUp()) moves.push_back(moveUp());
        return moves;
    }

    bool isSolved() {
        for (int i = 0; i < numbers.size() - 1; i++) {
            if (numbers[i] != i + 1) return false;
        }
        return numbers.back() == 0;
    }

    void print() const {
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                cout << numbers[i * gridSize + j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    bool operator==(const Grid& other) const {
        return numbers == other.numbers;
    }

    bool operator<(const Grid& other) const {
        return priorityValue > other.priorityValue; // for min-heap
    }
};

class NPuzzle {
public:
    Grid initial;
    priority_queue<Grid> openList;
    set<vector<int>> visited;

    NPuzzle(int gridSize, vector<int> numbers) : initial(gridSize, numbers) {}

    void solve() {
        int exploredCount = 0;
        int expandedCount = 0;
    
        openList.push(initial);
    
        while (!openList.empty()) {
            Grid current = openList.top();
            openList.pop();
            exploredCount++; // Every time we pop, we explore a node
    
            if (visited.count(current.numbers)) continue;
            visited.insert(current.numbers);
    
            if (current.isSolved()) {
                cout << "Puzzle solved in " << current.costValue << " moves.\n";
                cout << "Nodes explored: " << exploredCount << endl;
                cout << "Nodes expanded: " << expandedCount << endl;
    
                // Backtrack and collect steps
                vector<Grid> steps;
                shared_ptr<Grid> ptr = make_shared<Grid>(current);
                while (ptr) {
                    steps.push_back(*ptr);
                    ptr = ptr->parent;
                }
                reverse(steps.begin(), steps.end());
    
                // Print all steps
                for (int i = 0; i < steps.size(); ++i) {
                    cout << "Step " << i << ":\n";
                    steps[i].print();
                }
    
                return;
            }
    
            for (Grid neighbor : current.getAllMoves()) {
                if (!visited.count(neighbor.numbers)) {
                    openList.push(neighbor);
                }
            }
            expandedCount++; // After generating neighbors, we consider current node expanded
        }
    
        cout << "No solution found.\n";
        cout << "Nodes explored: " << exploredCount << endl;
        cout << "Nodes expanded: " << expandedCount << endl;
    }
    
};

int main() {
    vector<int> num = {
        1, 3, 4,
        0, 2, 6,
        7, 5, 8
    };

    NPuzzle npuzzle(3, num);
    npuzzle.solve();

    return 0;
}
