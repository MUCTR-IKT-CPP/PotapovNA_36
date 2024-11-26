#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <stdexcept> 
#include <windows.h> 

using namespace std;

const int BSIZE = 10; // ������ �������� ����
const char EMPTY = ' '; // ������ ������
const char SHIP = 'S'; // �������
const char HIT = 'X';  // ���������
const char MISS = '*'; // ������

class InvalidShipPlacementException : public exception {
public:
    const char* what() const noexcept override {
        return "Incorrect input / Wrong ship placement!";
    }
};

class Ship {
public:
    int size;
    Ship(int s) : size(s) {}
};

class GameField {
private:
    vector<vector<char>> board;
    vector<vector<char>> hits; // ���� ��� ����������� ����

public:
    GameField() {
        board.resize(BSIZE, vector<char>(BSIZE, EMPTY));
        hits.resize(BSIZE, vector<char>(BSIZE, EMPTY));
    }

    /**
    * ����� ��� ����������� �������� ����.
    * @param revealShips ���������, ������ �� ������� ���� ��������.
    */
    void displayBoard(bool revealShips = false) const {
        // ��������� ����� ������ �� ��������� (�����)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "  ";
        for (int j = 0; j <BSIZE; j++) {
            cout << "|" << j;
        }
        cout << "|" << endl;

        cout << "  ";
        for (int j = 0; j <BSIZE; j++) {
            cout << "--";
        }
        cout << "-" << endl;

        for (int i = 0; i <BSIZE; i++) {
            cout << i << " ";
            for (int j = 0; j <BSIZE; j++) {
                if (revealShips) {
                    if (hits[i][j] == HIT || hits[i][j] == MISS) {
                        cout << "|";
                        // ������������� ���� ������� ��� ���������
                        if (hits[i][j] == HIT) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // �������
                        }
                        cout << hits[i][j];
                        // ���������� ���� ������ �� ���������
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // �����
                    }
                    else {
                        cout << "|" << (board[i][j] == EMPTY ? EMPTY : board[i][j]);
                    }
                }
                else {
                    cout << "|" << (hits[i][j] == EMPTY ? EMPTY : hits[i][j]);
                }
            }
            cout << "|" << endl;

            cout << "  ";
            for (int j = 0; j <BSIZE; j++) {
                cout << "--";
            }
            cout << "-" << endl;
        }
    }

    /**
    * ���������, ��������� �� ������� ����� � �������� ��������.
    * @param x X-���������� �������.
    * @param y Y-���������� �������.
    * @param shipSize ������ �������.
    * @param horizontal ����������� ���������� (������������� ��� �����������).
    * @return true, ���� ����� � �������� ��������� ������ �������.
    */
    bool isAdjacent(int x, int y, int shipSize, bool horizontal) const {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= shipSize; dy++) {
                int nx = horizontal ? x + dx : x + dy;
                int ny = horizontal ? y + dy : y + dx;
                if (nx >= 0 && nx < BSIZE && ny >= 0 && ny < BSIZE && board[nx][ny] == SHIP) {
                    return true;
                }
            }
        }
        return false;
    }

    bool canPlaceShip(int x, int y, int shipSize, bool horizontal) const {
        if (horizontal) {
            if (y + shipSize > BSIZE) return false;
        }
        else {
            if (x + shipSize > BSIZE) return false;
        }

        for (int j = 0; j < shipSize; j++) {
            int nx = horizontal ? x : x + j;
            int ny = horizontal ? y + j : y;

            if (board[nx][ny] != EMPTY) return false;
            if (isAdjacent(nx, ny, shipSize, horizontal)) return false;
        }
        return true;
    }

    /**
    * ��������� ������� �� ����.
    * @param x X-���������� ������ ����������.
    * @param y Y-���������� ������ ����������.
    * @param shipSize ������ �������.
    * @param horizontal ����������� ���������� (true - �������������, false - �����������).
    */
    void placeShip(int x, int y, int shipSize, bool horizontal) {
        if (horizontal) {
            for (int j = 0; j < shipSize; j++) {
                board[x][y + j] = SHIP;
            }
        }
        else {
            for (int j = 0; j < shipSize; j++) {
                board[x + j][y] = SHIP;
            }
        }
    }

    /**
    * ���������� ������� ��������� ������� �� ����.
    * @param N �������� �� �������� ������� ���-�� ��������������� ��������
    */
    void generateShips(int N) {
        int counts[4] = { (N * 40) / 100, (N * 30) / 100, (N * 20) / 100, (N * 10) / 100 };
        vector<Ship> ships = { Ship(1), Ship(2), Ship(3), Ship(4) };
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < counts[i]; j++) {
                bool placed = false;
                while (!placed) {
                    int orientation = rand() % 2; // 0 - �������������, 1 - �����������
                    int x = rand() % BSIZE;
                    int y = rand() % BSIZE;

                    if (canPlaceShip(x, y, ships[i].size, orientation == 0)) {
                        placeShip(x, y, ships[i].size, orientation == 0);
                        placed = true;
                    }
                }
            }
        }
    }

    /**
    * ������������ ����� �� �������� �����������.
    * @param y Y-���������� �����.
    * @param x X-���������� �����.
    * @return true, ���� ��������� ������� (���������), � false, ���� ������.
    * @throw std::invalid_argument ���� ���������� ������� �� ������� ���� ��� ������ ��� ���������.
    */
    bool attack(int y, int x) {
        if (x < 0 || x >= BSIZE || y < 0 || y >= BSIZE) 
            throw invalid_argument("Out of field."); // �������� ������ �� �������
        if (hits[x][y] != EMPTY) 
            throw invalid_argument("This place were atackted already."); // ���� ��� ��������� ��� ������

        if (board[x][y] == SHIP) {
            hits[x][y] = HIT; // �������� ���������
            cout << "Hit!" << endl;
            return true;
        }
        else {
            hits[x][y] = MISS; // �������� ������
            cout << "Miss!" << endl;
            return false;
        }
    }

    /**
    * ������������ ���������� ����� ������ �������� �� ������� ����.
    * @return ���������� ����� ������ ��������.
    */
    int countLiveSpots() const {
        int count = 0;
        for (int i = 0; i < BSIZE; i++) {
            for (int j = 0; j < BSIZE; j++) {
                if (board[i][j] == SHIP) {
                    count++;
                }
            }
        }
        return count;
    }

    /**
    * ���������, ���� �� ����� ������� �� ����.
    * @return true, ���� ���� ����� ����� ��������; ����� false.
    */
    bool hasShips() const {
        return countLiveSpots() > 0;
    }

    /**
    * ������������ ���������� ��������� �� ��������.
    * @return ���������� ��������� �� ��������.
    */
    int countHits() const {
        int hitsCount = 0;
        for (int i = 0; i < BSIZE; i++) {
            for (int j = 0; j < BSIZE; j++) {
                if (hits[i][j] == HIT) {
                    hitsCount++;
                }
            }
        }
        return hitsCount;
    }

    /**
    *������������ ���������� ����������� ��������.
    *@return ���������� ����������� ��������.
    */
    int countSunkShips() const {
        int sunkShips = 0;
        vector<vector<bool>> checked(BSIZE, vector<bool>(BSIZE, false)); // ������� ����� ����������� ������

        for (int x = 0; x < BSIZE; x++) {
            for (int y = 0; y < BSIZE; y++) {
                // ���� ����� �������, ������� ��� �� ���������
                if (board[x][y] == SHIP && !checked[x][y]) {
                    // �������� �� �������������� ��� ������������ ������������
                    bool isSunk = true;

                    // ��������� �������������
                    for (int dy = 0; dy < BSIZE && y + dy < BSIZE && board[x][y + dy] == SHIP; dy++) {
                        checked[x][y + dy] = true; // �������� ������ ��� �����������
                        if (hits[x][y + dy] != HIT) { // �������� �� ���������
                            isSunk = false;
                        }
                    }

                    // ��������� �����������
                    for (int dx = 0; dx < BSIZE && x + dx < BSIZE && board[x + dx][y] == SHIP; dx++) {
                        checked[x + dx][y] = true; // �������� ������ ��� �����������
                        if (hits[x + dx][y] != HIT) { // �������� �� ���������
                            isSunk = false;
                        }
                    }

                    // ���� ������� ��������� ���������
                    if (isSunk) {
                        sunkShips++;
                    }
                }
            }
        }
        return sunkShips;
    }

    /**
    * ��������� ������� ������� �� ������� ����.
    * @param N �������� �� �������� ������� ���-�� ��������������� ��������
    */
    void manualPlaceShips(int N) {
        int counts[4] = { (N * 40) / 100, (N * 30) / 100, (N * 20) / 100, (N * 10) / 100 };
        vector<Ship> ships = { Ship(1), Ship(2), Ship(3), Ship(4) };

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < counts[i]; j++) {
                while (true) {
                    try {
                        cout << "Enter the coordinates and direction to place a ship of size " << ships[i].size << " (form x:y:L/R/T/B) ";
                        string input;
                        cin >> input;                    
                        int y = input[0] - '0';  // x-����������
                        int x = input[2] - '0';  // y-����������
                        char direction = input[4]; // �����������              

                        // �������� �����������
                        if (direction != 'L' && direction != 'R' && direction != 'T' && direction != 'B') {
                            throw InvalidShipPlacementException();
                        }                    
                        
                        bool horizontal = (direction == 'L' || direction == 'R'); // �������������� ����������                    
                        bool canPlace = false;  // ��������� ����������� ����������

                        if (horizontal) {
                            if (direction == 'L') {
                                if (x - ships[i].size + 1 < 0 || isAdjacent(x - ships[i].size + 1, y, ships[i].size, true)) {
                                    throw InvalidShipPlacementException();
                                }
                                placeShip(x - ships[i].size + 1, y, ships[i].size, true);
                                canPlace = true;
                            }
                            else { // direction == 'R'
                                if (x + ships[i].size - 1 >= BSIZE || isAdjacent(x, y, ships[i].size, true)) {
                                    throw InvalidShipPlacementException();
                                }
                                placeShip(x, y, ships[i].size, true);
                                canPlace = true;
                            }
                        }
                        else { // ������������ ����������
                            if (direction == 'T') {
                                if (y - ships[i].size + 1 < 0 || isAdjacent(x, y - ships[i].size + 1, ships[i].size, false)) {
                                    throw InvalidShipPlacementException();
                                }
                                placeShip(x, y - ships[i].size + 1, ships[i].size, false);
                                canPlace = true;
                            }
                            else { // direction == 'B'
                                if (y + ships[i].size - 1 >= BSIZE || isAdjacent(x, y, ships[i].size, false)) {
                                    throw InvalidShipPlacementException();
                                }
                                placeShip(x, y, ships[i].size, false);
                                canPlace = true;
                            }
                        }  
                        
                    if (canPlace) {
                            break; // ������� ��������, ������� �� �����
                        }
                    }
                    catch (const InvalidShipPlacementException& e) {
                        cout << e.what() << endl;
                    }
                }
            }
        }
    }
};

