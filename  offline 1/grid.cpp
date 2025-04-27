#include <bits/stdc++.h>
using namespace std;

class Grid
{
public:
    int gridSize, costValue, heuristicFunctionSelector;
    float heuristicValue, priorityValue;
    vector<int> numbers;
    Grid *parent; // raw pointer

    Grid(int gridSize, vector<int> numbers, int heuristicFunctionSelector, int costValue = 0, Grid *parent = nullptr)
        : gridSize(gridSize), numbers(numbers), costValue(costValue), parent(parent), heuristicFunctionSelector(heuristicFunctionSelector)
    {
        heuristicValue = heuristicFunction(heuristicFunctionSelector); // Manhattan heuristic
        priorityValue = costValue + heuristicValue;
    }


    int hammingDistance()
    {
        // cout << "hamming" << endl;
        int distance = 0;
        for (int i = 0; i < numbers.size(); i++)
        {
            if (numbers[i] == 0)
                continue;
            if (numbers[i] != i + 1)
                distance++;
        }
        return distance;
    }

    int manhattanDistance()
    {
        // cout << "man" << endl;
        int distance = 0;
        for (int i = 0; i < numbers.size(); i++)
        {
            if (numbers[i] == 0)
                continue;
            int currentRow = i / gridSize;
            int currentCol = i % gridSize;
            int target = numbers[i] - 1;
            int targetRow = target / gridSize;
            int targetCol = target % gridSize;
            distance += abs(currentRow - targetRow) + abs(currentCol - targetCol);
        }
        return distance;
    }

    float euclideanDistance()
    {
        // cout << "eucli" << endl;
        float distance = 0.0f;
        for (int i = 0; i < numbers.size(); i++)
        {
            if (numbers[i] == 0)
                continue;
            int currentRow = i / gridSize;
            int currentCol = i % gridSize;
            int target = numbers[i] - 1;
            int targetRow = target / gridSize;
            int targetCol = target % gridSize;
            distance += sqrt((currentRow - targetRow) * (currentRow - targetRow) +
                             (currentCol - targetCol) * (currentCol - targetCol));
        }
        return distance;
    }

