#include "ofApp.h"

vector<body> bodies;
int maxParticles;

body::body(int x, int y, int z, double mass_, int hue)
{
    position = glm::vec3(x, y, z);                                                             // Initial position
    double init = 1;                                                                           // Range of possible initial velocities
    velocity = glm::vec3(ofRandom(-init, init), ofRandom(-init, init), ofRandom(-init, init)); // Initial random velocity
    mass = mass_;
    if (mass < 100)
    {
        size = mass / 10;
    }
    else
    {
        size = 20;
    }
    color.setHsb(hue, 255, 255, 255);
}

body::~body()
{
}

void body::update(int body_num)
{
    acceleration_calculate(body_num);
    velocity += acceleration*0.5;
    position += velocity;
    trail.push_back(position);
    if (trail.size() > 200)
    {
        trail.erase(trail.begin());
    }
}

void body::draw()
{
    ofSetColor(color);
    ofDrawSphere(position, size);
    ofPolyline trails(trail);
    trails.draw();
}

void body::acceleration_calculate(int body_num)
{
    double ax = 0;
    double ay = 0;
    double az = 0;

    for (int i = 0; i < maxParticles; i++)
    {
        if (i != body_num)
        {
            double r = pow((sqrt(pow((position[0] - bodies[i].position[0]), 2) + pow((position[1] - bodies[i].position[1]), 2) + pow((position[2] - bodies[i].position[2]), 2))), 3);
            ax -= bodies[i].mass * (position[0] - bodies[i].position[0]) / r;
            ay -= bodies[i].mass * (position[1] - bodies[i].position[1]) / r;
            az -= bodies[i].mass * (position[2] - bodies[i].position[2]) / r;
        }
    }
    acceleration[0] = ax;
    acceleration[1] = ay;
    acceleration[2] = az;
}

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofSetBackgroundColor(0, 0, 0);

    maxParticles = 10;

    ofEnableLighting();
    ofEnableDepthTest();
    light.setPosition(0,0,0);

    ofSetSphereResolution(10);
}

//--------------------------------------------------------------
void ofApp::update()
{

    if (bodies.size() > maxParticles)
    {
        bodies.erase(bodies.begin());
    }
    for (int i = 0; i < bodies.size(); i++)
    {
        bodies[i].update(i);
    }

    std::stringstream strm;
    strm << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cam.begin();
    ofRotateDeg(90, 0, 0, 1); // Rotate the camera to a position
    light.enable();
    // ofDrawAxis(400); // Draw axis

    for (int i = 0; i < bodies.size(); i++)
    {
        bodies[i].draw();
    }

    light.disable();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key)
    {
    case 'f':
    case 'F':
        ofToggleFullscreen();
        break;
    case ' ':
        int init = 200;

        body newbody(ofRandom(-init, init), ofRandom(-init, init), ofRandom(-init, init),5000, 0); // Create a Giant Body
        bodies.push_back(newbody);

        for (int i = 1; i < maxParticles; i++)
        {
            body newbody(ofRandom(-init, init), ofRandom(-init, init), ofRandom(-init, init), ofRandom(10, 60), ofRandom(20, 255));
            bodies.push_back(newbody);
        }
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}