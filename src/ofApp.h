#pragma once

#include "ofMain.h"
#include "ofxPiMapper.h"

enum Direction {
	LEFT = 8,
	RIGHT = 10,
	UP = 6,
	DOWN = 9,
	CENTER = 7,
	SEPARATOR = 32
};

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	std::string inputToString(int direction, int state);
	void inputToMapper(int direction, int state);
	ofSerial serial;
	vector<int> buffer;
	ofxPiMapper mapper;
	
	float timer;
};