    float linearConflict()
    {
        // cout << "linear" << endl;
        int conflictCount = 0;
        int n = gridSize;

        // Row conflicts
        for (int row = 0; row < n; row++)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = i + 1; j < n; j++)
                {
                    int index1 = row * n + i;
                    int index2 = row * n + j;
                    int tile1 = numbers[index1];
                    int tile2 = numbers[index2];
                    if (tile1 == 0 || tile2 == 0)
                        continue;

                    int goalRow1 = (tile1 - 1) / n;
                    int goalCol1 = (tile1 - 1) % n;
                    int goalRow2 = (tile2 - 1) / n;
                    int goalCol2 = (tile2 - 1) % n;

                    if (goalRow1 == row && goalRow2 == row && goalCol1 > goalCol2)
                    {
                        conflictCount++;
                    }
                }
            }
        }

        // Column conflicts
        for (int col = 0; col < n; col++)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = i + 1; j < n; j++)
                {
                    int index1 = i * n + col;
                    int index2 = j * n + col;
                    int tile1 = numbers[index1];
                    int tile2 = numbers[index2];
                    if (tile1 == 0 || tile2 == 0)
                        continue;

                    int goalRow1 = (tile1 - 1) / n;
                    int goalCol1 = (tile1 - 1) % n;
                    int goalRow2 = (tile2 - 1) / n;
                    int goalCol2 = (tile2 - 1) % n;

                    if (goalCol1 == col && goalCol2 == col && goalRow1 > goalRow2)
                    {
                        conflictCount++;
                    }
                }
            }
        }

        return manhattanDistance() + 2 * conflictCount;
    }

    float heuristicFunction(int type)
    {
        if (type == 0)
            return hammingDistance();
        else if (type == 1)
            return manhattanDistance();
        else if (type == 2)
            return euclideanDistance();
        else
            return manhattanDistance();
    }

    int findBlankIndex()
    {
        for (int i = 0; i < numbers.size(); i++)
        {
            if (numbers[i] == 0)
                return i;
        }
        return -1;
    }

    void swap(int i, int j)
    {
        std::swap(numbers[i], numbers[j]);
    }

    bool canMoveLeft()
    {
        return findBlankIndex() % gridSize != 0;
    }

    bool canMoveRight()
    {
        return findBlankIndex() % gridSize != gridSize - 1;
    }

    bool canMoveUp()
    {
        return findBlankIndex() / gridSize != 0;
    }

    bool canMoveDown()
    {
        return findBlankIndex() / gridSize != gridSize - 1;
    }

    Grid moveLeft()
    {
        Grid next = *this;
        int idx = findBlankIndex();
        next.swap(idx, idx - 1);
        next.costValue++;
        next.heuristicValue = next.heuristicFunction(heuristicFunctionSelector);
        next.priorityValue = next.costValue + next.heuristicValue;
        next.parent = new Grid(*this); // allocate
        return next;
    }

    Grid moveRight()
    {
        Grid next = *this;
        int idx = findBlankIndex();
        next.swap(idx, idx + 1);
        next.costValue++;
        next.heuristicValue = next.heuristicFunction(heuristicFunctionSelector);
        next.priorityValue = next.costValue + next.heuristicValue;
        next.parent = new Grid(*this);
        return next;
    }

    Grid moveUp()
    {
        Grid next = *this;
        int idx = findBlankIndex();
        next.swap(idx, idx - gridSize);
        next.costValue++;
        next.heuristicValue = next.heuristicFunction(heuristicFunctionSelector);
        next.priorityValue = next.costValue + next.heuristicValue;
        next.parent = new Grid(*this);
        return next;
    }

    Grid moveDown()
    {
        Grid next = *this;
        int idx = findBlankIndex();
        next.swap(idx, idx + gridSize);
        next.costValue++;
        next.heuristicValue = next.heuristicFunction(heuristicFunctionSelector);
        next.priorityValue = next.costValue + next.heuristicValue;
        next.parent = new Grid(*this);
        return next;
    }

    vector<Grid> getAllMoves()
    {
        vector<Grid> moves;
        if (canMoveDown())
            moves.push_back(moveDown());
        if (canMoveLeft())
            moves.push_back(moveLeft());
        if (canMoveRight())
            moves.push_back(moveRight());
        if (canMoveUp())
            moves.push_back(moveUp());
        return moves;
    }

    bool isSolved()
    {
        for (int i = 0; i < numbers.size() - 1; i++)
        {
            if (numbers[i] != i + 1)
                return false;
        }
        return numbers.back() == 0;
    }

    void print() const
    {
        for (int i = 0; i < gridSize; i++)
        {
            for (int j = 0; j < gridSize; j++)
            {
                cout << numbers[i * gridSize + j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    bool operator==(const Grid &other) const
    {
        return numbers == other.numbers;
    }

    bool operator<(const Grid &other) const
    {
        return priorityValue > other.priorityValue; // for min-heap
    }
};

class NPuzzle
{
public:
    Grid initial;
    priority_queue<Grid> openList;
    set<vector<int>> visited;
    int heuristicFunctionSelector;

    NPuzzle(int gridSize, vector<int> numbers, int heuristicFunctionSelector = 0) : initial(gridSize, numbers, heuristicFunctionSelector) {}

    bool isSolvable()
    {
        int invCount = 0;
        int n = initial.numbers.size();

        // Count inversions
        for (int i = 0; i < n; i++)
        {
            if (initial.numbers[i] == 0)
                continue; // skip blank
            for (int j = i + 1; j < n; j++)
            {
                if (initial.numbers[j] == 0)
                    continue; // skip blank
                if (initial.numbers[i] > initial.numbers[j])
                    invCount++;
            }
        }

        if (initial.gridSize % 2 == 1)
        {
            // Odd grid: solvable if inversion count is even
            return invCount % 2 == 0;
        }
        else
        {
            // Even grid: need to consider blank's row from bottom
            int blankIndex = initial.findBlankIndex();
            int blankRowFromBottom = initial.gridSize - (blankIndex / initial.gridSize);

            if (blankRowFromBottom % 2 == 0)
            {
                // Blank on even row (from bottom), inversions must be odd
                return invCount % 2 == 1;
            }
            else
            {
                // Blank on odd row (from bottom), inversions must be even
                return invCount % 2 == 0;
            }
        }
    }

    void solve()
    {

        if (!isSolvable())
        {
            cout << "Unsolvable puzzle" << endl;
        }
        else
        {

            int exploredCount = 0;
            int expandedCount = 0;

            openList.push(initial);

            while (!openList.empty())
            {
                Grid current = openList.top();
                openList.pop();
                exploredCount++;

                if (visited.count(current.numbers))
                {
                    delete current.parent;
                    continue;
                }
                visited.insert(current.numbers);

                if (current.isSolved())
                {
                    cout << "Minimum number of moves = " << current.costValue << endl;
                    cout << "Nodes explored: " << exploredCount << endl;
                    cout << "Nodes expanded: " << expandedCount << endl;

                    // Collect steps
                    vector<Grid> steps;
                    vector<Grid *> toDelete;
                    Grid *ptr = new Grid(current); // Deep copy current first
                    while (ptr)
                    {
                        steps.push_back(*ptr);
                        if (ptr->parent)
                            toDelete.push_back(ptr);
                        ptr = ptr->parent;
                    }
                    reverse(steps.begin(), steps.end());

                    // Print steps
                    for (int i = 0; i < steps.size(); ++i)
                    {
                        cout << "Step " << i << ":\n";
                        steps[i].print();
                    }

                    // Cleanup heap memory
                    for (Grid *g : toDelete)
                    {
                        delete g;
                    }

                    return;
                }

                for (Grid neighbor : current.getAllMoves())
                {
                    if (!visited.count(neighbor.numbers))
                    {
                        openList.push(neighbor);
                    }
                    else
                    {
                        delete neighbor.parent;
                    }
                }
                expandedCount++;
            }

            cout << "No solution found.\n";
            cout << "Nodes explored: " << exploredCount << endl;
            cout << "Nodes expanded: " << expandedCount << endl;
        }
    }
};

int main()
{
    int gridSize,temp,heuristicFunctionSelector;
    vector<int> num;
    cout<<"enter the number of grid size: ";
    cin>>gridSize;
    cout<<"choose you heuristic function:"<<endl;
    cout<<"0. Hamming Distance"<<endl;
    cout<<"1. Manhattan Dsitance"<<endl;
    cout<<"2. Euclidean Distance"<<endl;
    cout<<"3. Linear Conflict"<<endl;
    cout<<"Enter you choice:";
    cin>>heuristicFunctionSelector;
    cout<<"Enter the inital conditon of board:"<<endl;
    for(int i=0;i<gridSize;i++){
        for(int j=0;j<gridSize;j++){
            cin>>temp;
            num.push_back(temp);
        }
    }

    NPuzzle npuzzle(gridSize, num, 0);
    npuzzle.solve();

    return 0;
}