class Player {
private:
    GameField gameField; // ���� ������
    string name; // ��� ������

public:
    Player(const string& playerName) : name(playerName) {}

    void generateShips(int N) {
        gameField.generateShips(N);
    }

    /**
    * ��������� ������� ������� �� ������� ����.
    * @param N ����� ���������� ������, ������� ����� ������������ ��� ���������� ��������.
    */
    void manualPlaceShips(int N) {
        gameField.manualPlaceShips(N);
    }

    /**
    * ���������� ������� ���� ������.
    * @param revealShips ���������, ������ �� ������� ���� ��������.
    */
    void displayBoard(bool revealShips = false) const {
        cout << name << "'s Board:" << endl;
        gameField.displayBoard(revealShips);
    }

    /**
    * ���������� ������ �� ���� ������.
    * @return ������ �� ������ GameField.
    */
    GameField& getGameField() {
        return gameField;
    }

    /**
    * ���������� ��� ������.
    * @return ��� ������.
    */
    string getName() const {
        return name;
    }

    /**
     * ������������ ���������� ����� ������ ��������.
     * @return ���������� ����� ������ ��������.
     */
    int countLiveSpots() const {
        return gameField.countLiveSpots();
    }

    /**
    * ������������ ���������� ��������� �� ��������.
    * @return ���������� ��������� �� ��������.
    */
    int countHits() const {
        return gameField.countHits();
    }

