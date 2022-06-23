/* Robot Module */
#include <graph.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "rob1m.h"

/* Initialize a rover */
init_rov( rov *r )
{
   double sch;
   int i;

   i = rand();
   sch = i / ( RAND_MAX / 640 );
   r->x = sch;

   i = rand();
   sch = i / ( RAND_MAX / 480 );
   r->y = sch;

   r->h = 0;
   r->seq = -1;
   r->v = 0;
   r->z = 0;
   r->m = 2;
   r->hm = .2;
   r->col = 0;
   r->cf = 0;
   r->xs = 0, r->ys = 0;

   /* Clear PIQ */
   for( i = 0; i < PIQ; i ++ )
      r->piq[i] = 0, r->p1[i] = 0, r->p2[i] = 0;
}

/* Initialize an obstacle */
init_obs( obs *o )
{
   double sch;
   int i;

   i = rand();
   sch = i / ( RAND_MAX / 640 );
   o->x = sch;

   i = rand();
   sch = i / ( RAND_MAX / 480 );
   o->y = sch;

   i = rand();
   sch = i / ( RAND_MAX / 20 );
   o->r = sch;
}

/* Draw Rover */
draw_rov( rov r )
{
   int x1, x2, y1, y2;

   x1 = r.x - 2;
   y1 = r.y - 2;
   x2 = r.x + 2;
   y2 = r.y + 2;

   _rectangle( _GBORDER, x1, y1, x2, y2 );

   x1 = r.x;
   y1 = r.y;
   _moveto( x1, y1 );
   x1 += cos( r.h ) * 10;
   y1 += sin( r.h ) * 10;
   _lineto( x1, y1 );

   /*if( r.seq >= 0 )
   {
      x1 = r.xs;
      y1 = r.ys;
      draw_cross( x1, y1 );
   }*/
}

int rob( rov *r )
{
   int i, i2;

   if( r->piq[0] == 0 )
      return( 1 );

   switch( r->piq[0] )
   {
      /* No instruction */
      case 0:
	 return( 1 );

      /* move_to */
      case 1:
	 r->seq = 0;
	 r->xs = r->p1[0], r->ys = r->p2[0];

	 move_to( r );
	 break;

      /* move_f */
      case 2:
	 r->seq = -1;
	 d_f( r );
	 break;

      /* move_b */
      case 3:
	 r->seq = -1;
	 d_r( r );
	 break;

      /* rot */
      case 4:
	 r->seq = 0;
	 i = rot( r, r->p1[0] );
	 if( i )
	    r->seq = -1;
	 break;
   }

   if( r->seq == -1 )
   {
      push_qi( r->piq, PIQ );
      push_qd( r->p1, PIQ );
      push_qd( r->p2, PIQ );
   }
}

int queue_i( int q[], int n, int v )
{
   int i;

   for( i = 0; i < n; i ++ )
   {
      if( q[i] == 0 )
      {
	 q[i] = v;
	 break;
      }
   }
}

int queue_d( double q[], int n, double v )
{
   int i;

   for( i = 0; i < n; i ++ )
   {
      if( q[i] == 0 )
      {
	 q[i] = v;
	 break;
      }
   }
}

int push_qi( int q[], int n )
{
   int i;

   for( i = 1; i < n; i ++ )
      q[i-1] = q[i];
}

int push_qd( double q[], int n )
{
   int i;

   for( i = 1; i < n; i ++ )
      q[i-1] = q[i];
}

int load_q( rov *r, char *fl )
{
   FILE *fp;
   int i, e = 0;
   char com[80], s;
   double schx, schy;

   if( ( fp = fopen( fl, "r" ) ) == NULL )
   {
      return( 1 );
   }

   while( fgets( com, 80, fp ) != NULL )
   {
      /* move_to command */
      if( !strncmp( com, "move_to", 7 ) )
      {
	 s = 1;

	 i = sscanf( com, "move_to %lf,%lf", &schx, &schy );

	 if( i < 2 )
	 {
	    e = 2;
	    break;
	 }
	 else
	 {
	    queue_i( r->piq, PIQ, 1 );
	    queue_d( r->p1, PIQ, schx );
	    queue_d( r->p2, PIQ, schy );
	 }
      }
      if( !strcmp( com, "move_f" ) )
      {
	 s = 1;

	 queue_i( r->piq, PIQ, 2 );
      }
      if( !strcmp( com, "move_b" ) )
      {
	 s = 1;

	 queue_i( r->piq, PIQ, 3 );
      }
      /* rot command */
      if( !strncmp( com, "rot", 3 ) )
      {
	 s = 1;

	 i = sscanf( com, "rot %lf", &schx );

	 if( i < 1 )
	 {
	    e = 2;
	    break;
	 }
	 else
	 {
	    queue_i( r->piq, PIQ, 1 );
	    queue_d( r->p1, PIQ, schx );
	 }
      }
      if( !s )
      {
	 e = 2;
	 break;
      }
   }

   fclose( fp );
   return( e );
}

/* Draw obstacle */
draw_obs( obs o )
{
   int x1, x2, y1, y2;

   x1 = o.x - o.r;
   y1 = o.y - o.r;
   x2 = o.x + o.r;
   y2 = o.y + o.r;

   _ellipse( _GBORDER, x1, y1, x2, y2 );
}

