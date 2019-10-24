
# include "my_viewer.h"
# include "sn_mynode.h"

# include <sigogl/gl_tools.h>

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	add_ui ();
	add_mynode (1);
	//redraw();
}

void MyViewer::add_ui ()
{
	UiPanel *p;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( new UiButton ( "Add", EvAdd ) );
	p->add ( mcolorbut=new UiCheckButton ( "Multicolor", EvMultiColor ) );
	p->add ( new UiButton ( "OpenGL", EvInfo ) );
	p->add ( new UiButton ( "Exit", EvExit ) );
}

void MyViewer::add_mynode ( int n )
{
	//PA1 was asked to implement one node.
	SnMyNode* c = new SnMyNode();
	c->color(GsColor::blue);
	rootg()->add(c);
}

int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
		case GsEvent::KeyLeft: gsout<<"Left\n"; return 1;
		// etc

		//Increases # of faces.
		case 'q': 
		{
				((SnMyNode*)rootg()->get(0))->n += 2.0f;
				((SnMyNode*)rootg()->get(0))->touch();
				break;
		}
		//Decreases # of faces.
		case 'a':
		{
			if (((SnMyNode*)rootg()->get(0))->n > 2.0f) {
				((SnMyNode*)rootg()->get(0))->n -= 2.0f;
				((SnMyNode*)rootg()->get(0))->touch();
				break;
			}
		}
		//Increases the r radius.
		case 'w':
		{
			((SnMyNode*)rootg()->get(0))->r += 0.06f;
			((SnMyNode*)rootg()->get(0))->touch();
			break;
		}
		//Decreases the r radius
		case 's':
		{
			((SnMyNode*)rootg()->get(0))->r -= 0.06f;
			((SnMyNode*)rootg()->get(0))->touch();
			break;
		}
		//Increases the R radius
		case 'e':
		{
			((SnMyNode*)rootg()->get(0))->R += 0.06f;
			((SnMyNode*)rootg()->get(0))->touch();
			break;
		}
		//Decreases the R radius
		case 'd':
		{
			((SnMyNode*)rootg()->get(0))->R -= 0.06f;
			((SnMyNode*)rootg()->get(0))->touch();
			break;
		}
		//default: gsout<<"Key pressed: "<<e.key<<gsnl;
	}
	redraw();
	return 0;
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{	//case EvAdd: add_mynode(1); return 1;

		case EvInfo:
		{	if ( output().len()>0 ) { output(""); return 1; }
			output_pos(0,30);
			activate_ogl_context(); // we need an active context
			GsOutput o; o.init(output()); gl_print_info(&o); // print info to viewer
			return 1;
		}

		case EvExit: gs_exit();

		case EvMultiColor: {
			//mcolorbut = true;
			//((SnMyNode*)rootg()->get(0))->touch();
			//redraw();

			return 0;
			//break;
		}
	}
	return WsViewer::uievent(e);
}