    /**
    * ������������ ���������� ����������� ��������.
    * @return ���������� ����������� ��������.
    */
    int countSunkShips() const {
        return gameField.countSunkShips();
    }
};

class ComputerPlayer : public Player {
private:
    vector<pair<int, int>> previousHits; // ������ ���������� ���������
    int lastHitX = -1; // X ���������� ���������� ���������
    int lastHitY = -1; // Y ���������� ���������� ���������
    bool searchingMode = false; // ����� ������ �������

public:
    ComputerPlayer(const string& playerName) : Player(playerName) {}


    /**
    *��������� ��� ����������, ������ ����������.
    *@param enemy �����(���������), �� �������� ����� ����������� �����.
    */
    void computerTurn(Player& enemy) {
        int x, y;

        cout << "\nPlayer 2 (comp) is attacking..." << endl;
        if (searchingMode) {
            // ���� ���� ����� �� �������, ���� �������� ������
            bool hitFound = false;

            // ���������� ��� 4 ��������� �����������
            pair<int, int> directions[] = {
                {0, 1},  // ������
                {0, -1}, // �����
                {1, 0},  // ����
                {-1, 0}  // �����
            };

            for (const auto& dir : directions) {
                int newX = lastHitX + dir.first;
                int newY = lastHitY + dir.second;

                // ���������, ��������� �� ����� ���������� � �������� ����
                if (newX >= 0 && newX < BSIZE && newY >= 0 && newY < BSIZE) {
                    try {
                        if (enemy.getGameField().attack(newY, newX)) {
                            hitFound = true;
                            lastHitX = newX;
                            lastHitY = newY;
                            previousHits.push_back({ newX, newY }); // ���������� ���������
                            cout << "Hit found at: (" << newY << ", " << newX << ")" << endl;
                            break; // ������� �� �����, ���� ����� ���������
                        }
                    }
                    catch (const invalid_argument& e) {
                        cout << e.what() << endl;
                    }
                }
            }

            // ���� �� ����� ����� ������, �� ������������ � ��������� ������
            if (!hitFound) {
                searchingMode = false;
            }
        }

        if (!searchingMode) {
            // ��������� �������
            do {
                x = rand() % BSIZE;
                y = rand() % BSIZE;
            } while (!enemy.getGameField().hasShips()); // ���������, ��� ���� �������

            try {
                if (enemy.getGameField().attack(y, x)) {
                    // ���� ������, ��������� � ����� ������
                    searchingMode = true;
                    lastHitX = x;
                    lastHitY = y;
                    previousHits.push_back({ x, y }); // ���������� ���������
                    cout << "Random hit found at: (" << y << ", " << x << ")" << endl;
                }
            }
            catch (const invalid_argument& e) {
                // ������� ���������, ���� ������ ��� ���������
                cout << e.what() << endl;
            }
        }
        enemy.displayBoard();           //����� �������� ����������
    }
};


