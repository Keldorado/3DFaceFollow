#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
    ofVideoGrabber cam; //grabs stuff form the camera
    ofxCv::ObjectFinder finder; //Where the face finding happens
    ofImage sunglasses; //the variable in which we are loding the image
    
    //this is our model we'll draw
    ofxAssimpModelLoader model;
    
    ofLight light;
    
    //we added these functions to make it easier to switch between the two methods of drawing
    void drawWithModel();
    void drawWithMesh();
		
};
