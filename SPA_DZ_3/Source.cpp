#include <vector>
#include <queue>
#include <limits>
#include <thread>
#include <iostream>
#include <ctime>

using namespace std;

const int ROWS = 20;
const int COLS = 40;
const char EMPTY = '.';
const char WALL = '#';
const char START = 'A';
const char END = 'B';
const char PATH = 'o';

struct Node {
	int row, col, dist;
	bool operator>(const Node& other) const {
		return dist > other.dist;
	}
};

bool isValid(int row, int col, const vector<vector<char>>& grid) {
	return row >= 0 && row < ROWS && col >= 0 && col < COLS && grid[row][col] != WALL;
}

vector<pair<int, int>> dijkstra(const vector<vector<char>>& grid, int startRow, int startCol, int endRow, int endCol) {
	vector<vector<int>> dist(ROWS, vector<int>(COLS, numeric_limits<int>::max()));
	vector<vector<pair<int, int>>> prev(ROWS, vector<pair<int, int>>(COLS, { -1, -1 }));
	priority_queue<Node, vector<Node>, greater<Node>> pq;

	dist[startRow][startCol] = 0;
	pq.push({ startRow, startCol, 0 });

	int dRow[] = { -1, 1, 0, 0 };
	int dCol[] = { 0, 0, -1, 1 };

	while (!pq.empty()) {
		int row = pq.top().row;
		int col = pq.top().col;
		int distance = pq.top().dist;
		pq.pop();

		if (row == endRow && col == endCol) break;

		for (int i = 0; i < 4; i++) {
			int newRow = row + dRow[i];
			int newCol = col + dCol[i];

			if (isValid(newRow, newCol, grid) && distance + 1 < dist[newRow][newCol]) {
				dist[newRow][newCol] = distance + 1;
				prev[newRow][newCol] = { row, col };
				pq.push({ newRow, newCol, dist[newRow][newCol] });
			}
		}
	}

	vector<pair<int, int>> path;
	for (pair<int, int> at = { endRow, endCol }; at != make_pair(-1, -1); at = prev[at.first][at.second]) {
		path.push_back(at);
	}
	reverse(path.begin(), path.end());
	return path;
}

char rnd_wall() {
	bool wall_or_empty = rand() % 4;
	if (!wall_or_empty) return WALL;
	else return EMPTY;
}

void input_rnd(vector<vector<char>>& grid) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			grid[i][j] = rnd_wall();
		}
	}
}

void printGrid(const vector<vector<char>>& grid) {
	for (const auto& row : grid) {
		for (const auto& cell : row) {
			cout << cell;
		}
		cout << endl;
	}
}

void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

bool checkBounds(int row, int col) {
	return row >= 1 && row <= ROWS && col >= 1 && col <= COLS;
}

void make_the_wall(vector<vector<char>>& grid) {
	bool y_n_w;
	cout << "Zelis zid? (1 - da, 0 - ne): ";
	cin >> y_n_w;
	if (y_n_w) {
		bool custom_or_random;
		cout << "Zelis svoj odabir ili nasumicno (0 - svoj odabir, 1 - nasumicno): ";
		cin >> custom_or_random;
		if (!custom_or_random) {
			int a1, a2, b1, b2;
			cout << "Unesi koordinate a (redak stupac): ";
			cin >> a1 >> a2;
			cout << "Unesi koordinate b (redak stupac): ";
			cin >> b1 >> b2;

			if (!checkBounds(a1, a2) || !checkBounds(b1, b2)) {
				cout << "Pogresan unos. Unesi vrijednosti unutar granica." << endl;
				return;
			}

			vector<pair<int, int>> wall = dijkstra(grid, a1 - 1, a2 - 1, b1 - 1, b2 - 1);

			for (int i = 1; i < wall.size() - 1; i++) {
				grid[wall[i].first][wall[i].second] = WALL;
			}
		}
		else {
			input_rnd(grid);
		}
	}
}

void get_the_path(vector<pair<int, int>>& path, vector<vector<char>>& grid) {
	for (int i = 1; i < path.size() - 1; i++) {
		grid[path[i].first][path[i].second] = PATH;
		if (i != 1) grid[path[i - 1].first][path[i - 1].second] = EMPTY;
		clearScreen();
		printGrid(grid);
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

int main() {
	srand(time(nullptr));
	int start_row, start_col, end_row, end_col;
	cout << "Unesite redak i stupac pocetne tocke A (1-20, 1-40): ";
	cin >> start_row >> start_col;
	cout << "Unesite redak i stupac krajnje točke B (1-20, 1-40): ";
	cin >> end_row >> end_col;

	if (!checkBounds(start_row, start_col) || !checkBounds(end_row, end_col)) {
		cout << "Pogresan unos. Unesi vrijednosti unutar granica." << endl;
		return 1;
	}

	vector<vector<char>> grid(ROWS, vector<char>(COLS, EMPTY));


	// Postavljanje A i B
	grid[start_row - 1][start_col - 1] = START;
	grid[end_row - 1][end_col - 1] = END;

	// Dodavanje zida
	make_the_wall(grid);

	vector<pair<int, int>> path = dijkstra(grid, start_row - 1, start_col - 1, end_row - 1, end_col - 1);

	if (path.size() < 2) { 
		cout << "Nema puta od A do B." << endl;
		printGrid(grid);
		return 1;
	}// provjera jel postoji put

	get_the_path(path, grid); // ispisi put

	grid[end_row - 1][end_col - 1] = PATH;
	grid[path[path.size() - 2].first][path[path.size() - 2].second] = EMPTY;

	clearScreen();
	printGrid(grid);
	cout << "Put naden!!!" << endl;
	return 0;
}