int main() {
    srand(time(0));
    int N;
    int a = 0;
    int player1Hits_total = 0;
    int computerHits_total = 0;

    cout << "Enter N value: ";
    cin >> N;

    while (N < 10) {
        cout << "����������, ������� ����� ������ ��� ������ 10." << endl;
        cin >> N;
    }

    Player player1("Player 1");
    ComputerPlayer computer("Player 2 (comp)");
  
    //player1.manualPlaceShips(N);
    player1.generateShips(N);
    computer.generateShips(N);

    while (a == 0) {
        // ��� ������ 1
        computer.displayBoard();

        string input;
        int x1, y1;

        while (true) {
            cout << player1.getName() << ", your turn (form x:y) ";
            cin >> input;

            if (input == "-1") {
                a = 1;
                break;
            }

            // �������� �� ����
            if (input.length() > 3 || !isdigit(input[0]) || input[1] != ':' || !isdigit(input[2])) {
                cout << "Incorrect input. Make sure that you enter the coordinates in the x:y format, where x and y are digits." << endl;
                continue;
            }

            x1 = input[0] - '0'; // ������� ������� � �����
            y1 = input[2] - '0';

            try {
                computer.getGameField().attack(x1, y1); // �������� ���������
                break; // ������� ���������, ������� �� �����
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl; // ������� ��������� �� ������
            }

        }

        if (!computer.getGameField().hasShips()) {
            cout << player1.getName() << " Win!" << endl;
            break;
        }

        // ��� ������������� ����������
        computer.computerTurn(player1);
        player1Hits_total++;
        computerHits_total++;

        if (!player1.getGameField().hasShips()) {
            cout << computer.getName() << " Win!" << endl;
            break;
        }
    }

    // ����� ����������� ���
    cout << "\n\n Game end!\n" << endl;
    cout << "Field " << player1.getName() << " (with ships):" << endl;
    player1.displayBoard(true);
    cout << "Field " << computer.getName() << " (with ships):" << endl;
    computer.displayBoard(true);

    int player1Hits = player1.countHits();
    int computerHits = computer.countHits();

    double computerAccuracy = (static_cast<double>(player1Hits) / (player1Hits_total)) * 100;
    double player1Accuracy = (static_cast<double>(computerHits) / (computerHits_total)) * 100;

    cout << "Accuracy " << player1.getName() << ": " << player1Accuracy << "%" << endl;
    cout << "Accuracy " << computer.getName() << ": " << computerAccuracy << "%" << endl;

    // ����� ���������� ������������ ��������
    cout << "Destroyed ships of " << player1.getName() << ": " << player1.countSunkShips() << endl;
    cout << "Destroyed ships of " << computer.getName() << ": " << computer.countSunkShips() << endl;

    return 0;
}