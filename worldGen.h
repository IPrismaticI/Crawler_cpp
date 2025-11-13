#pragma once
#include <string>
using namespace std;

//Default world settings
int sizeX = 24;
int sizeY = 24;
int viewX = 11;
int viewY = 11;
class saveDat {
private:
	string worldDat[48][48] = {}; //[sizeY][sizeX]
	string itemDat[48][48] = {};
	string mobDat[48][48] = {};
public:
	//World
	void set_worldDat(int y, int x, string id) {
		worldDat[y][x] = id;
	};
	string get_worldDat(int y, int x) {
		return worldDat[y][x];
	}
	//Items (NOTE: Will be replaced with something more versatile later to allow for item overlap.)
	void set_itemDat(int y, int x, string id) {
		itemDat[y][x] = id;
	};
	string get_itemDat(int y, int x) {
		return itemDat[y][x];
	}
	//Mobs (NOTE: See items. Same update will come later.)
	void set_mobDat(int y, int x, string id) {
		mobDat[y][x] = id;
	};
	string get_mobDat(int y, int x) {
		return mobDat[y][x];
	}
};

//Init loaded data
saveDat curSave;


//worldGen tools
void roomGen(int refX, int refY) {
	int N = (rand() % (5));
	int S = -(rand() % (5));
	int W = (rand() % (5));
	int E = -(rand() % (5));
	for (int x = 0; x < (W - E + 1); x++) {
		for (int y = 0; y < (N - S + 1); y++) {
			curSave.set_worldDat(refX + E + x, refY + S + y, "core_floor_Debug");
		}
	}
}

int pathCont(int startX, int startY, int endX, int endY) {
	int differX = endX - startX;
	int differY = endY - startY;
	if (differX > differY) {
		if (differX > 0) {
			curSave.set_worldDat(startX - 1, startY, "core_floor_Debug");
			return 3;
		}
		else {
			curSave.set_worldDat(startX + 1, startY, "core_floor_Debug");
			return 4;
		}
	}
	else {
		if (differY > 0) {
			curSave.set_worldDat(startX, startY - 1, "core_floor_Debug");
			return 1;
		}
		else {
			curSave.set_worldDat(startX, startY + 1, "core_floor_Debug");
			return 2;
		}
	}
}

bool pathCheck(int startX, int startY, int endX, int endY) {
	if ((startX == endX) & (startY == endY)) {
		return true;
	}
	else {
		return false;
	}
}
void pathGen(int startX, int startY, int endX, int endY) {
	//Move randomly
	int roll = (rand() % (4));
	//Modify startX/Y
	if (roll > 2) {
		if (roll == 3) {
			startX += 1;
		}
		else {
			startX -= 1;
		}
	}
	else if (roll == 1) {
		startY += 1;
	}
	else {
		startX -= 1;
	}
	//Compare to endX/Y
	bool check = pathCheck(startX, startY, endX, endY);
	//If end reached, return.
	if (check == true) {
		return;
	}
	//If not the end, pave floor
	curSave.set_worldDat(startX, startY, "core_floor_Debug");
	//Move towards exit
	roll = pathCont(startX, startY, endX, endY);
	//Modify startX/Y
	if (roll > 2) {
		if (roll == 3) {
			startX += 1;
		}
		else {
			startX -= 1;
		}
	}
	else if (roll == 1) {
		startY += 1;
	}
	else {
		startX -= 1;
	}
	//Compare to endX/Y
	check = pathCheck(startX, startY, endX, endY);
	//If end reached, return.
	if (check == true) {
		return;
	}
	//If not the end, pave floor
	curSave.set_worldDat(startX, startY, "core_floor_Debug");
	//Move towards exit
	roll = pathCont(startX, startY, endX, endY);
	//Modify startX/Y
	if (roll > 2) {
		if (roll == 3) {
			startX += 1;
		}
		else {
			startX -= 1;
		}
	}
	else if (roll == 1) {
		startY += 1;
	}
	else {
		startX -= 1;
	}
	//Compare to endX/Y
	check = pathCheck(startX, startY, endX, endY);
	//If end reached, return.
	if (check == true) {
		return;
	}
	//If not the end, pave floor
	curSave.set_worldDat(startX, startY, "core_floor_Debug");
	//Recurse if end has not been reached
	pathGen(startX, startY, endX, endY);
}


void worldGen(int sizeX, int sizeY) {
	//Pregen
	//Pave worldDat
	for (int x = 0; x < sizeX; x++) {
		for (int y = 0; y < sizeY; y++) {
			string tile = "core_wall_Debug"; //Default tile.
			//Check for map borders.
			if (x == 0 or x == sizeX - 1 or y == 0 or y == sizeY - 1) {
				tile = "core_wall_Debug";
			};
			curSave.set_worldDat(y, x, tile);
		};
	};
	//Pave itemDat
	for (int x = 0; x < sizeX; x++) {
		for (int y = 0; y < sizeY; y++) {
			curSave.set_itemDat(y, x, " ");
		};
	};
	//Pave mobDat
	for (int x = 0; x < sizeX; x++) {
		for (int y = 0; y < sizeY; y++) {
			curSave.set_mobDat(y,x," ");
		};
	};
	//Mazegen
	///Generate exit 'O'
	int rollX = (rand() % (sizeX / 2 - 2)) + (sizeX / 2);
	int rollY = (rand() % (sizeY / 2 - 2)) + (sizeY / 2);
	curSave.set_worldDat(rollX, rollY, "core_deco_Debug");
	int exitX = rollX;
	int exitY = rollY;
	///Generate spawnpoint
	rollX = (rand() % (sizeX / 2 - 1)+1);
	rollY = (rand() % (sizeY / 2 - 1)+1);
	curSave.set_mobDat(rollX, rollY, "@");
	curSave.set_worldDat(rollX, rollY, "core_floor_Debug");
	int startX = rollX;
	int startY = rollY;
	///Generate exit & spawn room
	roomGen(exitX, exitY);
	roomGen(startX, startY);
	///Generate prime path
	pathGen(startX, startY, exitX, exitY);
	///Generate random rooms & paths
	// TODO
	//Postgen
	///Reinforce exit
	curSave.set_worldDat(exitX, exitY, "core_deco_Debug");
	///World Border
	for (int x = 0; x < sizeX; x++) {
		for (int y = 0; y < sizeY; y++) {
			//Check for map borders.
			if (x == 0 or x == sizeX - 1 or y == 0 or y == sizeY - 1) {
				curSave.set_worldDat(y, x, "core_wall_Debug");
			};
			
		};
	};
};