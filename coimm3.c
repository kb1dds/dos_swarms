/* Simulation of immune simulation (Lab Version) */
#include <graph.h>
#include <math.h>
#include <time.h>
#include "coimmm.c"
#define MP 23 /* normal 25, maximum number of macrophages */
#define BA 50 /* normal 50, maximum number of bacteria */
#define dist( dx, dy ) ( sqrt( dx * dx + dy * dy ) )

main( )
{
   C macp[MP], bact[BA];
   int i, i2, i3, ma, ba, d, dd, f, q, x;
   unsigned t;
   int cacp[MP], cact[BA];
   double dx, dy;
   char k;

   /* Seed Random number generator */
   srand( (unsigned)time( NULL ) );

   /* Set up start values */
   ma = 1;
   t = 0;
   ba = 25;
   f = 1;
   x = 0;

   for( i = 0; i < MP; i ++ )
   {
      cacp[i] = 1;
      initC( &macp[i] );
      if( i < ma )
	 macp[i].act = 1;
      else
	 macp[i].act = 0;
   }

   for( i = 0; i < BA; i ++ )
   {
      cact[i] = 1;
      initC( &bact[i] );
      bact[i].sp = 1;
      if( i < ba )
	 bact[i].act = 1;
      else
	 bact[i].act = 0;
   }

   _setvideomode(_VRES16COLOR);
   while( 1 )
   {
      t ++;
      _settextposition( 0, 0 );
      printf("%u",t);

      i2 = 1;
      for( i = 0; i < BA; i ++ )
	 if( bact[i].act )
	    i2 = 0;

      if( i2 )
      {
	 for( i = 0; i < BA; i ++ )
	 {
	    _setcolor( 0 );
	    _ellipse(_GBORDER,bact[i].x-5,bact[i].y-5,bact[i].x+5,
	       bact[i].y+5 );
	    cact[i] = 1;
	    initC( &bact[i] );
	    bact[i].sp = 1;
	    if( i < ba )
	       bact[i].act = 1;
	    else
	       bact[i].act = 0;
	 }
      }

      if( kbhit() )
      {
	 q = 0;
	 k = getch();
	 switch( k )
	 {
	    case 'd':
	       _clearscreen(_GCLEARSCREEN);
	       x = 0;
	       f = !f;
	       break;
	    case 'p':
	       getch();
	       break;
	    case 'i':
	       t = 0;
	       for( i = 0; i < BA; i ++ )
	       {
		  _setcolor( 0 );
		  _ellipse(_GBORDER,bact[i].x-5,bact[i].y-5,bact[i].x+5,
		     bact[i].y+5 );

		  cact[i] = 1;
		  initC( &bact[i] );
		  bact[i].sp = 1;
		  if( i < ba )
		     bact[i].act = 1;
		  else
		     bact[i].act = 0;
	       }
	       break;
	    default:
	       q = 1;
	       break;
	 }
	 if( q )
	    break;
      }
      for( i = 0; i < MP; i ++ )
      {
	 if( !macp[i].act )
	    continue;

	 if( f )
	 {
	    _setcolor( 0 );
	    _ellipse(_GBORDER,macp[i].x-10,macp[i].y-10,macp[i].x+10,
	       macp[i].y+10 );
	 }

	 if( !cacp[i] )
	 {
	    macp[i].act = 0;
	    cacp[i] = 1;
	    continue;
	 }

	 mvC( &macp[i] );
	 bounceC( &macp[i] );

	 for( i2 = 0; i2 < MP; i2 ++ )
	 {
	    if( i2 == i )
	       continue;

	    if( ( abs( macp[i].x - macp[i2].x ) < 10 ) &&
		( abs( macp[i].y - macp[i2].y ) < 10 ) )
	    {
	       macp[i].x += macp[i].dx;
	       macp[i].y += macp[i].dy;
	    }
	 }

	 if( f )
	 {
	    _setcolor( 2 );
	    _ellipse(_GBORDER,macp[i].x-10,macp[i].y-10,macp[i].x+10,
	       macp[i].y+10 );
	 }

	 for( i2 = 0; i2 < BA; i2 ++ )
	 {
	    if( !bact[i2].act )
	       continue;

	    if( ( abs( macp[i].x - bact[i2].x ) < 10 ) &&
		( abs( macp[i].y - bact[i2].y ) < 10 ) )
	    {
	       copyC( macp, MP, i );
	       macp[i].rep = 0;
	    }
	 }

	 for( i2 = 0; i2 < BA; i2 ++ )
	 {
	    if( ( abs( macp[i].x - bact[i2].x ) < 10 ) &&
		( abs( macp[i].y - bact[i2].y ) < 10 ) )
	    {
	       if( bact[i2].act )
	       {
		  cact[i2] = 0;
		  macp[i].aant = bact[i2].sp;
		  macp[i].rep ++;
	       }
	    }
	 }
	 if( macp[i].rep > 25 )
	    cacp[i] = 0;

	 i3 = -1;
	 dd = 1000;
	 for( i2 = 0; i2 < BA; i2 ++ )
	 {
	    if( !bact[i2].act )
	       continue;

	    dx = bact[i2].x - macp[i].x;
	    dy = bact[i2].y - macp[i].y;

	    d = dist( dx, dy );

	    if( d < dd )
	       i3 = i2, dd = d;
	 }

	 if( i3 != -1 )
	 {
	    dx = bact[i3].x - macp[i].x;
	    dy = bact[i3].y - macp[i].y;

	    dd = dist( dx, dy ) * 1.001;

	    if( dd > 20 )
	       dd = 20;

	    run_xyb( dx, dy, &dx );

	    macp[i].dx = cos( dx ) * dd;
	    macp[i].dy = sin( dx ) * dd;
	 }
      }

      for( i = 0; i < BA; i ++ )
      {
	 if( !bact[i].act )
	    continue;

	 if( f )
	 {
	    _setcolor( 0 );
	    _ellipse(_GBORDER,bact[i].x-5,bact[i].y-5,bact[i].x+5,
	       bact[i].y+5 );
	 }

	 if( !cact[i] )
	 {
	    bact[i].act = 0;
	    cact[i] = 1;
	    continue;
	 }

	 /* The following block of code makes bacteria shake, making
	  * them easier to catch and more realistic */
	 dx = rand()/(RAND_MAX/20);
	 dx -= 10;
	 dx = dx / 5;
	 bact[i].dx += dx;
	 dy = rand()/(RAND_MAX/20);
	 dy -= 10;
	 dy = dy / 5;
	 bact[i].dy += dy;

	 mvC( &bact[i] );
	 bact[i].rep ++;
	 bounceC( &bact[i] );

	 if( f )
	 {
	    _setcolor( 12 );
	    _ellipse(_GBORDER,bact[i].x-5,bact[i].y-5,bact[i].x+5,
	       bact[i].y+5 );
	 }

	 if( bact[i].rep > 0 )
	 {
	    bact[i].rep = 0;
	    copyC( bact, BA, i );
	 }
      }

      if( !f )
      {
	 i2 = 0;
	 for( i = 0; i < BA; i ++ )
	    if( bact[i].act )
	       i2 ++;

	 if( x == 0 )
	    _moveto( x, 480-(i2*7) );
	 else
	 {
	    _setcolor( 10 );
	    _lineto( x, 480-(i2*7) );
	 }
	 x ++;
      }
   }
   _setvideomode(_DEFAULTMODE);
}
