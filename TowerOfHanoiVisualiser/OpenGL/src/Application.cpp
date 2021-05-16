//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include "Graphics2D.h"

#include <iostream>
#include <vector>

using namespace std;


Graphics2D engine = Graphics2D(960, 540, "Hanoi Visualised", false);
int ptr = 0;
vector<int> moves;
vector<vector<int>> pegs;
vector<vector<float>> colours = {{1, 0, 0}, {1, 0.65, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1}};
int n = 20;
int frameCount;
int speed = 10;
int current = -1;
float pegSpacing = 1.2;
float discWidth = 0.05; //per n, should be calculated when we change n	
float discHeight = 0.07;
bool solve = false;
int currentMoves;
string minMovesText;

void findSolution() {
	moves.clear();
	int base;
	if (n % 2 == 0) {
		base = 1;
	}
	else {
		base = 2;
	}

	for (int x = 0; x < n; x++) {
		//add a new base
		moves.push_back(0);
		moves.push_back(base);
		for (int i = moves.size() - 3; i >= 0; i--) { //reverse everything but change pegs
			if (moves[i] == base) {
				moves.push_back(0);
			}
			else if (moves[i] == 0) {
				moves.push_back(base);
			}
			else {
				moves.push_back(moves[i]);
			}
		}
		if (base == 1) {
			base = 2;
		}
		else {
			base = 1;
		}
	}
}

void resetPegs() {
	discWidth = (float)(pegSpacing - 0.1) / n;
	pegs.clear();
	pegs.resize(3, vector<int>(0));
	for (int i = n; i > 0; i--) {
		pegs[0].push_back(i); //place all discs on peg 0
	}
	current = -1;
	minMovesText = "MIN " + to_string((int)pow(2, n) - 1) + " MOVES";
	currentMoves = 0;
}

void renderPegs() {
	float currX = -1.2;
	float currY;
	float currWidth;
	int colourInd;
	for (int i = 0; i < pegs.size(); i++) {
		engine.line(currX, -1, currX, 0.5);
		currY = -1;
		for (int x = 0; x < pegs[i].size(); x++) {
			colourInd = pegs[i][x] % 6;
			engine.setFillColour(colours[colourInd][0], colours[colourInd][1], colours[colourInd][2], 1);
			currWidth = discWidth * pegs[i][x];
			engine.rect(currX - (float)currWidth / 2, currY, currWidth, discHeight);
			currY += discHeight;
		}
		currX += pegSpacing;
	}
}

void renderText() {
	engine.renderString(-engine.getAspectRatio() + 0.1, 0.8, minMovesText);
	engine.renderString(-engine.getAspectRatio() + 0.1, 0.7, to_string(currentMoves) + " MOVES");
	if (solve == true) {
		engine.renderString(0, 0.8, "AUTO SOLVE ON");
	}
	else {
		engine.renderString(0, 0.8, "AUTO SOLVE OFF");
	}
	engine.renderString(0, 0.7, to_string(speed) + " SPEED");
}

void windowInput() {
	switch (engine.getCurrentKey()) {
		case GLFW_KEY_S:
			if (solve == false) {
				frameCount = 0;
				ptr = 0;
				resetPegs();
				findSolution();
			}
			solve = !solve;
			break;
		case GLFW_KEY_R:
			speed = 10;
			break;
		case GLFW_KEY_LEFT:
			if (speed > 1) {
				speed -= 1;
			}
			break;
		case GLFW_KEY_RIGHT:
			speed += 1;
			break;
		case GLFW_KEY_ESCAPE:
			engine.closeWindow();
			break;
		default:
			break;
		}
}

void makeMove(int index) {
	if (current == -1) {
		if (pegs[index].size() > 0) {
			current = pegs[index][pegs[index].size() - 1];
			pegs[index].pop_back();
		}
	}
	else {
		if (pegs[index].size() == 0 || pegs[index][pegs[index].size() - 1] > current) {
			pegs[index].push_back(current);
			current = -1;
			currentMoves += 1;
		}
	}
}

void userInput() {
	int index = -1;
	switch (engine.getCurrentKey()) {
		case GLFW_KEY_1:
			index = 0;
			break;
		case GLFW_KEY_2:
			index = 1;
			break;
		case GLFW_KEY_3:
			index = 2;
			break;
		case GLFW_KEY_UP:
			if (n < 20) {
				n += 1;
				resetPegs();
			}
			break;
		case GLFW_KEY_DOWN:
			if (n > 1) {
				n -= 1;
				resetPegs();
			}
			break;
		default:
			break;
	}
	if (index != -1) {
		makeMove(index);
	}
}

void updateSolver() {
	if (ptr < moves.size()) {
		if (speed > 30) {
			int limit = speed - 30;
			if (moves.size() - ptr <= limit) {
				limit = moves.size() - ptr;
			}
			for (int i = 0; i < limit; i++) {
				makeMove(moves[ptr]);
				ptr += 1;
			}
		}
		else { //delay of 30 - speed
			if (frameCount % (31 - speed) == 0) {
				makeMove(moves[ptr]);
				ptr += 1;
			}
			frameCount += 1;
		}
	}
	else {
		solve = false;
	}
}

//for now the coordinates system goes from (-aspectRatio, -1) to (aspectRatio, 1)
int main() {
	resetPegs();
	engine.resizeText(0.04);
	while (engine.shouldClose() == false) {
		windowInput();
		if (solve == false) {
			userInput();
		}
		else {
			updateSolver();
		}
		renderPegs();
		renderText();
		engine.clear();
	}
}