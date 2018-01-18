#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main (int argc, char *argv[]) {
  ofSetupOpenGL(980, 460, OF_WINDOW);

  vector<string> args;
  if(argc > 1){
    for(int i = 0; i < argc; i++){
      args.push_back(argv[i]);
    }
  }

  ofApp *app = new ofApp();
  app->arguments = args;
  ofRunApp(app);
}
