#include "ofApp.h"

body::body(int x, int y,int z, int hue)
{
    position = glm::vec3(x,y,z);
    velocity = glm::vec3(0,0,0);
    size = 4;
    color.setHsb(hue,255,255,255);
}

body::~body(){

}

void body::update(){
    position += velocity;
    double r = sqrt(position[0]*position[0] + position[1]*position[1] + position[2]*position[2]);//Distance to Mass
    double abs_velocity = sqrt(10 / r);                                                          // Velocity modulus;
    velocity[0] = abs_velocity;
    velocity[1] = abs_velocity;
    velocity[2] = abs_velocity;
}

void body::draw(){
    ofSetColor(color);
    ofDrawSphere(position, size);
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetBackgroundColor(0, 0, 0);
    hue = 0;

    maxParticles = 5;
    ofEnableLighting();
    ofEnableDepthTest();
    light.setPosition(0, -600, 300);

    ofSetSphereResolution(10);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(bodies.size() > maxParticles){
        bodies.erase(bodies.begin());
    }
    for (int i = 0; i < bodies.size();i++){
        bodies[i].update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    ofRotateDeg(180, 0, 0, 1);//Rotate the camera to a position
    light.enable();
    ofDrawAxis(400);//Draw axis 

    for (int i = 0; i < bodies.size(); i++)
    {
        bodies[i].draw();
    }

    light.disable();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
        case 'F':
            ofToggleFullscreen();
            break;
        case ' ':
            int init = 5;
            body newbody(ofRandom(-init, init), ofRandom(-init, init), ofRandom(-init, init), hue);
            bodies.push_back(newbody);
            break;
        }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    hue = ofRandom(255);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}