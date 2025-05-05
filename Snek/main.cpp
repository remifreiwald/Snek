/*
* Copyright (c) 2025, Remi Freiwald
* All rights reserved.
* 
* This source code is licensed under the BSD-style license found in the
* LICENSE file in the root directory of this source tree.
*/

/*
* This project is based on "C++ Snake game using raylib - Beginner Tutorial (OOP)":
* www.youtube.com/watch?v=LGqsnM_WEK4
*/

#include <raylib.h>
#include <raymath.h>
#include <deque>

using namespace std;

Color SNEK_GREEN = { 173, 204, 96, 255 };
Color SNEK_DARKGREEN = { 43, 51, 24, 255 };

// grid for 750x750 pixels
int cellSize = 30;
int cellCount = 25;
int offset = 75;
int screenWidth = cellSize * cellCount + 2 * offset;
int screenHeight = cellSize * cellCount + 2 * offset;

double lastUpdateTime = 0;

bool eventTriggered(double interval) {
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval) {
		lastUpdateTime = currentTime;
		return true;
	} else {
		return false;
	}
}

bool elementInDeque(Vector2 element, deque<Vector2> deque) {
	for (unsigned int i = 0; i < deque.size(); i++) {
		if (Vector2Equals(deque[i], element)) {
			return true;
		}
	}
	return false;
}

class Snake {
public:
	deque<Vector2> body;
	Vector2 direction;
	bool addSegment = false;

	Snake() {
		Reset();
	}

	void Draw() {
		for (int i = 0; i < body.size(); i++) {
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = Rectangle{ x * cellSize + offset, y * cellSize + offset, (float)cellSize, (float)cellSize };
			DrawRectangleRounded(segment, 0.5, 6, SNEK_DARKGREEN);
		}
	}

	void Update() {
		// create a new segment in the direction of movement
		Vector2 newBodySegment = Vector2Add(body[0], direction);

		// add the new segment to the beginning of the list
		body.push_front(newBodySegment);

		if (addSegment) {
			// we added the segment, that's it
			addSegment = false;
		} else {
			// just move the snake, but don't make it longer, so remove the last segment
			body.pop_back();
		}
	}

	void Reset() {
		body = { Vector2{ 6, 9 }, Vector2{ 5, 9 }, Vector2{ 4, 9 } };
		direction = { 1, 0 };
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

	~Food() {
		UnloadTexture(texture);
	}

	void Draw() {
		DrawTexture(texture, position.x * cellSize + offset, position.y * cellSize + offset, WHITE);
	}

	Vector2 GenerateRandomCell() {
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2 { x, y };
	}

	Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {
		Vector2 newPosition = GenerateRandomCell();
		while (elementInDeque(newPosition, snakeBody)) {
			newPosition = GenerateRandomCell();
		}
		return newPosition;
	}
};

class Game {
public:
	Snake snake = Snake();
	Food food = Food(snake.body);
	bool running = true;
	int score = 0;

	void Draw() {
		snake.Draw();
		food.Draw();
	}

	void Update() {
		if (running) {
			snake.Update();
			CheckCollisionWithFood();
			CheckCollisionWithEdges();
			CheckCollisionWithTail();
		}
	}

	void CheckCollisionWithFood() {
		if (Vector2Equals(snake.body[0], food.position)) {
			food.position = food.GenerateRandomPos(snake.body);
			snake.addSegment = true;
			score++;
		}
	}

	void CheckCollisionWithEdges() {
		if (snake.body[0].x >= cellCount || snake.body[0].x <= -1) {
			GameOver();
		} else if (snake.body[0].y >= cellCount || snake.body[0].y <= -1) {
			GameOver();
		}
	}

	void CheckCollisionWithTail() {
		deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();
		if (elementInDeque(snake.body[0], headlessBody)) {
			GameOver();
		}
	}

	void GameOver() {
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.body);
		running = false;
		score = 0;
	}
};

int main() {
	
	InitWindow(screenWidth, screenHeight, "Snek");

	SetTargetFPS(60);

	Game game = Game();

	while(!WindowShouldClose()) {
		BeginDrawing();

		if (eventTriggered(0.2)) {
			game.Update();
		}

		if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
			game.snake.direction = { 0, -1 };
			game.running = true;
		}

		if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
			game.snake.direction = { 0, 1 };
			game.running = true;
		}

		if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
			game.snake.direction = { -1, 0 };
			game.running = true;
		}

		if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
			game.snake.direction = { 1, 0 };
			game.running = true;
		}

		// Drawing
		ClearBackground(SNEK_GREEN);
		DrawRectangleLinesEx(Rectangle{ (float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10 }, 5, SNEK_DARKGREEN);
		DrawText("Snek", offset - 5, 20, 40, SNEK_DARKGREEN);
		DrawText(TextFormat("Score: %i", game.score), offset - 5, offset + cellSize * cellCount + 15, 40, SNEK_DARKGREEN);
		game.Draw();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
