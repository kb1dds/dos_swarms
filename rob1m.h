/* Robot Rover Header */
#define dist(dx,dy) sqrt( ( dx ) * ( dx ) + ( dy ) * ( dy ) )
#define PIQ 10 /* Number of instructions in primitive queue */

/* Types and structures */
/* Rover */
typedef struct
{
   double x, y;    /* (x,y) position of rover */
   double h;       /* heading of rover (radians) */
   int seq;        /* Sequence code (see below) */
   double v, z, e; /* Sequencer storage */
   double xs, ys;  /* Destination Position */
   double m;       /* Movement drive at a time */
   double hm;      /* Rotational Movement */
   int col;        /* Object collided with */
   int cf;         /* Collision Flag (Use with move_to) */
   int piq[PIQ];   /* PrImitive Queue codes */
   double p1[PIQ]; /* PIQ parameter 1 */
   double p2[PIQ]; /* PIQ parameter 2 */
}rov;

/* Obstacle */
typedef struct
{
   double x, y; /* (x,y) of obstacle */
   double r;    /* Radius of obstacle */
}obs;

/* Sequence codes:
 * -1 = system is standing by
 * 0 = normal mode, move toward destination
 * 1 = collision with obstacle, angle of collision in v,
 * 2 = adjusting angle following collision (angle to turn to turn = z)
 * 3 = reverse 1 unit (dist travelled = z)
 *
 * codes 1 - 3 are resetting course after collision, after completion of
 * code 3, return to 0
 */

/* Function Prototypes */

/* Initialize a rover */
init_rov( rov *r );

/* Initialize an obstacle */
init_obs( obs *o );

/* Draw Rover */
draw_rov( rov r );

/* Draw obstacle */
draw_obs( obs o );

/* Draw Crosshairs */
draw_cross( int x, int y );

/* Drive Rover */
int run_rov( rov *r, obs o[], int n );

/* Drive Rover forward */
int d_f( rov *r );

/* Drive rover bacward */
int d_r( rov *r );

/* Turn rover angle, to meet angle <a> */
int rot( rov *r, double a );

/* Move Rover to coords defined by r.xs, r.ys
 *
 * If collision occurs:
 * Set r.e to size of object collided with, 1 if unknown
 * Set r.cf = 1 to indicate collision
 */
int move_to( rov *r );

/* Run 3 level architecture in robot <r> */
int rob( rov *r );

/* Move a queue <q> one element forward, <q> has <n> elements */
int push_qi( int q[], int n );
int push_qd( double q[], int n );

/* add a value <v> to end of queue <q> that has <n> elements */
int queue_i( int q[], int n, int v );
int queue_d( double q[], int n, double v );

/* Load queue int <fl> into robot */
int load_q( rov *r, char *fl );
