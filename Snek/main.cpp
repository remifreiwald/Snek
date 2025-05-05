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
int screenWidth = cellSize * cellCount;
int screenHeight = cellSize * cellCount;

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

class Snake {
public:
	deque<Vector2> body = { Vector2{ 6, 9 }, Vector2{ 5, 9 }, Vector2{ 4, 9 } };
	Vector2 direction = { 1, 0 };

	void Draw() {
		for (int i = 0; i < body.size(); i++) {
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = Rectangle{ x * cellSize, y * cellSize, (float)cellSize, (float)cellSize };
			DrawRectangleRounded(segment, 0.5, 6, SNEK_DARKGREEN);
		}
	}

	void Update() {
		body.pop_back(); // remove last segment of snake body
		Vector2 newBodySegment = Vector2Add(body[0], direction); // new segment in direction of movement
		body.push_front(newBodySegment); // add new segment to beginning of the list
	}
};

class Food {

public:
	Vector2 position;
	Texture2D texture;

	Food() {
		Image image = LoadImage("graphics/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos();
	}

	~Food() {
		UnloadTexture(texture);
	}

	void Draw() {
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}

	Vector2 GenerateRandomPos() {
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y };
	}
};

class Game {
public:
	Snake snake = Snake();
	Food food = Food();

	void Draw() {
		snake.Draw();
		food.Draw();
	}

	void Update() {
		snake.Update();
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
		}

		if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
			game.snake.direction = { 0, 1 };
		}

		if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
			game.snake.direction = { -1, 0 };
		}

		if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
			game.snake.direction = { 1, 0 };
		}

		// Drawing
		ClearBackground(SNEK_GREEN);
		game.Draw();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
