#include <bangtal.h>
#include <stdlib.h>
#include <time.h>

SceneID scene1, map, story;
ObjectID picture[16], goldicon1, goldicon2, mapicon, sceneicon, tax[4], ragebar1[100], ragebar2[100], whitebar1[100], whitebar2[100], startbutton, replaybutton, exitbutton, printsolvetime;
TimerID goldtimer, printgold, shuffletimer;
int board[4][4], blankx, blanky, goldspeed, gold, ragespeed, currage, curscene = 1, speedcnt, shufflecnt, solvetime;
char pictureimg[16][21] = {
	"Image/center-001.png",
	"Image/center-002.png",
	"Image/center-003.png",
	"Image/center-004.png",
	"Image/center-005.png",
	"Image/center-006.png",
	"Image/center-007.png",
	"Image/center-008.png",
	"Image/center-009.png",
	"Image/center-010.png",
	"Image/center-011.png",
	"Image/center-012.png",
	"Image/center-013.png",
	"Image/center-014.png",
	"Image/center-015.png",
	"Image/center-017.png"
};
char taximg[4][15] = {
	"Image/tax1.png", "Image/tax2.png","Image/tax3.png","Image/tax4.png"
};
int locate[4][4][2] = {
    340, 510, 490, 510, 640, 510, 790, 510,
	340, 360, 490, 360, 640, 360, 790, 360,
	340, 210, 490, 210, 640, 210, 790, 210,
	340,  60, 490,  60, 640,  60, 790,  60
};
clock_t start, end;

void init();
void init_replay();
void shuffle();
void move(int curx, int cury, int dx, int dy, ObjectID object, ObjectID blank, int mode);
void printObject(ObjectID object, SceneID scene, int i, int j);
void printrage();
void prepareexit();

void mouseCallback(ObjectID object, int x, int y, MouseAction action);
void timerCallback(TimerID timer);

int main() {
	init();
	startGame(story);
}

