
# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

double seconds = 0.0f;
int stage = 0;
float offset[3];

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	_nbut=0;
	_animating=false;
	build_ui ();
	build_scene ();
	output("Hello there human:) \nPressing Keys:\n\
			'q' = arm up\n\
			'a' = arm down\n\
			'w' = lower arm up\n\
			's' = lower arm down\n\
			'e' = hand up\n\
			'd' = hand down.\n\
			'Space' = animate.");
	//output_color(GsColor::darkred);

}

void MyViewer::build_ui ()
{
	UiPanel* p;// *sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	/*p->add ( new UiButton ( "View", sp=new UiPanel() ) );
	{	UiPanel* p=sp;
		p->add ( _nbut=new UiCheckButton ( "Normals", EvNormals ) ); 
	}*/
	p->add ( new UiButton ( "Animate", EvAnimate ) );
	p->add ( new UiButton ( "Exit", EvExit ) ); p->top()->separate();
}

void MyViewer::add_model ( SnShape* s, GsVec p )
{
	// This method demonstrates how to add some elements to our scene graph: lines,
	// and a shape, and all in a group under a SnManipulator.
	// Therefore we are also demonstrating the use of a manipulator to allow the user to
	// change the position of the object with the mouse. If you do not need mouse interaction,
	// you can just use a SnTransform to apply a transformation instead of a SnManipulator.
	// You would then add the transform as 1st element of the group, and set g->separator(true).
	// Your scene graph should always be carefully designed according to your application needs.

	
	// manip->visible(false); // call this to turn off mouse interaction

}

void MyViewer::build_scene ()
	{
	
	//Creatinf Model array for the 3 objects
	//Arm group of the whole arm.
	SnModel* model[3];
	arm = new SnGroup;
	for (int i = 0; i < 3; i++) {
		model[i] = new SnModel();
		offset[i] = 0;
	}

	//Loading the models from the .m file included.
	model[2]->model()->load("../arm/rhand.m");
	model[1]->model()->load("../arm/rlowerarm.m");
	model[0]->model()->load("../arm/rupperarm.m");

	
	//Group seperator true
	arm->separator(true);
	arm->add(t1 = new SnTransform);
	arm->add(model[0]);
	//Flat upperarm to visualize textured shirt
	model[0]->model()->flat(true);

	arm->add(t2 = new SnTransform);
	arm->add(model[1]);

	//Flat lowerarm to visualize textured shirt
	model[1]->model()->flat(true);

	arm->add(t3 = new SnTransform);
	arm->add(model[2]);
	model[2]->model()->smooth(true);

	//Rearranging the model to fit in its joints.
	GsMat trans, trans2, trans3, rot;
	trans.translation(GsVec(0.0f, 0.0f, 0.0f));
	t1->set(trans);

	trans2.translation(GsVec(0.0f, 0.0f, 29.0f));
	t2->set(trans2);

	trans3.translation(GsVec(0.0f, 0.0f, 26.0f));
	t3->set(trans3);

	//Adding the arm group into root.
	rootg()->add(arm);
	
}

// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation ()
{
	output("ANIMATING!");
	double frames = 1 / 60.0f;
	double t0 = gs_time(), t = 0.0f, tf = 0.0f;
	seconds = 0;
	stage = 0;
	int counter = 0;
	GsMat trans, rot,rot2,rot3;
	do
	{
		if (_animating == false)break;
		while (t - tf < frames) {
			ws_check();
			t = (gs_time() - t0);
		}

		tf = t;
		seconds++;

		//Broken up into stages. 
		if (counter % 60 == 0) {
			if (stage == 0) {
				rot.rotx(-gspi / 140);
				rot2.rotx(-gspi / 180);
				rot3.rotx(-gspi / 240);

				t1->get().mult(t1->get(), rot);
				t2->get().mult(t2->get(), rot2);
				t3->get().mult(t3->get(), rot3);
				//Once it reaches certain amount, next stage.
				if (seconds == 60)stage++;
			}

			if (stage == 1) {
				rot.rotx(gspi / 140);
				rot2.rotx(gspi / 180);
				rot3.rotx(gspi / 240);

				t1->get().mult(t1->get(), rot);
				t2->get().mult(t2->get(), rot2);
				t3->get().mult(t3->get(), rot3);

				if (seconds == 120)stage++;

			}
			if (stage == 2) {
				rot3.rotx(-gspi / 240);
				t3->get().mult(t3->get(), rot3);

				if (seconds == 150)stage++;

			}
			if (stage == 3) {
				rot3.rotx(gspi / 240);
				t3->get().mult(t3->get(), rot3);

				if (seconds == 180)break;

			}
		}

		counter += 1 / 60;
		render();
		ws_check();

	} while (true);

	_animating = false;
	output("Finished Animating!");
}

