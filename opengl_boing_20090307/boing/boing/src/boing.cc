/*****************************************************************************
 * $Id: boing.cc,v 1.47 2004/04/07 00:56:10 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 *          Original Amiga authors were R.J. Mical and Dale Luck.
 * Desc:	OpenGL port of Amiga Boing.
 * Notes:	- 360' = 2*PI [radian]
 *          - Distances between objects are created by doing a relative
 *            Z translations.
 *          - Although OpenGL enticingly supports alpha-blending,
 *            the shadow of the original Boing didn't affect the color
 *            of the grid.
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#include "gfx.h"
#include "boing.h"

/*****************************************************************************
 * main.
 *****************************************************************************/
int
main( int argc, char** argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
	glutInitWindowSize( 400, 400 );
	glutCreateWindow( "OpenGL Amiga Boing" );
	init();
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutDisplayFunc( display );
	glutTimerFunc( TIMER_MILSEC, loop, 0 );
	glutMainLoop();
	return 0;
}

/*****************************************************************************
 * Initialize.
 *****************************************************************************/
void
init( void )
{
	glClearColor( RGB_GRAY, 0.0 );
	glShadeModel( GL_FLAT );
}

/*****************************************************************************
 * OpenGL display callback.
 *****************************************************************************/
void
display(void)
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();

	drawBallHow = DRAW_BALL_SHADOW;
	DrawBoingBall();

	DrawGrid();

	drawBallHow = DRAW_BALL;
	DrawBoingBall();

	glPopMatrix();
	glFlush();
	glutSwapBuffers();

	int error = glGetError();
	static int errorCnt = 0;
	if ( error != GL_NO_ERROR  &&  errorCnt++ < 20 ) // eventually stop
		ErrorMsg( "glGetError() = %d \n", error );   // printing error msgs

	return;
}

/*****************************************************************************
 * OpenGL reshape callback.
 *****************************************************************************/
void
reshape( int w, int h )
{
	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( PerspectiveAngle( RADIUS * 2, 200 ),
                    (GLfloat)w / (GLfloat)h,
                    1.0,
                    VIEW_SCENE_DIST );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( 0.0,  0.0, VIEW_SCENE_DIST,	// eye
               0.0,  0.0, 0.0,				// center of vision
               0.0, -1.0, 0.0 );			// up vector
	return;
}

/*****************************************************************************
 * OpenGL loop.
 *****************************************************************************/
void
loop( UNUSED int dontcare )
{
	// Animate the boing ball.
	display();
	glutPostRedisplay();

	// Re-activate timer (GLUT timer is single-shot).
	glutTimerFunc( TIMER_MILSEC, loop, 0 );

	return;
}

/*****************************************************************************
 * Keyboard handler.
 *****************************************************************************/
void
keyboard( unsigned char key, UNUSED int x, UNUSED int y )
{
	switch ( key )
	{
		case 'q': case KEY_ESC:
		exit( 0 );

		default:
		break;
	}
	return;
}

/*****************************************************************************
 * Draw the Boing ball.
 * The Boing ball is sphere in which each facet is a rectangle.
 * Facet colors alternate between red and white.
 * The ball is built by stacking latitudinal circles.  Each circle is composed
 * of a widely-separated set of points, so that each facet is noticably large.
 *****************************************************************************/
void
DrawBoingBall( void )
{
	GLfloat lon_deg; // degree of longitude

	glPushMatrix();
	glMatrixMode( GL_MODELVIEW );

	// Another relative Z translation to separate objects.
	glTranslatef( 0.0, 0.0, DIST_BALL );
	BounceBall();

	// Tilt the ball.
	glRotatef( -20.0, 0.0, 0.0, 1.0 );

	// Continually rotate ball around Y axis.
	glRotatef( deg_rot_y, 0.0, 1.0, 0.0 );
	deg_rot_y = TruncateDeg( deg_rot_y + deg_rot_y_inc );

	// Set OpenGL state for Boing ball.
	glCullFace( GL_FRONT );
	glEnable( GL_CULL_FACE );
	glEnable( GL_NORMALIZE );

	// Build a faceted latitude slice of the Boing ball,
	// stepping same-sized vertical bands of the sphere.
	for ( lon_deg = 0;
          lon_deg < 180;
          lon_deg += STEP_LONGITUDE )
	{
		// Draw a latitude circle at this longitude.
		DrawBoingBallBand( lon_deg, lon_deg + STEP_LONGITUDE );
	}

	glPopMatrix();
	return;
}

