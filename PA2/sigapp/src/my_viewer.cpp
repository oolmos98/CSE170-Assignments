
# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

GsModel* cyl, * hand, * hand2;
SnGroup* G1, * G2, * G3,*sG1,*sG2,*sG3;
SnTransform* _t1, * _t2,*_t3,*s_t2,*s_t3,*s_t1;
float seconds = 0.0f;

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	_nbut=0;
	_animating=false;
	build_ui ();
	build_scene ();
}

void MyViewer::build_ui ()
{
	UiPanel *p, *sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( new UiButton ( "View", sp=new UiPanel() ) );
	{	UiPanel* p=sp;
		p->add ( _nbut=new UiCheckButton ( "Normals", EvNormals ) ); 
	}
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


	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	
	// manip->visible(false); // call this to turn off mouse interaction

	rootg()->add(g);
}

void MyViewer::build_scene ()
{
	
	G1 = new SnGroup;
	cyl = new GsModel;
	cyl->make_cylinder(GsPnt(0, 0, 0), GsPnt(0, 0, 0.001f), 0.2f, 0.2f, 30, true);
	G1->separator(true);
	G1->add(_t1 = new SnTransform);
	G1->add(new SnModel(cyl));
	G1->top<SnModel>()->color(GsColor::blue);
	//G1->separator(false);
	cyl = new GsModel;
	cyl->make_cylinder(GsPnt(0, 0, 0.0015f), GsPnt(0, 0, -0.001f), 0.18f, 0.18f, 30, true);
	G1->add(new SnModel(cyl));
	G1->top<SnModel>()->color(GsColor::white);

	G2 = new SnGroup;
	hand = new GsModel;
	hand->make_cylinder(GsPnt(0, 0, 0.01f), GsPnt(0, 0.15f, 0.01f), 0.005f, 0.005f, 30, true);
	G2->separator(true);
	G2->add(_t2 = new SnTransform);
	G2->add(new SnModel(hand));
	G2->top<SnModel>()->color(GsColor::red);
	
	G3 = new SnGroup;
	hand2 = new GsModel;
	hand2->make_cylinder(GsPnt(0, 0, 0.015f), GsPnt(0, 0.1f, 0.015f), 0.005f, 0.005f, 30, true);
	G3->separator(true);
	G3->add(_t3 = new SnTransform);
	G3->add(new SnModel(hand2));
	G3->top<SnModel>()->color(GsColor::orange);
	

	sG1 = new SnGroup;
	cyl = new GsModel;
	cyl->make_cylinder(GsPnt(0, 0, 0), GsPnt(0, 0, 0.001f), 0.2f, 0.2f, 30, true);
	sG1->separator(false);
	sG1->add(s_t1 = new SnTransform);
	sG1->add(new SnModel(cyl));
	sG1->top<SnModel>()->color(GsColor::black);
	//sG1->separator(true);
	cyl = new GsModel;
	cyl->make_cylinder(GsPnt(0, 0, 0.0015f), GsPnt(0, 0, -0.001f), 0.18f, 0.18f, 30, true);
	sG1->add(new SnModel(cyl));
	sG1->top<SnModel>()->color(GsColor::white);


	sG2 = new SnGroup;
	hand = new GsModel;
	hand->make_cylinder(GsPnt(0, 0, 0.01f), GsPnt(0, 0.15f, 0.01f), 0.005f, 0.005f, 30, true);
	sG2->separator(true);
	sG2->add(s_t2 = new SnTransform);
	sG2->add(new SnModel(hand));
	sG2->top<SnModel>()->color(GsColor::black);

	sG3 = new SnGroup;
	hand2 = new GsModel;
	hand2->make_cylinder(GsPnt(0, 0, 0.015f), GsPnt(0, 0.1f, 0.015f), 0.005f, 0.005f, 30, true);
	sG3->separator(true);
	sG3->add(s_t3 = new SnTransform);
	sG3->add(new SnModel(hand2));
	sG3->top<SnModel>()->color(GsColor::black);

	GsMat trans, rot;
	trans.translation(GsVec(0.0f, -0.2f, -0.2f));
	rot.rotx(-gspi / 2);

	s_t1->get().mult(trans, rot);
	s_t2->get().mult(trans, rot);
	s_t3->get().mult(trans, rot);
	


	

	rootg()->add(G1);
	rootg()->add(G2);
	rootg()->add(G3);
	rootg()->add(sG1);
	rootg()->add(sG2);
	rootg()->add(sG3);
	s_t2->get().rot(GsVec::k, (float)GS_2PI);
	s_t3->get().rot(GsVec::k, (float)(GS_2PI));
	
}

// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation ()
{
	
	
	
	double frames = 1 / 60.0f;
	double t0 = gs_time(), t = 0.0f,tf=0.0f;
	
	int counter = 0;
	
	do
	{
		if (_animating == false)break;
		while (t - tf < frames) {
			ws_check();
			t = (gs_time() - t0);
		}

		tf = t;
		seconds++;
			if ((counter % 60) == 0) {
				_t3->get().rot(GsVec::k, (float)((-GS_2PI / 60.0f) * seconds/60));
				s_t3->get().rot(GsVec::k, (float)((-GS_2PI / 60.0f) * seconds / 60));

				//break;
			}
			
				_t2->get().rot(GsVec::k, (float)((-GS_2PI / 60.0f) * seconds));
				s_t2->get().rot(GsVec::k, (float)((-GS_2PI / 60.0f) * seconds));

	
			counter+=1/60;
			render();
			ws_check();
		
	} while (true);
	

}

