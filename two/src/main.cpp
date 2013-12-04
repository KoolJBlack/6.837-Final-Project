#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "extra.h"

#include "modelerapp.h"
#include "ModelerView.h"


using namespace std;

int main( int argc, char* argv[] )
{
	if( argc < 2 )
	{
		cout << "Usage: " << argv[ 0 ] << " PREFIX" << endl;
		cout << "For example, if you're trying to load data/cheb.skel, data/cheb.obj, and data/cheb.attach, run with: " << argv[ 0 ] << " data/cheb" << endl;
		return -1;
	}

    // Initialize the controls.  You have to define a ModelerControl
    // for every variable name that you define in the enumeration.

    // The constructor for a ModelerControl takes the following arguments:
    // - text label in user interface
    // - minimum slider value
    // - maximum slider value
    // - step size for slider
    // - initial slider value

	const int NUM_FACES = 11;

	ModelerControl controls[ NUM_FACES ];
	string faceNames[NUM_FACES]={ "Head", "Left Smile", "Right Smile", "Left Frown", "Right Frown", "Left Eye Closed", "Right Eye Closed", 
		"Left Eyebrow Up", "Right Eyebrow Up", "Left Eyebrow Down", "Right Eyebrow Down" };
	for(unsigned int i = 0; i < NUM_FACES; i++)
	{
		char buf[255];
		sprintf(buf, "%s", faceNames[i].c_str());
		controls[i] = ModelerControl(buf, 0.0f, 1.0f, 0.1f, i == 0);

	}

    ModelerApplication::Instance()->Init
	(
		argc, argv,
		controls,
		NUM_FACES
	);

    // Run the modeler application.
    int ret = ModelerApplication::Instance()->Run();

    // This line is reached when you close the program.
    delete ModelerApplication::Instance();

    return ret;
}
