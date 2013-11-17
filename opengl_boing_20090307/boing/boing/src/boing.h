/*****************************************************************************
 * $Id: boing.h,v 1.25 2004/04/07 00:56:10 jimb Exp $
 * Author:	Jim E. Brooks  http://www.jimbrooks.org
 *          Original Amiga authors were R.J. Mical and Dale Luck.
 * Desc:	OpenGL port of Amiga Boing.
 * Notes:
 * LEGAL:	COPYRIGHT (C) JIM E. BROOKS 2004
 *          THIS SOURCE CODE IS RELEASED UNDER THE TERMS
 *          OF THE GNU GENERAL PUBLIC LICENSE (GPL).
 *****************************************************************************/

#define BOING_DEBUG	0

typedef vertexEye vertex_t;
void init( void );
void display( void );
void reshape( int w, int h );
void loop( int dontcare );
void keyboard( unsigned char key, int x, int y );
void DrawBoingBall( void );
void BounceBall( void );
void DrawBoingBallBand( GLfloat long_lo,
                        GLfloat long_hi );
void DrawGrid( void );

#define RADIUS			 (70.0)
#define STEP_LONGITUDE	 (22.5)					/* 22.5 makes 8 bands like original Boing */
#define STEP_LATITUDE	 (22.5)

#define DIST_BALL		(RADIUS * 2.0 + RADIUS * 0.1)

#define VIEW_SCENE_DIST	(DIST_BALL * 3.0+ 200.0)/* distance from viewer to middle of boing area */
#define GRID_SIZE		(RADIUS * 4.5)			/* length (width) of grid */
#define BOUNCE_HEIGHT	(RADIUS * 2.1)
#define BOUNCE_WIDTH	(RADIUS * 2.1)

#define SHADOW_OFFSET_X	(-20.0)
#define SHADOW_OFFSET_Y	(10.0)
#define SHADOW_OFFSET_Z (0.0)

#define WALL_L_OFFSET	(0.0)
#define WALL_R_OFFSET	(5.0)

// For full-speed (ie no delay), set this to 1.
#define TIMER_MILSEC	(40)

// Draw ball, or its shadow.
enum draw_e { DRAW_BALL, DRAW_BALL_SHADOW };

// global vars
GLfloat	deg_rot_y		= 0.0;
GLfloat deg_rot_y_inc	= 2.0;
GLfloat ball_x			= -RADIUS;
GLfloat ball_y			= -RADIUS;
GLfloat ball_x_inc		= 1.0;
GLfloat ball_y_inc		= 2.0;
draw_e drawBallHow;