void init() {
	srand(time(NULL));
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene1 = createScene("", "Image/background4.png");
	map = createScene("", "Image/map.png");
	story = createScene("", "Image/story.png");

	for (int i = 0; i < 16; i++) {
		picture[i] = createObject(pictureimg[i]);
	}

	goldicon1 = createObject("Image/gold.png");
	locateObject(goldicon1, scene1, 598, 646);
	scaleObject(goldicon1, 0.17f);
	showObject(goldicon1);
	goldicon2 = createObject("Image/gold.png");
	locateObject(goldicon2, map, 598, 646);
	scaleObject(goldicon2, 0.17f);
	showObject(goldicon2);

	mapicon = createObject("Image/mapicon.png");
	locateObject(mapicon, scene1, 1110, 20);
	scaleObject(mapicon, 0.19f);
	showObject(mapicon);

	sceneicon = createObject("Image/sceneicon.png");
	locateObject(sceneicon, map, 1110, 25);
	scaleObject(sceneicon, 0.7f);
	showObject(sceneicon);

	startbutton = createObject("Image/start.png");
	locateObject(startbutton, story, 1100, 100);
	showObject(startbutton);
	
	replaybutton = createObject("Image/replay.png");
	locateObject(replaybutton, scene1, 30, 30);
	showObject(replaybutton);

	exitbutton = createObject("Image/exit.png");
	locateObject(exitbutton, scene1, 30, 80);

	printsolvetime = createObject("Image/solvetime.png");
	locateObject(printsolvetime, scene1, 498, 663);

	for (int i = 0; i < 100; i++) {
		ragebar1[i] = createObject("Image/rage.png");
		ragebar2[i] = createObject("Image/rage.png");
		whitebar1[i] = createObject("Image/ragedown.png");
		whitebar2[i] = createObject("Image/ragedown.png");
		locateObject(ragebar1[i], scene1, 1216, 77 + i * 5);
		locateObject(ragebar2[i], map, 1216, 77 + i * 5);
		locateObject(whitebar1[i], scene1, 1216, 77 + i * 5);
		locateObject(whitebar2[i], map, 1216, 77 + i * 5);
	}
	
	int cnt = 0;
	for (int i = 0; i < 4; i++) {
		tax[i] = createObject(taximg[i]);
		locateObject(tax[i], map, 30 + i * 300, 30 + i * 150);
		scaleObject(tax[i], 0.4f);
		showObject(tax[i]);
		for (int j = 0; j < 4; j++) {
			board[i][j] = cnt++;
		}
	}
	blankx = 3;
	blanky = 3;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printObject(picture[board[i][j]], scene1, i, j);
		}
	}
}
void init_replay() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			board[i][j] = i * 4 + j;
		}
	}
	blankx = 3;
	blanky = 3;
	goldspeed = 0;
	gold = 0;
	ragespeed = 0;
	currage = 0;
	speedcnt = 0;
	shufflecnt = 0;
	solvetime = 0;
	hideObject(exitbutton);
	hideObject(printsolvetime);
	printrage();
	setTimer(printgold, 0.0f);
	hideTimer();
	stopTimer(goldtimer);
	setTimer(goldtimer, 1.0f);
}
void shuffle() {
	int tmp, d[4][2] = { 0, 1, 1, 0, 0, -1, -1, 0 }, dir, dblankx, dblanky;
	bool isrun = true;
	while (isrun) {
		dir = rand() % 4;
		dblankx = blankx + d[dir][0];
		dblanky = blanky + d[dir][1];
		if (0 <= dblankx && dblankx < 4 && 0 <= dblanky && dblanky < 4) {
			isrun = false;
			locateObject(picture[board[blankx][blanky]], scene1, locate[dblankx][dblanky][0], locate[dblankx][dblanky][1]);
			locateObject(picture[board[dblankx][dblanky]], scene1, locate[blankx][blanky][0], locate[blankx][blanky][1]);
			tmp = board[blankx][blanky];
			board[blankx][blanky] = board[dblankx][dblanky];
			board[dblankx][dblanky] = tmp;
			blankx = dblankx;
			blanky = dblanky;
		}
	}
}
void move(int curx, int cury, int dx, int dy, ObjectID object, ObjectID blank, int mode) {
	if (gold < 3) return;
	decreaseTimer(printgold, 3.0f);
	gold -= 3;
	int tmp = board[curx][cury];
	board[curx][cury] = board[curx + dx][cury + dy];
	board[curx + dx][cury + dy] = tmp;
	locateObject(object, scene1, locate[curx + dx][cury + dy][0], locate[curx + dx][cury + dy][1]);
	locateObject(blank, scene1, locate[curx][cury][0], locate[curx][cury][1]);
	if (mode == 1) {
		blankx = curx;
		blanky = cury;
	}
	else if(mode == 0) {
		blankx = curx + dx;
		blanky = cury + dy;
	}
	int ck = 0;
	bool isend = true;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] != ck) {
				isend = false;
			}
			else if (board[i][j] == ck) {
				ck++;
				goldspeed++;
			}
		}
	}
	if (isend) {
		prepareexit();
	}
}
void printObject(ObjectID object, SceneID scene, int i, int j) {
	locateObject(object, scene, locate[i][j][0], locate[i][j][1]);
	showObject(object);
}
void printrage() {
	for (int i = 0; i < currage; i++) {
		if (curscene == 1) {
			showObject(ragebar1[i]);
			hideObject(whitebar1[i]);
		}
		else if(curscene == 2) {
			showObject(ragebar2[i]); 
			hideObject(whitebar2[i]);
		}
	}
	for (int i = currage; i < 100; i++) {
		if (curscene == 1) {
			showObject(whitebar1[i]);
			hideObject(ragebar1[i]);
		}
		else if (curscene == 2) {
			showObject(whitebar2[i]);
			hideObject(ragebar2[i]);
		}
	}
}
void prepareexit() {
	end = clock();
	solvetime = (end - start) / 1000;
	setTimer(printgold, float(solvetime));
	stopTimer(printgold);
	stopTimer(goldtimer);
	showObject(printsolvetime);
	showObject(exitbutton);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == picture[board[blankx][blanky]]) {
		if (action == MouseAction::MOUSE_DRAG_LEFT) {
			if (blanky > 0)
				move(blankx, blanky, 0, -1, object, picture[board[blankx][blanky - 1]], 0);
		}
		else if (action == MouseAction::MOUSE_DRAG_RIGHT) {
			if (blanky < 3)
				move(blankx, blanky, 0, 1, object, picture[board[blankx][blanky + 1]], 0);
		}
		else if (action == MouseAction::MOUSE_DRAG_UP) {
			if (blankx > 0)
				move(blankx, blanky, -1, 0, object, picture[board[blankx - 1][blanky]], 0);
		}
		else if (action == MouseAction::MOUSE_DRAG_DOWN) {
			if (blankx < 3)
				move(blankx, blanky, +1, 0, object, picture[board[blankx + 1][blanky]], 0);
		}
	}
	else if (object == mapicon) {
		enterScene(map);
		curscene = 2;
		printrage();
	}
	else if (object == sceneicon) {
		enterScene(scene1);
		curscene = 1;
		printrage();
	}
	else if (object == startbutton) {
		enterScene(scene1);
		shuffletimer = createTimer(0.001f);
		startTimer(shuffletimer);
		shuffle();
		goldtimer = createTimer(1.0f);
		printgold = createTimer(0.0f);
		showTimer(printgold);
		start = clock();
	}
	else if (object == replaybutton) {
		init_replay();
		startGame(story);
	}
	else if (object == exitbutton) {
		endGame();
	}
	else {
		bool istax = false;
		for (int i = 0; i < 4; i++) {
			if (object == tax[i]) {
				ragespeed++;
				currage += ragespeed * ragespeed;
				currage += i * 5;
				gold += (i + 1)*50;
				increaseTimer(printgold, float((i+1)*50));
				if (currage > 100) {
					prepareexit();
				}
				istax = true;
				printrage();
			}
		}
		if (!istax) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (object == picture[board[i][j]]) {
						int dx = blankx - i;
						int dy = blanky - j;
						if (action == MouseAction::MOUSE_DRAG_LEFT && dy == -1 && dx == 0) {
							move(i, j, dx, dy, object, picture[board[blankx][blanky]], 1);
						}
						else if (action == MouseAction::MOUSE_DRAG_RIGHT && dy == 1 && dx == 0) {
							move(i, j, dx, dy, object, picture[board[blankx][blanky]], 1);
						}
						else if (action == MouseAction::MOUSE_DRAG_UP && dy == 0 && dx == -1) {
							move(i, j, dx, dy, object, picture[board[blankx][blanky]], 1);
						}
						else if (action == MouseAction::MOUSE_DRAG_DOWN && dy == 0 && dx == 1) {
							move(i, j, dx, dy, object, picture[board[blankx][blanky]], 1);
						}
					}
				}
			}
		}
	}
}
void timerCallback(TimerID timer) {
	if (timer == goldtimer) {
		increaseTimer(goldtimer, 1.0f);
		startTimer(goldtimer);
		increaseTimer(printgold, 1.0f);
		gold += 1;
		if (currage > 0) {
			currage -= 1;
			speedcnt++;
			if (speedcnt > 20) {
				speedcnt = 0;
				ragespeed--;
			}
		}
		printrage();
	}
	if (timer == shuffletimer) {
		shuffle();
		shufflecnt++;
		if (shufflecnt <= 300) {
			increaseTimer(shuffletimer, 0.001f);
			startTimer(shuffletimer);
		}
		else {
			startTimer(goldtimer);
		}
	}
}