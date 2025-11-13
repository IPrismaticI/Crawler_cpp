// Libs
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

//For keyboard input reading
#include<conio.h>

//For file input reading
#include <filesystem>
namespace fs = std::filesystem;
#include <fstream>

//Local files
#include "worldGen.h"

int found = 0;


using namespace std;

//Init Variables
bool isGameOver = false;
string choiceHolder;
int returnDat;
char ch; //Key input holder
bool exitHolder = false;

//Data Holder Classes
//TODO class mob
//TODO class item

class tile {
private:
	string id;

	char tileKey;
	string tileImg;
	string name;

	string group;
public:
	tile(char nutileKey, string nuid, string nutileImg, string nuname, string nugroup) {
		id = nuid;
		tileKey = nutileKey;
		tileImg = nutileImg;
		name = nuname;
		group = nugroup;
	}
	string get_id() {
		return id;
	}
	char get_tileKey() {
		return tileKey;
	}
	string get_tileImg() {
		return tileImg;
	}
	string get_name() {
		return name;
	}
	string get_group() {
		return group;
	}
};

//Init data files
vector<tile> tileDat;



//Define input codes
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77


bool loadDat() {
	try {
		//Mobs
		// TODO
		//Items
		// TODO
		//Tiles
		string path = "dat/tiles/"; //TODO: Make an exception to check if dat/tiles/ exists.
		ifstream loadFile;
		int fileCount = 0;
		string line;
		for (const auto& entry : fs::directory_iterator(path)) {
			cout << "Loading game data...\n\nOpening file : " << entry.path();
			loadFile.open(entry.path());
			//Check if open was successful. Throw exception if not.
			if (!loadFile.is_open()) {
				throw "loadFileOpenFailed";
			}
			cout << "\nFile opened successfully. Getting data...";
			//Copy data to reference list.
			int lineNum = 0;
			char charHolder = loadFile.get();
			string stringHolder[4];
			while (getline(loadFile, line)) {
				if (lineNum != 0) {
					stringHolder[lineNum - 1] = line;
				}
				++lineNum;
			}
			tileDat.push_back(tile(charHolder, stringHolder[0], stringHolder[1], stringHolder[2],stringHolder[3]));
			cout << "\nData copied. Closing file...";
			loadFile.close();
			//Check if file closure succeeded. Throw exception if not.
			if (loadFile.is_open()) {
				throw "loadFileCloseFailed";
			}
			cout << "\nFile closed. Continuing to next file.";
			system("cls"); //Clear screen
			++fileCount;
		}
		return true;
	}
	catch(const char* msg) {
		cout << "\nEXCEPTION FOUND: " << msg << "\nClosing program.";
		return false;
	}
}

//Find data properties
///Mobs
//TODO
///Items
//TODO
///Tiles
////tileKey
char findTileDat_tileKey(int nuY, int nuX) {
	int tileCount = 0;
	int testKey = tileDat.size();
	string target = curSave.get_worldDat(nuY, nuX);
	while (tileCount < tileDat.size()) {
		if (tileDat[tileCount].get_id() == target) {
			return tileDat[tileCount].get_tileKey();
		}
		++tileCount;
	}
	return 'E';
}
////tileImg
//TODO
////name
//TODO
////group
string findTileDat_group(int nuY, int nuX) {
	int tileCount = 0;
	string target = curSave.get_worldDat(nuY, nuX);
	while (tileCount < tileDat.size()) {
		if (tileDat[tileCount].get_id() == target) {
			return tileDat[tileCount].get_group();
		}
		++tileCount;
	}
	return "Error";
}


void waitForInput() {
	ch = NULL;
	do {
		ch = _getch(); // Wait for a key press
	} while (ch == NULL);
};