/*****************************************************************************
 * Bounce the ball.
 *****************************************************************************/
void
BounceBall( void )
{
	GLfloat sign;
	GLfloat deg;

	if ( ball_x >  (BOUNCE_WIDTH/2 + WALL_R_OFFSET ) )
	{
		ball_x_inc = -0.5 - 0.75 * random_f();
		deg_rot_y_inc = -deg_rot_y_inc;
	}
	if ( ball_x < -(BOUNCE_HEIGHT/2 + WALL_L_OFFSET) )
	{
		ball_x_inc =  0.5 + 0.75 * random_f();
		deg_rot_y_inc = -deg_rot_y_inc;
	}
	ball_x += ball_x_inc;
	glTranslatef( ball_x, 0.0, 0.0 );

	if ( ball_y >  BOUNCE_HEIGHT/2      ) ball_y_inc = -0.75 - 1.0 * random_f();
	if ( ball_y < -BOUNCE_HEIGHT/2*0.85 ) ball_y_inc =  0.75 + 1.0 * random_f();
	ball_y += ball_y_inc;
	glTranslatef( 0.0, ball_y, 0.0 );

	// Simulate the effects of gravity on Y movement.
	if ( ball_y_inc < 0 ) sign = -1.0; else sign = 1.0;
	deg = (ball_y + BOUNCE_HEIGHT/2) * 90 / BOUNCE_HEIGHT;
	if ( deg > 80 ) deg = 80;
	if ( deg < 10 ) deg = 10;
	ball_y_inc = sign * 4.0 * sin_deg( deg );

	// Offset the shadow.
	if ( drawBallHow == DRAW_BALL_SHADOW )
	{
		glTranslatef( SHADOW_OFFSET_X,
                      SHADOW_OFFSET_Y,
                      SHADOW_OFFSET_Z );
	}
	return;
}

/*****************************************************************************
 * Draw a faceted latitude band of the Boing ball.
 * Parms: long_lo, long_hi
 *        Low and high longitudes of slice, resp.
 *****************************************************************************/
void
DrawBoingBallBand( GLfloat long_lo,
                   GLfloat long_hi )
{
	vertex_t		vert_ne;		// "ne" means south-east and so on
	vertex_t		vert_nw;
	vertex_t		vert_sw;
	vertex_t		vert_se;
	vertexNormal	vert_norm;
	GLfloat			lat_deg;
	static int		colorToggle = 0;

	// Iterate thru the points of a latitude circle.
	// A latitude circle is a 2D set of X,Z points.
	for ( lat_deg = 0;
          lat_deg <= (360 - STEP_LATITUDE);
          lat_deg += STEP_LATITUDE )
	{
		// Color this polygon with red or white.
		if ( colorToggle )
			glColor3f( RGB_RED );
		else
			glColor3f( RGB_WHITE );

		colorToggle = ! colorToggle;

		// Change color if drawing shadow.
		if ( drawBallHow == DRAW_BALL_SHADOW )
			glColor3f( RGB_SHADOW );

		// Assign each Y.
		vert_ne.y = vert_nw.y = cos_deg(long_hi) * RADIUS;
		vert_sw.y = vert_se.y = cos_deg(long_lo) * RADIUS;

		// Assign each X,Z with sin,cos values scaled by latitude radius indexed by longitude.
		// Eg, long=0 and long=180 are at the poles, so zero scale is sin(longitude),
		// while long=90 (sin(90)=1) is at equator.
		vert_ne.x = cos_deg( lat_deg                 ) * (RADIUS * sin_deg( long_lo + STEP_LONGITUDE ));
		vert_se.x = cos_deg( lat_deg                 ) * (RADIUS * sin_deg( long_lo                  ));
		vert_nw.x = cos_deg( lat_deg + STEP_LATITUDE ) * (RADIUS * sin_deg( long_lo + STEP_LONGITUDE ));
		vert_sw.x = cos_deg( lat_deg + STEP_LATITUDE ) * (RADIUS * sin_deg( long_lo                  ));

		vert_ne.z = sin_deg( lat_deg                 ) * (RADIUS * sin_deg( long_lo + STEP_LONGITUDE ));
		vert_se.z = sin_deg( lat_deg                 ) * (RADIUS * sin_deg( long_lo                  ));
		vert_nw.z = sin_deg( lat_deg + STEP_LATITUDE ) * (RADIUS * sin_deg( long_lo + STEP_LONGITUDE ));
		vert_sw.z = sin_deg( lat_deg + STEP_LATITUDE ) * (RADIUS * sin_deg( long_lo                  ));

		// Draw the facet.
		glBegin( GL_POLYGON );

		CrossProduct( &vert_norm, &vert_ne, &vert_nw, &vert_sw );
		glNormal3f( vert_norm.x, vert_norm.y, vert_norm.z );

		glVertex3f( vert_ne.x, vert_ne.y, vert_ne.z );
		glVertex3f( vert_nw.x, vert_nw.y, vert_nw.z );
		glVertex3f( vert_sw.x, vert_sw.y, vert_sw.z );
		glVertex3f( vert_se.x, vert_se.y, vert_se.z );

		glEnd();

#if BOING_DEBUG
		Msg( "----------------------------------------------------------- \n" );
		Msg( "lat = %f  long_lo = %f  long_hi = %f  \n", lat_deg, long_lo, long_hi );
		Msg( "vert_ne  x = %.8f  y = %.8f  z = %.8f \n", vert_ne.x, vert_ne.y, vert_ne.z );
		Msg( "vert_nw  x = %.8f  y = %.8f  z = %.8f \n", vert_nw.x, vert_nw.y, vert_nw.z );
		Msg( "vert_se  x = %.8f  y = %.8f  z = %.8f \n", vert_se.x, vert_se.y, vert_se.z );
		Msg( "vert_sw  x = %.8f  y = %.8f  z = %.8f \n", vert_sw.x, vert_sw.y, vert_sw.z );
#endif
   }

	// Toggle color so that next band will opposite red/white colors than this one.
   colorToggle = ! colorToggle;

	// This circular band is done.
   return;
}