/* Draw crosshairs */
draw_cross( int x, int y )
{
   _moveto( x - 5, y );
   _lineto( x + 5, y );

   _moveto( x, y - 5 );
   _lineto( x, y + 5 );
}

int run_rov( rov *r, obs o[], int n )
{
   int i, i2;
   double sch, sch2;

   switch( r->seq )
   {
      case 0:
	 sch = dist( r->x - r->xs, r->y - r->ys );

	 if( sch > r->m )
	 {
	    run_xyb( r->xs - r->x, r->ys - r->y, &sch );

	    i = rot( r, sch );
	    if( i )
	       d_f( r );
	 }
	 else
	 {
	    r->seq = -1;
	 }
	 break;
      case 1:
	 r->seq ++;
	 sch = r->h;
	 if( sch > 3.1415 )
	    sch -= 6.28319;
	 sch2 = r->v;
	 if( sch2 > 3.1415 )
	    sch2 -= 6.28319;

	 sch = sch2 - sch;

	 if( sch < 0 )
	    r->z = r->h - 1;
	 else
	    r->z = r->h + 1;

	 if( r->z < 0 )
	    r->z += 6.28319;
	 if( r->z > 6.28319 )
	    r->z -= 6.28319;
	 break;
      case 2:
	 i = rot( r, r->z );
	 i2 = r->col;

	 if( i )
	 {
	    r->seq ++, r->z = o[i2].r;
	    break;
	 }

	 /*sch = dist( r->x - o[i2].x, r->y - o[i2].y );

	 if( sch > ( 2 * o[i2].r ) )
	    r->seq = 0;*/
	 break;
      case 3:
	 d_r( r );
	 i = r->col;
	 sch = dist( r->x - o[i].x, r->y - o[i].y );

	 if( sch > ( 2 * r->z ) )
	    r->seq = 0;
	 break;
   }

   if( r->seq <= 0 )
   {
      for( i = 0; i < n; i ++ )
      {
	 sch = dist( r->x - o[i].x, r->y - o[i].y );
	 sch -= 2;

	 if( sch < o[i].r )
	 {
	    r->seq = 1;
	    run_xyb( o[i].x - r->x, o[i].y - r->y, &sch );
	    r->v = sch;
	    r->col = i;
	 }
      }
   }
}

/* Move Rover to coords defined by r.xs, r.ys
 *
 * If collision occurs:
 * Set r.e to size of object collided with, 1 if unknown
 * Set r.cf = 1 to indicate collision
 */

int move_to( rov *r )
{
   int i, i2;
   double sch, sch2;

   if( r->cf )
   {
      r->seq = 1;
   }

   switch( r->seq )
   {
      case 0:
	 sch = dist( r->x - r->xs, r->y - r->ys );

	 if( sch > r->m )
	 {
	    run_xyb( r->xs - r->x, r->ys - r->y, &sch );

	    i = rot( r, sch );
	    if( i )
	       d_f( r );
	 }
	 else
	 {
	    r->seq = -1;
	 }
	 break;
      case 1:
	 r->seq ++;
	 /*sch = r->h;
	 if( sch > 3.1415 )
	    sch -= 6.28319;
	 sch2 = r->v;
	 if( sch2 > 3.1415 )
	    sch2 -= 6.28319;

	 sch = sch2 - sch;

	 if( sch < 0 )
	    r->z = r->h - 1;
	 else*/

	 r->z = r->h + 1;

	 if( r->z < 0 )
	    r->z += 6.28319;
	 if( r->z > 6.28319 )
	    r->z -= 6.28319;
	 break;
      case 2:
	 i = rot( r, r->z );

	 if( i )
	 {
	    r->seq ++;
	    r->v = 0;
	    break;
	 }

	 break;
      case 3:
	 d_r( r );
	 r->v += 2;
	 sch = r->v;

	 if( sch > ( 2 * r->e ) )
	    r->seq = 0;
	 break;
   }
}


int d_f( rov *r )
{
   r->x += cos( r->h ) * r->m;
   r->y += sin( r->h ) * r->m;
}

int d_r( rov *r )
{
   r->x += cos( r->h ) * ( - r->m );
   r->y += sin( r->h ) * ( - r->m );
}

int rot( rov *r, double a )
{
   double sch, aa;

   sch = r->h;
   if( sch > 3.1415 )
      sch -= 6.28319;
   aa = a;
   if( aa > 3.1415 )
      aa -= 6.28319;

   aa = sch - aa;

   if( fabs( aa ) > r->hm )
   {
      aa = fabs( r->h - a );
      if( aa > 6.28319 )
	 aa -= 6.28319;

      if( aa < 3.1415 )
      {
	 if( r->h > a )
	 {
	    r->h -= r->hm;
	 }
	 else
	 {
	    r->h += r->hm;
	 }
      }
      else
      {
	 if( r->h > a )
	 {
	    r->h += r->hm;
	 }
	 else
	 {
	    r->h -= r->hm;
	 }
      }
   }
   else
   {
      return( 1 );
   }

   if( r->h < 0 )
      r->h += 6.28319;
   if( r->h > 6.28319 )
      r->h -= 6.28319;

   return( 0 );
}
