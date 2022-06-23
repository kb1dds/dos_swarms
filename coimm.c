/* Simulation of immune simulation */
#include <graph.h>
#include <math.h>
#include <time.h>
#define MAB 50
#define HT 50
#define MP 25
#define BA 50
#define BC 20
#define AB 50

#include "coimmm.c"
#define dist( dx, dy ) ( sqrt( dx * dx + dy * dy ) )

main()
{
   C tcell[HT], macp[MP], bact[BA], bcell[BC];
   int i, i2, i3, ha, ma, ba, bstart, d, dd, f, q, x;
   int caht[HT], cacp[MP], cact[BA];
   int abx[AB], aby[AB], ab, agb[AB];
   double dx, dy;
   char k;

   /* Seed Random number generator */
   srand( (unsigned)time( NULL ) );

   /* Set up start values */
   ha = 10;
   ma = 5;
   ab = 0;
   bstart = 10;
   ba = 10;
   f = 1;
   x = 0;

   for( i = 0; i < AB; i ++ )
      abx[i] = 0, aby[i] = 0, agb[i] = 0;

   for( i = 0; i < HT; i ++ )
   {
      initC( &tcell[i] );
      caht[i] = 1;
      if( i < ha )
	 tcell[i].act = 1;
      else
	 tcell[i].act = 0;
   }

   for( i = 0; i < BC; i ++ )
   {
      initC( &bcell[i] );
      if( i < bstart )
	 bcell[i].act = 1;
      else
	 bcell[i].act = 0;
   }

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
	    case 'i':
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
      for( i = 0; i < AB; i ++ )
      {
	 if( ( abx[i] == 0 ) && ( aby[i] == 0 ) )
	    agb[i] = 0;

	 if( agb[i] != 0 )
	 {
	    _setcolor( 0 );
	    _ellipse(_GBORDER, abx[i]-agb[i], aby[i]-agb[i], abx[i]+agb[i],
	       aby[i]+agb[i] );
	    agb[i] += 5;
	    _setcolor( 5 );
	    _ellipse(_GBORDER, abx[i]-agb[i], aby[i]-agb[i], abx[i]+agb[i],
	       aby[i]+agb[i] );
	 }
	 if( agb[i] > MAB )
	 {
	    _setcolor( 0 );
	    _ellipse(_GBORDER, abx[i]-agb[i], aby[i]-agb[i], abx[i]+agb[i],
	       aby[i]+agb[i] );
	    agb[i] = 0;
	 }
      }
      for( i = 0; i < HT; i ++ )
      {
	 if( !tcell[i].act )
	    continue;

	 if( f )
	 {
	    _setcolor( 0 );
	    _ellipse(_GBORDER,tcell[i].x-5,tcell[i].y-5,tcell[i].x+5,
	       tcell[i].y+5 );
	 }

	 if( !caht[i] )
	 {
	    tcell[i].act = 0;
	    caht[i] = 1;
	    continue;
	 }

	 mvC( &tcell[i] );
	 bounceC( &tcell[i] );
	 if( f )
	 {
	    _setcolor( 1 );
	    _ellipse(_GBORDER,tcell[i].x-5,tcell[i].y-5,tcell[i].x+5,
	       tcell[i].y+5 );
	 }

	 for( i2 = 0; i2 < BA; i2 ++ )
	 {
	    if( ( abs( tcell[i].x - bact[i2].x ) < 10 ) &&
		( abs( tcell[i].y - bact[i2].y ) < 10 ) )
	    {
	       if( bact[i2].act )
	       {
		  copyC( tcell, HT, i );
		  tcell[i].aant = bact[i2].sp;
	       }
	    }
	 }
	 for( i2 = 0; i2 < MP; i2 ++ )
	 {
	    if( ( abs( tcell[i].x - macp[i2].x ) < 10 ) &&
		( abs( tcell[i].y - macp[i2].y ) < 10 ) )
	    {
	       if( macp[i2].act )
	       {
		  copyC( tcell, HT, i );
		  tcell[i].aant = macp[i2].aant;
	       }
	    }
	 }
      }
      for( i = 0; i < BC; i ++ )
      {
	 if( !bcell[i].act )
	    continue;

	 if( f )
	 {
	    _setcolor( 0 );
	    _ellipse(_GBORDER,bcell[i].x-5,bcell[i].y-5,bcell[i].x+5,
	       bcell[i].y+5 );
	 }

	 mvC( &bcell[i] );
	 bounceC( &bcell[i] );
	 if( f )
	 {
	    _setcolor( 3 );
	    _ellipse(_GBORDER,bcell[i].x-5,bcell[i].y-5,bcell[i].x+5,
	       bcell[i].y+5 );
	 }

	 for( i2 = 0; i2 < HT; i2 ++ )
	 {
	    if( !tcell[i2].act )
	       continue;

	    if( !tcell[i2].aant )
	       continue;

	    ab = 0;
	    for( i3 = 0; i3 < AB; i3 ++ )
	       if( agb[i3] )
		  ab ++;

	    if( ab >= AB )
	       break;

	    if( ( abs( bcell[i].x - tcell[i2].x ) < 10 ) &&
		( abs( bcell[i].y - tcell[i2].y ) < 10 ) )
	    {
	       bcell[i].aant = tcell[i2].aant;

	       copyC( bcell, BC, i );

	       for( i3 = 0; i3 < AB; i3 ++ )
	       {
		  if( !agb[i3] )
		  {
		     break;
		  }
	       }

	       abx[i3] = bcell[i].x;
	       aby[i3] = bcell[i].y;

	       bcell[i].rep = 0;
	       agb[i3] = 1;
	       ab ++;
	    }
	 }
	 bcell[i].rep ++;
	 if( ( bcell[i].rep > 20 ) && ( bcell[i].aant ) )
	 {
	    bcell[i].act = 0;
	    _setcolor( 0 );
	    _ellipse(_GBORDER,bcell[i].x-5,bcell[i].y-5,bcell[i].x+5,
	       bcell[i].y+5 );
	 }
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

	 for( i2 = 0; i2 < HT; i2 ++ )
	 {
	    if( !tcell[i2].act )
	       continue;

	    if( !tcell[i2].aant )
	       continue;

	    if( ( abs( macp[i].x - tcell[i2].x ) < 10 ) &&
		( abs( macp[i].y - tcell[i2].y ) < 10 ) )
	    {
	       copyC( macp, MP, i );
	       macp[i].aant = tcell[i2].aant;
	       macp[i].rep = 0;
	       ab ++;
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

	    dd = dist( dx, dy );

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

	 mvC( &bact[i] );
	 bact[i].rep ++;
	 bounceC( &bact[i] );

	 if( f )
	 {
	    _setcolor( 12 );
	    _ellipse(_GBORDER,bact[i].x-5,bact[i].y-5,bact[i].x+5,
	       bact[i].y+5 );
	 }

	 for( i2 = 0; i2 < AB; i2 ++ )
	 {
	    if( !agb[i2] )
	       continue;

	    dx = abx[i2] - bact[i].x;
	    dy = aby[i2] - bact[i].y;
	    d = dist( dx, dy );

	    if( d < agb[i2] )
	    {
	       if( d == 0 )
	       {
		  bact[i].dx = 0;
		  bact[i].dy = 0;
		  continue;
	       }

	       dx = agb[i2]/MAB.;
	       dx = 50 * dx / d;

	       if( bact[i].dx > 0 )
	       {

		  bact[i].dx -= dx;
		  if( bact[i].dx < 0 )
		     bact[i].dx = 0;
	       }
	       else
	       {
		  bact[i].dx += dx;
		  if( bact[i].dx > 0 )
		     bact[i].dx = 0;
	       }

	       if( bact[i].dy > 0 )
	       {
		  bact[i].dy -= dx;
		  if( bact[i].dy < 0 )
		     bact[i].dy = 0;
	       }
	       else
	       {
		  bact[i].dy += dx;
		  if( bact[i].dy > 0 )
		     bact[i].dy = 0;
	       }
	    }
	 }

	 if( bact[i].rep > 1 )
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
