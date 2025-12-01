// 单机贪吃蛇, via raylib
#include "raylib.h"
#include <cstdlib>
#include <deque>
#include <functional>
#include <vector>

class Board;

const size_t FPS = 60;

enum Direction { DIR_UP = -2, DIR_DOWN = 2, DIR_LEFT = -1, DIR_RIGHT = 1 };

bool isOppositeDirection(Direction d1, Direction d2) { return d1 == -d2; }

class Snake {
private:
  enum Status { ALIVE, DEAD };
  Status _status;
  Direction _direction;
  size_t _x;
  size_t _y;     // 蛇头的位置
  size_t _speed; // 每秒移动的格子数
  Board &_board;
  std::deque<std::pair<size_t, size_t>> _body; // 蛇的身体
  size_t frameSinceLastMove;
  friend class Board;

public:
  Snake(Board &board);
  std::pair<size_t, size_t> head() const { return _body.back(); }
  std::pair<size_t, size_t> tail() const { return _body.front(); }

  void move();
  void reset();
  void setDirection(Direction direction) {
    if (isOppositeDirection(direction, _direction)) {
      return;
    }
    _direction = direction;
  }
};

enum class Cell { EMPTY, SNAKE, FOOD };

class Board {
private:
  int _width;
  int _height;
  std::vector<std::vector<Cell>> _board;
  Snake *_snake;
  Texture food;

public:
  Board(int width, int height);
  int width() const { return _width; }
  int height() const { return _height; }
  int cellSize() const {
    return std::min(GetScreenWidth() / _width, GetScreenHeight() / _height);
  }
  void render();
  void addFood();
  void reset();
  void setCell(size_t x, size_t y, Cell cell);
  void setSnake(Snake *snake) { _snake = snake; }
  Cell getCell(size_t x, size_t y) const { return _board[x][y]; }
};

Snake::Snake(Board &board)
    : _board(board), _status(ALIVE),
      _direction(static_cast<Direction>(GetRandomValue(0, 3))),
      _x(board.width() / 2), _y(board.height() / 2), _speed(5),
      frameSinceLastMove(0) {
  board.setSnake(this);
  _body.push_back(std::make_pair(_x, _y));
  _board.setCell(_x, _y, Cell::SNAKE);
}

void Snake::reset() {
  _status = ALIVE;
  _direction = static_cast<Direction>(GetRandomValue(0, 3));
  _x = _board.width() / 2;
  _y = _board.height() / 2;
  _speed = 5;
  _body.clear();
  _body.push_back(std::make_pair(_x, _y));
  _board.setCell(_x, _y, Cell::SNAKE);
  _board.setSnake(this);
  frameSinceLastMove = 0;
}

void Snake::move() {
  if (_status == DEAD) {
    return;
  }

  frameSinceLastMove++;
  if (frameSinceLastMove < FPS / _speed) { // 如果时间不够移动1格，不移动
    return;
  }

  frameSinceLastMove = 0;
  switch (_direction) {
  case DIR_UP:
    _y--;
    break;
  case DIR_DOWN:
    _y++;
    break;
  case DIR_LEFT:
    _x--;
    break;
  case DIR_RIGHT:
    _x++;
    break;
  }
  if (_x >= _board.width() || _y >= _board.height() || _x < 0 || _y < 0) {
    _status = DEAD;
    return;
  }

  _body.push_back(std::make_pair(_x, _y));
  if (_board.getCell(_x, _y) == Cell::FOOD) {
    _board.addFood();
    TraceLog(LOG_DEBUG, "eat food speed before: %d", _speed);
    _speed = std::min(int((_body.size() + 4) / 5) * 5, 25);
    TraceLog(LOG_DEBUG, "eat food speed after: %d", _speed);
  } else {
    auto [tailX, tailY] = _body.front();
    _board.setCell(tailX, tailY, Cell::EMPTY);
    _body.pop_front();
  }
  _board.setCell(_x, _y, Cell::SNAKE);
}

void Board::setCell(size_t x, size_t y, Cell cell) {
  if (x >= _width || y >= _height) {
    return;
  }
  _board[x][y] = cell;
}

void Board::reset() {
  _board.clear();
  _board.resize(_width, std::vector<Cell>(_height, Cell::EMPTY));
  addFood();
}

