#include "../helpers.h"
#include "../common.h"

using namespace std;

struct Point {
    Point() = default;
    Point(int _x, int _y) {
        x = _x;
        y = _y;
    }

    int x = 0, y = 0;

    Point operator-(const Point& other) {
        return Point(x - other.x, y - other.y);
    }
};

class Vent {
public:
    Vent(ifstream& stream) {
        string token;
        getline(stream, token, ',');

        start.x = atoi(token.c_str());
        getline(stream, token, ' ');
        start.y = atoi(token.c_str());

        getline(stream, token, ' ');
        
        getline(stream, token, ',');
        end.x = atoi(token.c_str());
        getline(stream, token);
        end.y = atoi(token.c_str());

        maxX = max(start.x, end.x);
        maxY = max(start.y, end.y);

        if (start.x == end.x) {
            if (start.y > end.y)
                swap(start, end);
        } else {
            if (start.x > end.x)
                swap(start, end);
        }
    }

    int getMaxX() const { return maxX; }
    int getMaxY() const { return maxY; }
    Point getStartPoint() const { return start; }
    Point getEndPoint() const { return end; }

private:
    int maxX = 0;
    int maxY = 0;
    Point start;
    Point end;
};

class VentGrid {
public:
    VentGrid(const vector<Vent>& vents) {
        for (const auto& vent : vents) {
            gridSizeX = max(vent.getMaxX(), gridSizeX);
            gridSizeY = max(vent.getMaxY(), gridSizeY);
        }

        gridSizeX++;
        gridSizeY++;

        createGrid();

        for (const auto& vent : vents) {
            addVentToGrid(vent);
        }
    }

    void addVentToGrid(const Vent& vent) {
        Point start = vent.getStartPoint();
        Point difference = vent.getEndPoint() - start;

        bool vertical = difference.x == 0;
            
        int x = 0;
        int y = 0;
        for (;;) {
            grid[start.x + x][start.y + y] += 1;

            if (x == difference.x && y == difference.y)
                break;

            if (y < abs(difference.y))
                if (difference.y > 0)
                    y++;
                else 
                    y--;

            if (x < difference.x)
                x++;
        }
    }

    int getPoints() {
        int pts = 0;
        for (int i = 0; i < gridSizeX; i++) {
            for (int j = 0; j < gridSizeY; j++) {
                if (grid[j][i] > 1)
                    pts++;
            }
        }
        return pts;
    }

    ~VentGrid() {
        for (int i = 0; i < gridSizeY; i++) {
            delete [] grid[i];
        }
        delete [] grid;
    }
private:
    int gridSizeX = 0;
    int gridSizeY = 0;
    int** grid;

    void createGrid() {
        grid = new int*[gridSizeY];
        for (int i = 0; i < gridSizeY; i++) {
            grid[i] = new int[gridSizeX] { 0 };
        }
    }

    void printGrid() {
        for (int i = 0; i < gridSizeX; i++) {
            for (int j = 0; j < gridSizeY; j++) {
                cout << grid[j][i];
            }
            cout << endl;
        }
    }
};

int main() {
    FileParser fp("2021/5-data");

    vector<Vent> vents = fp.parseRest<Vent>();

    VentGrid grid(vents);

    cout << grid.getPoints() << endl;
}