/*****************************************************************************
 * Draw the purple grid of lines, behind the Boing ball.
 * When the Workbench is dropped to the bottom, Boing shows 12 rows.
 *****************************************************************************/
void
DrawGrid( void )
{
	int				row, col;
	const int		rowTotal	= 12;					/* must be divisible by 2 */
	const int		colTotal	= rowTotal;				/* must be same as rowTotal */
	const GLfloat	widthLine	= 2.0;					/* should be divisible by 2 */
	const GLfloat	sizeCell	= GRID_SIZE / rowTotal;
	const GLfloat	z_offset	= -40.0;
	GLfloat			xl, xr;
	GLfloat			yt, yb;

	glPushMatrix();
	glDisable( GL_CULL_FACE );

	// Another relative Z translation to separate objects.
	glTranslatef( 0.0, 0.0, DIST_BALL );

	// Draw vertical lines (as skinny 3D rectangles).
	for ( col = 0; col <= colTotal; col++ )
	{
		// Compute co-ords of line.
		xl = -GRID_SIZE / 2 + col * sizeCell;
		xr = xl + widthLine;

		yt =  GRID_SIZE / 2;
		yb = -GRID_SIZE / 2 - widthLine;

		glBegin( GL_POLYGON );

		glColor3f( RGB_PURPLE );			// purple

		glVertex3f( xr, yt, z_offset );		// NE
		glVertex3f( xl, yt, z_offset );		// NW
		glVertex3f( xl, yb, z_offset );		// SW
		glVertex3f( xr, yb, z_offset );		// SE

		glEnd();
	}

	// Draw horizontal lines (as skinny 3D rectangles).
	for ( row = 0; row <= rowTotal; row++ )
	{
		// Compute co-ords of line.
		yt = GRID_SIZE / 2 - row * sizeCell;
		yb = yt - widthLine;

		xl = -GRID_SIZE / 2;
		xr =  GRID_SIZE / 2 + widthLine;

		glBegin( GL_POLYGON );

		glColor3f( RGB_PURPLE );			// purple

		glVertex3f( xr, yt, z_offset );		// NE
		glVertex3f( xl, yt, z_offset );		// NW
		glVertex3f( xl, yb, z_offset );		// SW
		glVertex3f( xr, yb, z_offset );		// SE

		glEnd();
	}

	glPopMatrix();
	return;
}
