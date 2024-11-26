#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <stdexcept> 
#include <windows.h> 

using namespace std;

const int BSIZE = 10; // Размер игрового поля
const char EMPTY = ' '; // Пустая клетка
const char SHIP = 'S'; // Корабль
const char HIT = 'X';  // Попадание
const char MISS = '*'; // Промах

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
    vector<vector<char>> hits; // Поле для отображения атак

public:
    GameField() {
        board.resize(BSIZE, vector<char>(BSIZE, EMPTY));
        hits.resize(BSIZE, vector<char>(BSIZE, EMPTY));
    }

    /**
    * Метод для отображения игрового поля.
    * @param revealShips Указывает, должны ли корабли быть видимыми.
    */
    void displayBoard(bool revealShips = false) const {
        // Установка цвета текста по умолчанию (белый)
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
                        // Устанавливаем цвет красным для попаданий
                        if (hits[i][j] == HIT) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // Красный
                        }
                        cout << hits[i][j];
                        // Возвращаем цвет текста по умолчанию
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Белый
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
    * Проверяет, находится ли корабль рядом с заданной позицией.
    * @param x X-координата позиции.
    * @param y Y-координата позиции.
    * @param shipSize Размер корабля.
    * @param horizontal Направление размещения (горизонтально или вертикально).
    * @return true, если рядом с позицией находится другой корабль.
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
    * Размещает корабль на поле.
    * @param x X-координата начала размещения.
    * @param y Y-координата начала размещения.
    * @param shipSize Размер корабля.
    * @param horizontal Направление размещения (true - горизонтально, false - вертикально).
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
    * Генерирует корабли случайным образом на поле.
    * @param N значение от которого зависит кол-во сгенерированных кораблей
    */
    void generateShips(int N) {
        int counts[4] = { (N * 40) / 100, (N * 30) / 100, (N * 20) / 100, (N * 10) / 100 };
        vector<Ship> ships = { Ship(1), Ship(2), Ship(3), Ship(4) };
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < counts[i]; j++) {
                bool placed = false;
                while (!placed) {
                    int orientation = rand() % 2; // 0 - горизонтально, 1 - вертикально
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
    * Обрабатывает атаку по заданным координатам.
    * @param y Y-координата атаки.
    * @param x X-координата атаки.
    * @return true, если атаковано успешно (попадание), и false, если промах.
    * @throw std::invalid_argument Если координаты выходят за пределы поля или клетка уже атакована.
    */
    bool attack(int y, int x) {
        if (x < 0 || x >= BSIZE || y < 0 || y >= BSIZE) 
            throw invalid_argument("Out of field."); // Проверка выхода за границы
        if (hits[x][y] != EMPTY) 
            throw invalid_argument("This place were atackted already."); // Если уже атаковали эту клетку

        if (board[x][y] == SHIP) {
            hits[x][y] = HIT; // Отметить попадание
            cout << "Hit!" << endl;
            return true;
        }
        else {
            hits[x][y] = MISS; // Отметить промах
            cout << "Miss!" << endl;
            return false;
        }
    }

    /**
    * Подсчитывает количество живых частей кораблей на игровом поле.
    * @return Количество живых частей кораблей.
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
    * Проверяет, есть ли живые корабли на поле.
    * @return true, если есть живые части кораблей; иначе false.
    */
    bool hasShips() const {
        return countLiveSpots() > 0;
    }

    /**
    * Подсчитывает количество попаданий по кораблям.
    * @return Количество попаданий по кораблям.
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
    *Подсчитывает количество затопленных кораблей.
    *@return Количество затопленных кораблей.
    */
    int countSunkShips() const {
        int sunkShips = 0;
        vector<vector<bool>> checked(BSIZE, vector<bool>(BSIZE, false)); // Ведение учёта проверенных клеток

        for (int x = 0; x < BSIZE; x++) {
            for (int y = 0; y < BSIZE; y++) {
                // Если нашли корабль, который еще не проверяли
                if (board[x][y] == SHIP && !checked[x][y]) {
                    // Проверка на горизонтальное или вертикальное расположение
                    bool isSunk = true;

                    // Проверяем горизонтально
                    for (int dy = 0; dy < BSIZE && y + dy < BSIZE && board[x][y + dy] == SHIP; dy++) {
                        checked[x][y + dy] = true; // Помечаем клетку как проверенную
                        if (hits[x][y + dy] != HIT) { // Проверка на попадание
                            isSunk = false;
                        }
                    }

                    // Проверяем вертикально
                    for (int dx = 0; dx < BSIZE && x + dx < BSIZE && board[x + dx][y] == SHIP; dx++) {
                        checked[x + dx][y] = true; // Помечаем клетку как проверенную
                        if (hits[x + dx][y] != HIT) { // Проверка на попадание
                            isSunk = false;
                        }
                    }

                    // Если корабль полностью уничтожен
                    if (isSunk) {
                        sunkShips++;
                    }
                }
            }
        }
        return sunkShips;
    }

    /**
    * Размещает корабли вручную на игровом поле.
    * @param N значение от которого зависит кол-во сгенерированных кораблей
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
                        int y = input[0] - '0';  // x-координата
                        int x = input[2] - '0';  // y-координата
                        char direction = input[4]; // направление              

                        // Проверка направления
                        if (direction != 'L' && direction != 'R' && direction != 'T' && direction != 'B') {
                            throw InvalidShipPlacementException();
                        }                    
                        
                        bool horizontal = (direction == 'L' || direction == 'R'); // Горизонтальное размещение                    
                        bool canPlace = false;  // Проверяем возможность размещения

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
                        else { // Вертикальное размещение
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
                            break; // Успешно размещён, выходим из цикла
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
    GameField gameField; // Поле игрока
    string name; // Имя игрока

public:
    Player(const string& playerName) : name(playerName) {}

    void generateShips(int N) {
        gameField.generateShips(N);
    }

    /**
    * Размещает корабли вручную на игровом поле.
    * @param N Общее количество клеток, которое можно использовать для размещения кораблей.
    */
    void manualPlaceShips(int N) {
        gameField.manualPlaceShips(N);
    }

    /**
    * Отображает игровое поле игрока.
    * @param revealShips Указывает, должны ли корабли быть видимыми.
    */
    void displayBoard(bool revealShips = false) const {
        cout << name << "'s Board:" << endl;
        gameField.displayBoard(revealShips);
    }

    /**
    * Возвращает ссылку на поле игрока.
    * @return Ссылка на объект GameField.
    */
    GameField& getGameField() {
        return gameField;
    }

    /**
    * Возвращает имя игрока.
    * @return Имя игрока.
    */
    string getName() const {
        return name;
    }

    /**
     * Подсчитывает количество живых частей кораблей.
     * @return Количество живых частей кораблей.
     */
    int countLiveSpots() const {
        return gameField.countLiveSpots();
    }

    /**
    * Подсчитывает количество попаданий по кораблям.
    * @return Количество попаданий по кораблям.
    */
    int countHits() const {
        return gameField.countHits();
    }

    /**
    * Подсчитывает количество затопленных кораблей.
    * @return Количество затопленных кораблей.
    */
    int countSunkShips() const {
        return gameField.countSunkShips();
    }
};

class ComputerPlayer : public Player {
private:
    vector<pair<int, int>> previousHits; // Список предыдущих попаданий
    int lastHitX = -1; // X координата последнего попадания
    int lastHitY = -1; // Y координата последнего попадания
    bool searchingMode = false; // Режим поиска корабля

public:
    ComputerPlayer(const string& playerName) : Player(playerName) {}


    /**
    *Выполняет ход компьютера, атакуя противника.
    *@param enemy Игрок(противник), на которого будет произведена атака.
    */
    void computerTurn(Player& enemy) {
        int x, y;

        cout << "\nPlayer 2 (comp) is attacking..." << endl;
        if (searchingMode) {
            // Если есть охота на корабль, ищем соседние клетки
            bool hitFound = false;

            // Определяем все 4 возможные направления
            pair<int, int> directions[] = {
                {0, 1},  // Вправо
                {0, -1}, // Влево
                {1, 0},  // Вниз
                {-1, 0}  // Вверх
            };

            for (const auto& dir : directions) {
                int newX = lastHitX + dir.first;
                int newY = lastHitY + dir.second;

                // Проверяем, находятся ли новые координаты в пределах поля
                if (newX >= 0 && newX < BSIZE && newY >= 0 && newY < BSIZE) {
                    try {
                        if (enemy.getGameField().attack(newY, newX)) {
                            hitFound = true;
                            lastHitX = newX;
                            lastHitY = newY;
                            previousHits.push_back({ newX, newY }); // Запоминаем попадание
                            cout << "Hit found at: (" << newY << ", " << newX << ")" << endl;
                            break; // Выходим из цикла, если нашли попадание
                        }
                    }
                    catch (const invalid_argument& e) {
                        cout << e.what() << endl;
                    }
                }
            }

            // Если не нашли новую клетку, то возвращаемся к случайным атакам
            if (!hitFound) {
                searchingMode = false;
            }
        }

        if (!searchingMode) {
            // Случайный выстрел
            do {
                x = rand() % BSIZE;
                y = rand() % BSIZE;
            } while (!enemy.getGameField().hasShips()); // Проверяем, что есть корабли

            try {
                if (enemy.getGameField().attack(y, x)) {
                    // Если попали, переводим в режим поиска
                    searchingMode = true;
                    lastHitX = x;
                    lastHitY = y;
                    previousHits.push_back({ x, y }); // Запоминаем попадание
                    cout << "Random hit found at: (" << y << ", " << x << ")" << endl;
                }
            }
            catch (const invalid_argument& e) {
                // Выводим сообщение, если клетка уже атакована
                cout << e.what() << endl;
            }
        }
        enemy.displayBoard();           //Вывод действий компьютера
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
        cout << "Пожалуйста, введите число больше или равное 10." << endl;
        cin >> N;
    }

    Player player1("Player 1");
    ComputerPlayer computer("Player 2 (comp)");
  
    //player1.manualPlaceShips(N);
    player1.generateShips(N);
    computer.generateShips(N);

    while (a == 0) {
        // Ход игрока 1
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

            // Проверка на ввод
            if (input.length() > 3 || !isdigit(input[0]) || input[1] != ':' || !isdigit(input[2])) {
                cout << "Incorrect input. Make sure that you enter the coordinates in the x:y format, where x and y are digits." << endl;
                continue;
            }

            x1 = input[0] - '0'; // Перевод символа в число
            y1 = input[2] - '0';

            try {
                computer.getGameField().attack(x1, y1); // Пытаемся атаковать
                break; // Успешно атаковали, выходим из цикла
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl; // Выводим сообщение об ошибке
            }

        }

        if (!computer.getGameField().hasShips()) {
            cout << player1.getName() << " Win!" << endl;
            break;
        }

        // Ход компьютерного противника
        computer.computerTurn(player1);
        player1Hits_total++;
        computerHits_total++;

        if (!player1.getGameField().hasShips()) {
            cout << computer.getName() << " Win!" << endl;
            break;
        }
    }

    // Вывод результатов боя
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

    // Вывод количества уничтоженных кораблей
    cout << "Destroyed ships of " << player1.getName() << ": " << player1.countSunkShips() << endl;
    cout << "Destroyed ships of " << computer.getName() << ": " << computer.countSunkShips() << endl;

    return 0;
}