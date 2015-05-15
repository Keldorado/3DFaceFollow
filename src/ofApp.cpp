#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true); //means only do the draw at the same time you are cycling the gpu. So you don't have parts of the screen that are one frame ahead and one frame behind.
    ofSetFrameRate(120); //refresh rate
    
    finder.setup("haarcascade_frontalface_alt.xml"); //finds what ever the haarcasecadae is doing
    finder.setPreset(ObjectFinder::Fast); //Prefering to miss some face for the prefernce of speed.
    finder.getTracker().setSmoothingRate(.3); //Tells if the face it is reading is a face it has seen before
    cam.initGrabber(640, 480); //the camand that initiates the camera
    model.loadModel("dog/dog.3ds"); //loads the image
    ofEnableAlphaBlending(); //Turns on the alpha because our png has an alpha channel
    light.enable(); //enables light
    
    
    ofDisableArbTex(); //we need to call this for textures to work on models
    
    
}

//--------------------------------------------------------------
void ofApp::update() {
    cam.update();
    if(cam.isFrameNew()) { //If the frame we get from the camera is new, we want to update the picture that the camera is doing.
        finder.update(cam);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.draw(0,0); //draws what ever the hell the camera sees
    ofSetColor(255); //sets the color
    ofEnableDepthTest(); //Prevents the back of the model from shinning through to the front
    glShadeModel(GL_SMOOTH); //some model / light stuff
    light.enable(); //enables the light
    ofEnableSeparateSpecularLight(); //enables the specular light

    
    for(int i = 0; i < finder.size(); i++) { //look for the size of finder. The size of finder is the number of faces we see. 1 face = 1 loop, 2 face = 2 loop, etc.
        ofRectangle object = finder.getObjectSmoothed(i); //A rectangle that define the size of the face.
        //sunglasses.setAnchorPercent(.5, .5);
        float scaleAmount = .85 * object.width / sunglasses.getWidth(); //Determines that the sunglasses should be 85% of my face.
        ofPushMatrix();
        //ofLogNotice("xpos", ofToString(model.getPosition().y));
        ofTranslate(object.x + object.width / 2., object.y + object.height * .42); //Makes the glasses go to the middle of my face, and .42 down my face.
        //ofScale(scaleAmount, scaleAmount);
        //sunglasses.draw(0, 0); //
        model.drawFaces();
        
        
        
        ofPopMatrix(); //pops the matrix
        ofPushMatrix(); //pushes the matrix
        ofTranslate(object.getPosition()); //gets our object's position
        ofDrawBitmapStringHighlight(ofToString(finder.getLabel(i)), 0, 0); //draws the bit map
        ofLine(ofVec2f(), toOf(finder.getVelocity(i)) * 10); //gets our velocity
        ofPopMatrix(); //pops the matrix
    }
    
    ofDisableDepthTest(); //prevents the back of the model from shinning through to the front
    light.disable(); //disables our light
    ofDisableLighting(); //is ables the lighting
    ofDisableSeparateSpecularLight(); //disables the specular color
    
    ofSetColor(255, 255, 255, 255); //sets our color
}

//--------------------------------------------------------------
//draw the model manually
void ofApp::drawWithMesh(){
    
    //get the model attributes we need
    ofVec3f scale = model.getScale(); //gets the scale
    ofVec3f position = model.getPosition(); //gets the position
    float normalizedScale = model.getNormalizedScale(); //normailzes the scale
    ofVboMesh mesh = model.getMesh(0); //gets the mesh
    ofTexture texture; //gets the texture
    ofxAssimpMeshHelper& meshHelper = model.getMeshHelper( 0 ); //calls the mesh helper
    bool bHasTexture = meshHelper.hasTexture(); //tells the mesh helper that the mesh has texture
    if( bHasTexture ) {
        texture = model.getTextureForMesh(0); //gets the texture for the mesh
    }
    
    ofMaterial material = model.getMaterialForMesh(0); //gets the matiral for the mesh
    
    ofPushMatrix(); //pushes the matrix
    
    //translate and scale based on the positioning.
    ofTranslate(position);
    ofRotate(-ofGetMouseX(), 0, 1, 0);
    ofRotate(90,1,0,0);
    
    ofScale(normalizedScale, normalizedScale, normalizedScale);
    ofScale(scale.x,scale.y,scale.z);
    
    //modify mesh with some noise
    float liquidness = 5;
    float amplitude = mouseY/100.0;
    float speedDampen = 5;
    vector<ofVec3f>& verts = mesh.getVertices();
    for(unsigned int i = 0; i < verts.size(); i++){
        verts[i].x += ofSignedNoise(verts[i].x/liquidness, verts[i].y/liquidness,verts[i].z/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
        verts[i].y += ofSignedNoise(verts[i].z/liquidness, verts[i].x/liquidness,verts[i].y/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
        verts[i].z += ofSignedNoise(verts[i].y/liquidness, verts[i].z/liquidness,verts[i].x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
    }
    
    //draw the model manually
    if(bHasTexture) texture.bind();
    material.begin();
    mesh.drawFaces();
    material.end();
    if(bHasTexture) texture.unbind();
    
    ofPopMatrix();
    
}