void Board::render() {
  // adjust according to the window size
  auto cellSize = this->cellSize();
  if (cellSize <= 0) {
    return;
  }
  DrawRectangle(0, 0, cellSize * _width, cellSize * _height, WHITE);

  auto [headX, headY] = _snake->head();
  auto [tailX, tailY] = _snake->tail();

  for (int i = 0; i < _width; i++) {
    for (int j = 0; j < _height; j++) {
      switch (_board[i][j]) {
      case Cell::EMPTY:
        continue;
      case Cell::SNAKE: {
        int deltaX = 0;
        int deltaY = 0;
        /*
        switch (_snake->_direction) {
        case UP: {
          deltaY = _snake->_speed * _snake->frameSinceLastMove / FPS * cellSize;
          break;
        }
        case DOWN: {
          deltaY =
              -_snake->_speed * _snake->frameSinceLastMove / FPS * cellSize;
          break;
        }
        case LEFT: {
          deltaX =
              -_snake->_speed * _snake->frameSinceLastMove / FPS * cellSize;
          break;
        }
        case RIGHT: {
          deltaX = _snake->_speed * _snake->frameSinceLastMove / FPS * cellSize;
          break;
        }
        }
        */
        if (i == headX && j == headY) { // is snake head
          DrawRectangle(i * cellSize + deltaX, j * cellSize + deltaY,
                        cellSize + std::abs(deltaX),
                        cellSize + std::abs(deltaY), BLACK);
        } else if (i == tailX && j == tailY) {
          DrawRectangle(i * cellSize + deltaX, j * cellSize + deltaY,
                        cellSize - std::abs(deltaX),
                        cellSize - std::abs(deltaY), BLACK);
        } else {
          DrawRectangle(i * cellSize + deltaX, j * cellSize + deltaY,
                        cellSize - std::abs(deltaX),
                        cellSize - std::abs(deltaY), BLACK);
        }
        break;
      }
      case Cell::FOOD:
        DrawCircle(i * cellSize + cellSize / 2., j * cellSize + cellSize / 2.,
                   cellSize / 2., RED);
        break;
      }
    }
  }
}

void Board::addFood() {
  int x = GetRandomValue(0, _width - 1);
  int y = GetRandomValue(0, _height - 1);
  _board[x][y] = Cell::FOOD;
  // auto food = GenImage
}

Board::Board(int width, int height)
    : _width(width), _height(height),
      _board(width, std::vector<Cell>(height, Cell::EMPTY)) {
  addFood();
}

class Game {
  Board _board;
  Snake _snake;
  std::unordered_map<int, std::function<void()>> _keybindings;
  int _boardWidth;
  int _boardHeight;
  int _windowWidth;
  int _windowHeight;

public:
  Game(int boardHeight = 50, int boardWidth = 50, int windowWidth = 800,
       int windowHeight = 600);
  void init();
  void start();
  void reset();
  void render();
  ~Game() { CloseWindow(); }
};

Game::Game(int boardHeight, int boardWidth, int windowWidth, int windowHeight)
    : _board(boardWidth, boardHeight), _snake(_board),
      _windowHeight(windowHeight), _windowWidth(windowWidth),
      _boardHeight(boardHeight), _boardWidth(boardWidth) {
  SetRandomSeed(GetTime());
  _board.setSnake(&_snake);
  _keybindings = {
      {KEY_R,
       [&]() {
         TraceLog(LOG_DEBUG, "restart");
         reset();
       }},
      {KEY_UP,
       [&]() {
         TraceLog(LOG_DEBUG, "up");
         _snake.setDirection(DIR_UP);
       }},
      {KEY_DOWN,
       [&]() {
         TraceLog(LOG_DEBUG, "down");
         _snake.setDirection(DIR_DOWN);
       }},
      {KEY_LEFT,
       [&]() {
         TraceLog(LOG_DEBUG, "left");
         _snake.setDirection(DIR_LEFT);
       }},
      {KEY_RIGHT,
       [&]() {
         TraceLog(LOG_DEBUG, "right");
         _snake.setDirection(DIR_RIGHT);
       }},
  };
}

void Game::start() {
  InitWindow(_windowWidth, _windowHeight, "贪吃蛇");
  SetTargetFPS(FPS);
  auto cellSize = _board.cellSize();
  auto img =
      GenImageColor(cellSize * _boardWidth, cellSize * _boardHeight, RAYWHITE);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (auto [key, func] : _keybindings) {
      if (IsKeyDown(key)) {
        func();
      }
    }
    _snake.move();
    _board.render();
    EndDrawing();
  }
}

void Game::reset() {
  _snake.reset();
  _board.reset();
}

void Game::render() { auto cellSize = _board.cellSize(); }

int main(int argc, const char **argv) {
  SetTraceLogLevel(LOG_DEBUG);
  Game game;
  game.start();
  return 0;
}