int makeSave() {
	try {
		//List current saves
		string path = "saves/"; //TODO: Make an exception to check if saves/ does not exist.
		for (const auto& entry : fs::directory_iterator(path)) {
			cout << "\n"<< entry.path().filename() << endl;
		}
		cout << "\nType a new save name and press ENTER to confirm. Type in an existing save name to overwrite.";
		string input;
		cin >> input;
		//Create save file
		ofstream outfile("saves/" + input+".txt");
		//Confirm save.
		if (!outfile.is_open()) {
			throw "saveFileOpenFailed";
		}
		//Write to save
		for (int y = 0; y < sizeY; y++) {
			string log;
			for (int x = 0; x < sizeX; x++) {
				log = log + curSave.get_worldDat(y, x) + " ";
			};
			outfile << log << endl;
		};
		outfile.close();
		//Confirm close.
		if (outfile.is_open()) {
			throw "saveFileCloseFailed";
		}
		return 0;
	}
	catch (const char* msg) {
		cout << "\nEXCEPTION FOUND: " << msg;
		return 1;
	}
	return 0;
};

int loadSave() {
	try{
		system("cls"); //Clear screen
		//Display save folder dat.
		string path = "saves/"; //TODO: Make an exception to check if saves/ does not exist.
		int fileCount = 0;
		ifstream loadFile;
		string line;
		for (const auto& entry : fs::directory_iterator(path)) {
			cout << entry.path().filename() << endl; //TODO: Find a way to remove "" and .txt
			++fileCount;
		}
		if (fileCount == 0) {
			cout << "No save files found. Press any button to start a new game.";
			waitForInput();
			system("cls"); //Clear screen
			worldGen(sizeX, sizeY);
			return 0;
		}
		//Load worldDat from save folder.
		cout << "\nType in a file name to load.\n";
		string input;
		cin >> input;
		loadFile.open(path+input+".txt");
		if (loadFile.is_open()) {
			while (loadFile.good()) {
				loadFile >> input;
				cout << input;
			}
		}
		return 0;
	}
	catch (const char* msg) {
		cout << "\nEXCEPTION FOUND: " << msg << "\nClosing program.";
		return 0;
	}
	return 0;
};


void printAll() {
	for (int y = 0; y < sizeY; y++) {
		string log = "\n";
		for (int x = 0; x < sizeX; x++) {
			if (curSave.get_itemDat(y,x) != " ") {
				log = log + curSave.get_itemDat(y, x);
			}
			else if (curSave.get_mobDat(y, x) != " ") {
				log = log + curSave.get_mobDat(y, x);
			}
			else {
				log = log + curSave.get_worldDat(y, x);
			}
		};
		cout << log;
	};
}

void printWorld(int curY, int curX) {
	string log = "\n╔"; //These unicode icons were an attempt at making a border. It functions, but sadly the icons themselves got lost in translation.
	for (int c = 0; c < viewX; c++) {
		log = log + "═";
	}
	log = log + "╗";
	cout << log;
	for (int y = 0; y < viewY; y++) {
		log = "\n║";
		for (int x = 0; x < viewX; x++) {
			int nuY = y - (viewY / 2) + curY;
			int nuX = x - (viewX / 2) + curX;
			//TODO: Put get_Dats in a checker that looks for the group of the target. Tile appearance depends on group.
			//If out of bounds, display ' '. Else, read data.
			if (nuX < 0 or nuY < 0) {
				log = log + " ";
			}
			else if (findTileDat_group(nuY, nuX) == "wall") {
				log = log + findTileDat_tileKey(nuY, nuX);
			}
			else if (curSave.get_itemDat(nuY, nuX) != " ") { //TODO
				log = log + curSave.get_itemDat(nuY, nuX); 
			}
			else if (curSave.get_mobDat(nuY, nuX) != " ") { //TODO
				log = log + curSave.get_mobDat(nuY, nuX);
			}
			else if (findTileDat_group(nuY, nuX) != "invis") {
				log = log + findTileDat_tileKey(nuY, nuX);
			}
			else {
				log = log + " ";
			}
		};
		while (log.length() < viewX + 2) {
			log = log + " ";
		};
		log = log + "║";
		cout << log;
	};
	log = "\n╚";
	for (int c = 0; c < viewX; c++) {
		log = log + "═";
	}
	log = log + "╝";
	cout << log;
};

