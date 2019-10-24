# include "sn_mynode.h"

const char* SnMyNode::class_name = "SnMyNode"; // static
SN_SHAPE_RENDERER_DEFINITIONS(SnMyNode);

//===== SnMyNode =====

SnMyNode::SnMyNode () : SnShape ( class_name )
{
	//Variables needed for the torus.
	r = 0.2f;
	R = 0.8f;
	n = 14.0f;
	theta = phi = gs2pi;
	multicolor = true;
	if ( !SnMyNode::renderer_instantiator ) SnMyNodeRegisterRenderer();
}

SnMyNode::~SnMyNode ()
{
}

void SnMyNode::get_bounding_box ( GsBox& b ) const
{
	// the bounding box is needed for camera view_all computations
	b.set ( init, init+GsVec(width,height,0) );
}

//===== Renderer Instantiator =====

# include "glr_mynode.h"

static SnShapeRenderer* GlrMyNodeInstantiator()
{
	return new GlrMyNode;
}

void SnMyNodeRegisterRenderer ()
{
	SnMyNode::renderer_instantiator = &GlrMyNodeInstantiator;
}
