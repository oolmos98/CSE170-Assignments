# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>
# include <sig/sn_lines.h>

# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>
#include <sig/sn_model.h>

// Viewer for this example:
class MyViewer : public WsViewer
{  protected :
	//Torus Values.
	SnModel* torus;
	float r = 0.08f, R = 0.25f, theta, phi;
	int n = 24;
	//Center for the normal calculations.
	GsArray<GsPnt> center;
	//Determines the type of shading
	bool S = false;
	//For the lines 
	SnLines* l;
	bool norm = false;
	GsColor color = GsColor::darkred;
	int size = 7;
	char* image[7];
	int texture = 0;



	enum MenuEv { EvNormals, EvAnimate, EvExit, EvFlat, EvSmooth, EvRed, EvBlue, EvGreen, EvRandom };
	UiCheckButton* _nbut;
	bool _animating;
   public :
	MyViewer ( int x, int y, int w, int h, const char* l );
	void build_ui ();
	//void add_model ( SnShape* s, GsVec p );
	void build_scene ();
	//void show_normals ( bool view );
	void compute_mappings(int);
	void run_animation ();
	void shade();
	void compute_segments(bool flat);
	virtual int handle_keyboard ( const GsEvent &e ) override;
	virtual int uievent ( int e ) override;
};