//TODO findCoordsWorld
//TODO findCoordsItem
void findCoordsMob(string target, bool& holder, int& curY, int& curX) {
	bool confirm = false;
	do {
		for (int x = 0; x < sizeX; x++) {
			for (int y = 0; y < sizeY; y++) {
				//Check coordset data.
				if (curSave.get_mobDat(y, x) == target) {
					holder = true;
					curY = y;
					curX = x;
					return;
				};
			};
		};
	} while (confirm == false);
};

void movePlayer(int& y, int& x, int direction) {
	//Add direction to coords.
	int nuY = 0;
	int nuX = 0;
	if (direction == 1) {//Southwest
		nuY = y + 1;
		nuX = x - 1;
	}
	else if (direction == 2) {//South
		nuY = y + 1;
		nuX = x;
	}
	else if (direction == 3) {//Southeast
		nuY = y + 1;
		nuX = x + 1;
	}
	else if (direction == 4) {//West
		nuY = y;
		nuX = x - 1;
	}
	else if (direction == 5) {//Center, UNUSED
		nuY = y;
		nuX = x;
	}
	else if (direction == 6) {//East
		nuY = y;
		nuX = x + 1;
	}
	else if (direction == 7) {//Northwest
		nuY = y - 1;
		nuX = x - 1;
	}
	else if (direction == 8) {//North
		nuY = y - 1;
		nuX = x;
	}
	else if (direction == 9) {//Northeast
		nuY = y - 1;
		nuX = x + 1;
	}
	//Check coords for world objects and mobs. If the slot is clear, move to that position.
	if (findTileDat_group(nuY, nuX) == "floor" and curSave.get_mobDat(nuY, nuX) == " ") {
		curSave.set_mobDat(y, x," ");
		curSave.set_mobDat(nuY, nuX,"@");
		x = nuX;
		y = nuY;
	}
}

void base() {//Exploration loop
	exitHolder = false;
	bool holder = false;
	int y = 0;
	int x = 0;
	findCoordsMob("@", holder, y, x);
	if (holder == false) {
		cout << "Error: Failed to find player";
		return;
	};
	do {
		printWorld(y, x);//Redraw screen
		//printAll();
		waitForInput();
		//Movement Keys
		if (ch == 'w' or ch == 'W' or ch == '8') {
			movePlayer(y, x, 8);
		}
		else if (ch == 's' or ch == 'S' or ch == '2') {
			movePlayer(y, x, 2);
		}
		else if (ch == 'a' or ch == 'A' or ch == '4') {
			movePlayer(y, x, 4);
		}
		else if (ch == 'd' or ch == 'D' or ch == '6') {
			movePlayer(y, x, 6);
		}
		//Diagonal direction keys
		else if (ch == '7') {
			movePlayer(y, x, 7);
		}
		else if (ch == '9') {
			movePlayer(y, x, 9);
		}
		else if (ch == '1') {
			movePlayer(y, x, 1);
		}
		else if (ch == '3') {
			movePlayer(y, x, 3);
		}
		else if (ch == '5') {
			movePlayer(y, x, 5);
		}
		//Save game
		else if (ch == 'e') {
			makeSave();
		}
		//Exit game
		else if (ch == '~') {
			exitHolder = true;
		}
		system("cls"); //Clear screen
	} while (exitHolder != true);
};


int main() {
	//Load
	bool loadStatus = loadDat();
	if (loadStatus == true){
		do
		{
			//Title
			cout << "\n\nCRAWLER - INDEV 0.6\n\n1 - New Game\n2 - Load Game\n3 - Exit";
			waitForInput();
			system("cls"); //Clear screen
			//Random Number Generator
			srand(time(0));

			//Game Loop
			if (ch == '3') {
				return 0;
			}
			else {
				if (ch == '2') {
					loadSave();
				}
				else {
					//Generate new worldDat
					worldGen(sizeX, sizeY);
				}

				//Init exploration loop
				base();

				//Continue?
				cout << "\n\nTry again? (Y/N)";

				waitForInput();
				if (ch == 'y' or ch == 'Y')//If y, continue loop
				{
					system("cls"); //Clear screen
					isGameOver = false;
				}
				else if (ch == 'n' or ch == 'N')//If n, end loop
				{
					isGameOver = true;
				}
			return 0;
			}
		} while (isGameOver == false);
	}
	return 0;
};