
# include "my_viewer.h"

# include <sigogl/ws_run.h>

int main ( int argc, char** argv )
{
	MyViewer* v = new MyViewer ( -1, -1, 1920, 1080, "Torus Texturing PA" );
	v->cmd ( WsViewer::VCmdAxis );
	v->background(GsColor::darkgray);
	v->view_all ();
	v->show ();
	ws_run();
	return 1;
}