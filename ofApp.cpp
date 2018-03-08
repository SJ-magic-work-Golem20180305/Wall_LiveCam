/************************************************************
************************************************************/
#include "ofApp.h"


/************************************************************
************************************************************/
/******************************
******************************/
ofApp::ofApp(int _id)
: Cam_id(_id)
, VideoCam(NULL)
, Osc("127.0.0.1", 12349, 12348)
{
}

/******************************
******************************/
ofApp::~ofApp()
{
	if(VideoCam) delete VideoCam;
}

/******************************
******************************/
void ofApp::exit()
{
	printMessage("Good-bye");
}

//--------------------------------------------------------------
void ofApp::setup(){
	/********************
	********************/
	ofSetWindowTitle("Wall:LiveCam");
	ofSetVerticalSync(true);
	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
	ofSetWindowShape(WIDTH, HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	ofSetCircleResolution(50);
	
	/********************
	********************/
	SyphonTexture.setName("Screen Output");
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	
	setup_gui();
	
	/********************
	********************/
	VideoCam = new ofVideoGrabber;
	
	ofSetLogLevel(OF_LOG_VERBOSE);
    VideoCam->setVerbose(true);
	
	VideoCam->listDevices();// 上 2行がないと、表示されない.
	
	if(Cam_id == -1){
		std::exit(1);
	}else{
		VideoCam->setDeviceID(Cam_id);
		VideoCam->initGrabber(WIDTH, HEIGHT);
	}
}

/******************************
******************************/
void ofApp::setup_gui()
{
	/********************
	********************/
	gui.setup();
	{
		ofColor initColor = ofColor(255, 255, 255, 255);
		ofColor minColor = ofColor(0, 0, 0, 0);
		ofColor maxColor = ofColor(255, 255, 255, 255);
		
		gui.add(guiColor.setup("color", initColor, minColor, maxColor));
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	/********************
	********************/
	bool b_OscReceived = false;
	
	/********************
	********************/
	while(Osc.OscReceive.hasWaitingMessages()){
		ofxOscMessage m_receive;
		Osc.OscReceive.getNextMessage(&m_receive);
		
		if(m_receive.getAddress() == "/HumanPose"){
			MocapPos_x = m_receive.getArgAsFloat(0);
			MocapPos_y = m_receive.getArgAsFloat(1);
			MocapPos_z = m_receive.getArgAsFloat(2);
			
			b_OscReceived = true;
		}
	}
	
	/********************
	********************/
	ofxOscMessage m;
	m.setAddress("/HumanPose");
	m.addFloatArg(MocapPos_x);
	m.addFloatArg(MocapPos_y);
	m.addFloatArg(MocapPos_z);
	
	Osc.OscSend.sendMessage(m);
	
	/********************
	********************/
	VideoCam->update();
	if(VideoCam->isFrameNew()){
		// nothing.
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	/********************
	********************/
	// Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/********************
	********************/
	fbo.begin();
		ofClear(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		
		VideoCam->draw(0, 0, ofGetWidth(), ofGetHeight());
	fbo.end();
	
	/********************
	fbo加工
	********************/
	
	/********************
	********************/
	ofColor BaseColor = guiColor;
	ofSetColor(BaseColor);
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	/********************
	********************/
	SyphonTexture.publishScreen();
	
	/********************
	oF wallのみの情報表示
	********************/
	gui.draw();
	
	/*
	ofSetColor(255, 0, 0, 255);
	
	char buf[BUF_SIZE];
	sprintf(buf, "%7.4f", video.getPosition());
	ofDrawBitmapString(buf, 100, 50);
	
	if(BootMode == BOOTMODE_PLAY){
		float MousePos = ofMap(mouseX, 0, ofGetWidth(), 0, 1);
		sprintf(buf, "%7.4f", MousePos);
		ofDrawBitmapString(buf, 100, 70);
	}
	*/
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