void MyViewer::show_normals ( bool view )
{
	// Note that primitives are only converted to meshes in GsModel
	// at the first draw call.
	GsArray<GsVec> fn;
	SnGroup* r = (SnGroup*)root();
	for ( int k=0; k<r->size(); k++ )
	{	SnManipulator* manip = r->get<SnManipulator>(k);
		SnShape* s = manip->child<SnGroup>()->get<SnShape>(0);
		SnLines* l = manip->child<SnGroup>()->get<SnLines>(1);
		if ( !view ) { l->visible(false); continue; }
		l->visible ( true );
		if ( !l->empty() ) continue; // build only once
		l->init();
		if ( s->instance_name()==SnPrimitive::class_name )
		{	GsModel& m = *((SnModel*)s)->model();
			m.get_normals_per_face ( fn );
			const GsVec* n = fn.pt();
			float f = 0.33f;
			for ( int i=0; i<m.F.size(); i++ )
			{	const GsVec& a=m.V[m.F[i].a]; l->push ( a, a+(*n++)*f );
				const GsVec& b=m.V[m.F[i].b]; l->push ( b, b+(*n++)*f );
				const GsVec& c=m.V[m.F[i].c]; l->push ( c, c+(*n++)*f );
			}
		}  
	}
}

int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
		case 'n' : { bool b=!_nbut->value(); _nbut->value(b); show_normals(b); return 1; }
		case ' ': { 
			if (_animating == true) {
				_animating = false;
			}
			else {
				_animating = true; 
			}
			run_animation();
			return 1; 
		}
		case GsEvent::KeyEnter: {
			_t2->get().rot(GsVec::k, (float)GS_2PI);
			_t3->get().rot(GsVec::k, (float)(GS_2PI));
			s_t2->get().rot(GsVec::k, (float)GS_2PI);
			s_t3->get().rot(GsVec::k, (float)(GS_2PI));
			seconds = 0.0f;
			render();
			ws_check();
			return 1;
		}
		case 'q':{
			s_t2->get().rot(GsVec::k, (float)GS_2PI);
			s_t3->get().rot(GsVec::k, (float)(GS_2PI));
			GsMat trans;
			trans.translation(GsVec(0.03f, 0, 0.0f));

			s_t1->get().mult(s_t1->get(),trans);
			s_t2->get().mult(s_t2->get(), trans);
			s_t3->get().mult(s_t3->get(), trans);
			render();
			ws_check();
			return 1;
		}
		case 'a': {
			s_t2->get().rot(GsVec::k, (float)GS_2PI);
			s_t3->get().rot(GsVec::k, (float)(GS_2PI));
			GsMat trans;
			trans.translation(GsVec(-0.03f, 0, 0.0f));

			s_t1->get().mult(s_t1->get(), trans);
			s_t2->get().mult(s_t2->get(), trans);
			s_t3->get().mult(s_t3->get(), trans);
			render();
			ws_check();
			return 1; 
		}

		case 'w': {
			s_t2->get().rot(GsVec::k, (float)GS_2PI);
			s_t3->get().rot(GsVec::k, (float)(GS_2PI));
			GsMat trans;
			trans.translation(GsVec(0.0f, 0.0F, 0.03F));

			s_t1->get().mult(s_t1->get(), trans);
			s_t2->get().mult(s_t2->get(), trans);
			s_t3->get().mult(s_t3->get(), trans);
			render();
			ws_check();
			return 1;
	 }
		case 's': {
			s_t2->get().rot(GsVec::k, (float)GS_2PI);
			s_t3->get().rot(GsVec::k, (float)(GS_2PI));
			GsMat trans;
			trans.translation(GsVec(0.0f, 0, -0.03F));

			s_t1->get().mult(s_t1->get(), trans);
			s_t2->get().mult(s_t2->get(), trans);
			s_t3->get().mult(s_t3->get(), trans);
			render();
			ws_check();
			return 1; 
		}
		case 'e': {
			s_t2->get().rot(GsVec::k, (float)GS_2PI);
			s_t3->get().rot(GsVec::k, (float)(GS_2PI));
			GsMat trans;
			trans.translation(GsVec(0.0f, 0.03F,0));

			s_t1->get().mult(s_t1->get(), trans);
			s_t2->get().mult(s_t2->get(), trans);
			s_t3->get().mult(s_t3->get(), trans);
			render();
			ws_check();
			return 1; 
		}
		case 'd': {
			s_t2->get().rot(GsVec::k, (float)GS_2PI);
			s_t3->get().rot(GsVec::k, (float)(GS_2PI));
			GsMat trans;
			trans.translation(GsVec(0.0f, -0.03F, 0));

			s_t1->get().mult(s_t1->get(), trans);
			s_t2->get().mult(s_t2->get(), trans);
			s_t3->get().mult(s_t3->get(), trans);
			render();
			ws_check();
			return 1;
		}

		default: gsout<<"Key pressed: "<<e.key<<gsnl;
	}

	return 0;
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{	case EvNormals: show_normals(_nbut->value()); return 1;
	case EvAnimate: _animating = true; run_animation();  return 1;
		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
