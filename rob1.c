/* Robot Simulation */
#include <graph.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "rob1m.h"

#define O 20
#define R 10

main()
{
   int i, i2;
   obs o[O], o2[O];
   rov r[R], r2[R];
   double sch, oa[O], ta, tx, ty, tx2, ty2;
   int x, y;

   srand( (unsigned)time( NULL ) );

   /* Initialization */

   i2 = rand();
   sch = i2 / ( RAND_MAX / 6283 );
   sch /= 1000;
   ta = sch;

   for( i = 0; i < O; i ++ )
   {
      init_obs( &o[i] );
      o2[i].x = o[i].x;
      o2[i].y = o[i].y;
      o2[i].r = o[i].r;
      i2 = rand();
      sch = i2 / ( RAND_MAX / 6283 );
      sch /= 1000;
      oa[i] = sch;
   }

   for( i = 0; i < R; i ++ )
   {
      init_rov( &r[i] );
      r[i].seq = 0;
   }

   i = rand();
   sch = i / ( RAND_MAX/640 );
   tx = sch;
   i = rand();
   sch = i / ( RAND_MAX/480 );
   ty = sch;

   _setvideomode(_VRES16COLOR);

   while( !kbhit() )
   {
      i2 = rand();
      sch = i2 / ( RAND_MAX / 15708 );
      sch /= 10000;
      sch -= .785;

      ta += sch;

      tx2 = tx;
      ty2 = ty;

      tx += cos( ta ) * 1;
      ty += sin( ta ) * 1;

      if( tx < 0 )
	 tx = 0;
      if( tx > 640 )
	 tx = 640;

      if( ty < 0 )
	 ty = 0;
      if( ty > 480 )
	 ty = 480;

      _setcolor( 0 );
      x = tx2, y = ty2;
      _moveto( x - 5, y );
      _lineto( x + 5, y );
      _moveto( x, y - 5 );
      _lineto( x, y + 5 );

      _setcolor( 9 );
      x = tx, y = ty;
      _moveto( x - 5, y );
      _lineto( x + 5, y );
      _moveto( x, y - 5 );
      _lineto( x, y + 5 );

      for( i = 0; i < R; i ++ )
      {
	 _setcolor( 0 );
	 draw_rov( r2[i] );

	 if( r[i].x < 0 )
	    r[i].x = 0;
	 if( r[i].x > 640 )
	    r[i].x = 640;

	 if( r[i].y < 0 )
	    r[i].y = 0;
	 if( r[i].y > 480 )
	    r[i].y = 480;

	 r[i].xs = tx;
	 r[i].ys = ty;

	 while( r[i].h > 6.28319 )
	    r[i].h -= 6.28319;

	 _setcolor( 3 );
	 draw_rov( r[i] );

	 r2[i].x = r[i].x;
	 r2[i].y = r[i].y;
	 r2[i].h = r[i].h;
	 r2[i].seq = r[i].seq;
	 r2[i].xs = tx;
	 r2[i].ys = ty;
      }

      for( i = 0; i < O; i ++ )
      {
	 _setcolor( 0 );
	 draw_obs( o2[i] );

	 i2 = rand();
	 sch = i2 / ( RAND_MAX / 15708 );
	 sch /= 10000;
	 sch -= .785;

	 oa[i] += sch;

	 o[i].x += cos( oa[i] ) * 0;
	 o[i].y += sin( oa[i] ) * 0;

	 if( o[i].x < 0 )
	    o[i].x = 0;
	 if( o[i].x > 640 )
	    o[i].x = 640;

	 if( o[i].y < 0 )
	    o[i].y = 0;
	 if( o[i].y > 480 )
	    o[i].y = 480;


	 _setcolor( 5 );
	 draw_obs( o[i] );
	 o2[i].x = o[i].x;
	 o2[i].y = o[i].y;
	 o2[i].r = o[i].r;
      }

      for( i = 0; i < R; i ++ )
      {
	 run_rov( &r[i], o, O );

	 if( r[i].seq < 0 )
	 {
	    r[i].seq = 0;
	    i = rand();
	    sch = i / ( RAND_MAX/640 );
	    tx = sch;
	    i = rand();
	    sch = i / ( RAND_MAX/480 );
	    ty = sch;
	 }
      }
   }

   getch();
   _setvideomode(_DEFAULTMODE);
}
