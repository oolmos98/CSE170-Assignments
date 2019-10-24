
# include "my_viewer.h"
# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>
# include <sigogl/ws_run.h>

double seconds = 0.0f;
//References: TA, Jefferson Santiago.
//The code is built from what the TA has helped with during Labs and help by others.
MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	_nbut=0;
	_animating=false;
	build_ui ();
	build_scene ();
	output("Hello there human:) \nPressing Keys:\n\
			'q' = Faces++\n\
			'a' = Faces--\n\
			'w' = r++\n\
			's' = r--\n\
			'e' = R++\n\
			'd' = R--\n\
			SPACE = New Texture");
}
void MyViewer::build_ui ()
{
	UiPanel* p/*, *sp*/;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	//p->add ( new UiButton ( "Shading", sp=new UiPanel() ) );
	//{	UiPanel* p=sp;
	//	p->add ( new UiButton ( "Flat", EvFlat ) ); 
	//	p->add(new UiButton("Smooth", EvSmooth));
	//}
	////p->add ( new UiButton ( "Animate", EvAnimate ) );
	//p->add(new UiButton("Normals", EvNormals));
	//p->add(new UiButton("Color", sp = new UiPanel()));
	//{	UiPanel* p = sp;
	//p->add(new UiButton("Red", EvRed));
	//p->add(new UiButton("Blue", EvBlue));	
	//p->add(new UiButton("Green", EvGreen));
	//p->add(new UiButton("Random", EvRandom));
	//}
	//p->add(new UiButton("Animate", EvAnimate));
	p->add ( new UiButton ( "Exit", EvExit ) ); p->top()->separate();
}
void MyViewer::build_scene()
{
	torus = new SnModel;
	theta = phi = (float)GS_2PI;
	center.remove(0, center.size());
	GsPnt P1, P2, P3, P4;
	int index = 0;
	torus->model()->init();
	int start = index;

	image[0] = "../textures/1.jpg";
	image[1] = "../textures/2.jpg";
	image[2] = "../textures/3.jpg";
	image[3] = "../textures/4.png   ";
	image[4] = "../textures/5.jpg";
	image[5] = "../textures/6.jpg";	
	image[6] = "../textures/7.png";

	float offset = theta / n;
	for (float theta1 = 0.0f; theta1 < theta; theta1 += offset) {

		for (float phi2 = 0.0f; phi2 < phi; phi2 += offset) {
			start = index;
			//Using the equation given from the guidelines.
			P1.set(
				(R + r * cosf(theta1)) * cosf(phi2),
				(R + r * cosf(theta1)) * sinf(phi2),
				r * sinf(theta1)
			);
			torus->model()->V.push(P1);
			index++;
			P2.set(
				(R + r * cosf(theta1 + offset)) * cosf(phi2),
				(R + r * cosf(theta1 + offset)) * sinf(phi2),
				r * sinf(theta1 + offset)
			);
			torus->model()->V.push(P2);
			index++;

			P3.set(
				(R + r * cosf(theta1 + offset)) * cosf(phi2 + offset),
				(R + r * cosf(theta1 + offset)) * sinf(phi2 + offset),
				r * sinf(theta1 + offset)
			);
			torus->model()->V.push(P3);
			index++;
			P4.set(
				(R + r * cosf(theta1)) * cosf(phi2 + offset),  //X
				(R + r * cosf(theta1)) * sinf(phi2 + offset),	//Y
				r * sinf(theta1)	//Z
			);
			torus->model()->V.push(P4);
			index++;

			//Reversed the face coordinates to get the normals to face out not in.
			torus->model()->F.push(GsModel::Face(start, start + 2, start + 1));
			torus->model()->F.push(GsModel::Face(start, start + 3, start + 2));

			//Calculates centers for the smooth shading.
			//With the help of Jefferson Santiago.
			float x0 = (R * cosf(phi2));
			float y0 = (R * sinf(phi2));
			
			center.push(GsPnt(x0, y0, 0.0f));
			center.push(GsPnt(x0, y0, 0.0f));

			//float offset = i + ((float)gs2pi / n);
			x0 = (R * cosf(phi2 + offset));
			y0 = (R * sinf(phi2 + offset));
			center.push(GsPnt(x0, y0, 0.0f));
			center.push(GsPnt(x0, y0, 0.0f));
		}
	}
	torus->color(color);
	rootg()->add(torus);
	shade();
	compute_mappings(texture);
}
void MyViewer::compute_mappings(int index) 
{
	//Code Provided by the Professor in the PA5 Pdf.
	GsModel& m = *torus->model();
	GsModel::Group& g = *m.G.push();
	g.fi = 0;
	g.fn = m.F.size();
	g.dmap = new GsModel::Texture;
	g.dmap->fname.set(image[index]);

	m.M.push().init();
	m.M.top() = GsMaterial();

	int nv = m.V.size();
	m.T.size(nv);
	int k = 0;
	float off = theta / n;
	//Calculates the correct mapping to the image.
	for (float theta1 = 0.0f; theta1 < theta; theta1 += off) {

		for (float phi2 = 0.0f; phi2 < phi; phi2 += off) {
			float u = theta1 / (float)GS_2PI;
			float v = phi2 / (float)GS_2PI;
			float u2 = (theta1 + off) / (float)GS_2PI;
			float v2 = (phi2 + off) / (float)GS_2PI;

			m.T[k++].set(GsVec2(u, v));
			m.T[k++].set(GsVec2(u2, v));
			m.T[k++].set(GsVec2(u2, v2)); 
			m.T[k++].set(GsVec2(u, v2));
		}
	}
	m.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	m.textured = true;
}
void MyViewer::shade() {
	GsVec V, V2;
	torus->model()->N.remove(0, torus->model()->N.size());

	for (int i = 0; i < torus->model()->V.size(); i++) {
		torus->model()->N.push(normalize((torus->model()->V[i] - center[i])));
	}

	render();
}
void MyViewer::compute_segments(bool flat) {
	rootg()->remove(l);
	l = new SnLines;
	l->init();
	l->color(GsColor::white);
		//Provided by the PA4 PDF.
		GsModel& m = *torus->model();
		for (int i = 0; i < m.V.size(); i++) {
			const GsVec& fcenter = m.V[i];
			l->push(fcenter, fcenter + (m.N[i]*0.03f));
		}
		l->line_width(2.0f);
		rootg()->add(l);
		render();
}
int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;
	//GsMat rot;
	//will determine how fast each interaction is. Lower is bigger.
	//float speed = 100;
	switch (e.key)
	{
	case GsEvent::KeyEsc: gs_exit(); return 1;

	case 'q': {
		if (n > 0) {
			n += 2;
			rootg()->remove_all();
			build_scene();
			compute_segments(S);
			l->visible(norm);
			output("Updating");
		}
		return 1;
	}
	case 'a': {
		if (n > 4) {
			n -= 2;
			rootg()->remove_all();
			build_scene();
			compute_segments(S);
			l->visible(norm);
			output("Updating");
		}
		return 1;
	}
	case 'w': {
		r += 0.05f;
		rootg()->remove_all();
		build_scene();
		compute_segments(S);
		l->visible(norm);
		output("Updating");
		return 1;
	}
	case 's': {
		if (r > 0.05f) {
			r -= 0.05f;
			rootg()->remove_all();
			build_scene();
			compute_segments(S);
			l->visible(norm);
			output("Updating");
		}
		return 1;
	}
	case 'e': {
		if (R > 0.0f) {
			R += 0.05f;
			rootg()->remove_all();
			build_scene();
			compute_segments(S);
			l->visible(norm);
			output("Updating");
		}
		return 1;

	}
	case 'd': {
		if (R > 0.1f) {
			R -= 0.05f;
			rootg()->remove_all();
			build_scene();
			compute_segments(S);
			l->visible(norm);
			output("Updating");
		}
		return 1;
	}
	case 'c': {
		norm = true;	
		compute_segments(S);
		l->visible(norm);
		return 1;
	}
	case 'v': {
		norm = false;
		compute_segments(S);
		l->visible(norm);
		return 1;
	}
	case ' ': {
		texture++;
		if (texture == 7) texture = 0;
		rootg()->remove_all();
		build_scene();
		compute_segments(S);
		l->visible(norm);
		//compute_mappings(texture);
		return 1;
	}
	default: output("Hello there human:) \nPressing Keys:\n\
			'q' = Faces++\n\
			'a' = Faces--\n\
			'w' = r++\n\
			's' = r--\n\
			'e' = R++\n\
			'd' = R--\n\
			SPACE = New Texture");
		return 1;
}
	return 0;
}
int MyViewer::uievent ( int e )
{
	switch ( e )
	{
	case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
