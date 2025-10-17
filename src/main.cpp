#include <deque>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 25;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool eventTriggered(double interval) {
  double currenTime = GetTime();
  if (currenTime - lastUpdateTime >= interval) {
    lastUpdateTime = currenTime;
    return true;
  }
  return false;
}

bool ElementInDeque(Vector2 element, deque<Vector2> deque) {
  for (unsigned int i = 0; i < deque.size(); i++) {
    if (Vector2Equals(deque[i], element)) {
      return true;
    }
  }
  return false;
}

class Snake {
public:
  bool addSegment = false;
  deque<Vector2> body{Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};

  Vector2 direciton = {1, 0};
  void Draw() {
    for (unsigned int i = 0; i < body.size(); i++) {
      float x = body[i].x;
      float y = body[i].y;
      DrawRectangleRounded(Rectangle{offset + x * cellSize,
                                     offset + y * cellSize, (float)cellSize,
                                     (float)cellSize},
                           0.5, 6, darkGreen);
    }
  }

  void Update() {
    body.push_front(Vector2Add(body[0], direciton));

    if (addSegment) {
      addSegment = false;
    } else {
      body.pop_back();
    }
  }

  void Reset() {
    body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    direciton = {1, 0};
  }
};

class Food {
public:
  Vector2 position;
  Texture2D texture;

  Food(deque<Vector2> snakeBody) {
    Image image = LoadImage("graphics/food.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position = GenerateRandomPos(snakeBody);
  }

  ~Food() { UnloadTexture(texture); }
  void Draw() {
    DrawTexture(texture, offset + position.x * cellSize,
                offset + position.y * cellSize, WHITE);
  }

  Vector2 GenerateRandomCell() {
    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);
    return Vector2{x, y};
  }
  Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {

    Vector2 position = GenerateRandomCell();
    while (ElementInDeque(position, snakeBody)) {
      position = GenerateRandomCell();
    }
    return position;
  }
};

class Game {
public:
  Snake snake = Snake();
  Food food = Food(snake.body);
  bool running = true;
  int score = 0;
  void Draw() {
    food.Draw();
    snake.Draw();
  }

  void Update() {
    if (!running)
      return;
    snake.Update();
    checkCollisionWithFood();
    checkCollisionWithEdges();
    checkCollisionWithTail();
  }

  void checkCollisionWithFood() {
    if (Vector2Equals(snake.body[0], food.position)) {
      food.position = food.GenerateRandomPos(snake.body);
      snake.addSegment = true;
      score++;
    }
  }

  void checkCollisionWithEdges() {
    if (snake.body[0].x == cellCount || snake.body[0].x == -1) {
      GameOver();
    }

    if (snake.body[0].y == cellCount || snake.body[0].y == -1) {
      GameOver();
    }
  }

  void checkCollisionWithTail() {
    deque<Vector2> headlessBody = snake.body;
    headlessBody.pop_front();
    if (ElementInDeque(snake.body[0], headlessBody)) {
      GameOver();
    }
  }

  void GameOver() {
    score = 0;
    snake.Reset();
    food.position = food.GenerateRandomPos(snake.body);
    running = false;
  }
};

int main() {

  cout << "Starting the game.. " << endl;
  InitWindow(2 * offset + cellSize * cellCount,
             2 * offset + cellSize * cellCount, "Snake Game");
  SetTargetFPS(60);
  Game game = Game();

  while (WindowShouldClose() == false) {
    BeginDrawing();
    if (eventTriggered(0.2))
      game.Update();

    if (IsKeyPressed(KEY_UP)) {
      game.snake.direciton = {0, -1};
      game.running = true;
    } else if (IsKeyPressed(KEY_DOWN)) {
      game.snake.direciton = {0, 1};
    } else if (IsKeyPressed(KEY_RIGHT)) {
      game.snake.direciton = {1, 0};
    } else if (IsKeyPressed(KEY_LEFT)) {
      game.snake.direciton = {-1, 0};
    }

    // Drawing
    ClearBackground(green);
    DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5,
                                   (float)cellCount * cellSize + 10,
                                   (float)cellCount * cellSize + 10},
                         5, darkGreen);

    DrawText("RetroSnake", offset - 5, 20, 40, darkGreen);
    DrawText(TextFormat("%i", game.score), offset - 5,
             offset + cellSize * cellCount + 10, 40, darkGreen);

    game.Draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
