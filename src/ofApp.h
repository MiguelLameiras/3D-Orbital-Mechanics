#pragma once

#include "ofMain.h"

class body {
public:
	//Constructor and destructor
	body(int x, int y, int z, int hue);//Initial coordinates and hue
	~body();

	void update();
	void draw();

	ofColor color;
	float size;
	glm::vec3 position, velocity; // 3D vectors for particles physics
};

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	vector<body> bodies;
	int hue;
	ofEasyCam cam;
	ofLight light;
	int maxParticles;
};