void MyViewer::show_normals ( bool view )
{
	// Note that primitives are only converted to meshes in GsModel
	// at the first draw call.
	//GsArray<GsVec> fn;
	//SnGroup* r = (SnGroup*)rootg();
	//for ( int k=0; k<r->size(); k++ )
	//{	SnManipulator* manip = r->get<SnManipulator>(k);
	//	SnShape* s = manip->child<SnGroup>()->get<SnShape>(0);
	//	SnLines* l = manip->child<SnGroup>()->get<SnLines>(1);
	//	if ( !view ) { l->visible(false); continue; }
	//	l->visible ( true );
	//	if ( !l->empty() ) continue; // build only once
	//	l->init();
	//	if ( s->instance_name()==SnPrimitive::class_name )
	//	{	GsModel& m = *((SnModel*)s)->model();
	//		m.get_normals_per_face ( fn );
	//		const GsVec* n = fn.pt();
	//		float f = 0.33f;
	//		for ( int i=0; i<m.F.size(); i++ )
	//		{	const GsVec& a=m.V[m.F[i].a]; l->push ( a, a+(*n++)*f );
	//			const GsVec& b=m.V[m.F[i].b]; l->push ( b, b+(*n++)*f );
	//			const GsVec& c=m.V[m.F[i].c]; l->push ( c, c+(*n++)*f );
	//		}
	//	}  
	//}
}

int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;
	GsMat rot;
	//will determine how fast each interaction is. Lower is bigger.
	float speed = 100;
	switch (e.key)
	{
	case GsEvent::KeyEsc: gs_exit(); return 1;
	case 'n': { bool b = !_nbut->value(); _nbut->value(b); show_normals(b); return 1; }

	case ' ': {
			_animating = true;
		run_animation();
		return 1;
	}

	case 'q': {
		if (offset[0] < 1.0f) {
			rot.rotx(-gspi / speed);
			t1->get().mult(t1->get(), rot);
			offset[0] += gspi / speed;
			//gsout << offset[0] << gsnl;
			render();
			ws_check();
			output("Moving arm up!");
		}
		else output("Cant Move Anymore");

		return 1;
	}
	case 'a': {
		if (offset[0] > -1.0f) {
			rot.rotx(gspi / speed);
			t1->get().mult(t1->get(), rot);
			offset[0] -= gspi / speed;
			//gsout << offset[0] << gsnl;
			render();
			output("Moving arm down!");

		}
		else output("Cant Move Anymore");

		return 1;
	}
	case 'w': {
		if (offset[1] < 2.0f) {
			rot.rotx(-gspi / (speed + 40));
			t2->get().mult(t2->get(), rot);
			offset[1] += gspi / (speed + 40);
			//gsout << offset[1] << gsnl;
			render();
			output("Moving lower arm up!");

		}
		else output("Cant Move Anymore");

		return 1;
	}
	case 's': {
		if (offset[1] > 0.0f) {
			rot.rotx(gspi / (speed + 40));
			t2->get().mult(t2->get(), rot);
			offset[1] -= gspi / (speed + 40);
			//gsout << offset[1] << gsnl;
			render();
			output("Moving lower arm down!");

		}
		else output("Cant Move Anymore");

		return 1;
	}
	case 'e': {
		if (offset[2] < 0.5f) {
			rot.rotx(-gspi / (speed + 40));
			t3->get().mult(t3->get(), rot);
			offset[2] += gspi / (speed + 40);
			//gsout << offset[1] << gsnl;
			render();
			output("Moving hand up!");

		}
		else output("Cant Move Anymore");

		return 1;
	}
	case 'd': {
		if (offset[2] > -0.7f) {
			rot.rotx(gspi / 160);
			t3->get().mult(t3->get(), rot);
			offset[2] -= gspi / 160;
			//gsout << offset[1] << gsnl;
			render();
			output("Moving hand down!");

		}
		else output("Cant Move Anymore");

		return 1;
	}



	default: output("Hello there human:) \nPressing Keys:\n\
			'q' = arm up\n\
			'a' = arm down\n\
			'w' = lower arm up\n\
			's' = lower arm down\n\
			'e' = hand up\n\
			'd' = hand down.\n\
			'Space' = animate."); return 1;

	
}

	return 0;
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{	case EvNormals: show_normals(_nbut->value()); return 1;
	case EvAnimate: _animating = true;  run_animation(); return 1;
		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
