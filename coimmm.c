/* Module for immune simulation */
#include <stdlib.h>

typedef struct
{
   int x, y;    /* x,y of cell */
   int dx, dy;  /* delta-x, delta-y of cell */
   int sp;      /* Self protein */
   int aant;    /* antigen protein collected */
   int rep;     /* replicate flag */
   int out;     /* chemical messenger out */
   int act;     /* Activation */
}C;

int initC( C *ce );
int ryn( );
int bounceC( C *ce );
int mvC( C *ce );
int copyC( C ce[], int m, int orig );
int run_xyb( double dx1,
	     double dy1,
	     double *a1
	    );

int initC( C *ce )
{
   ce->x = rand() / ( RAND_MAX / 640 );
   ce->y = rand() / ( RAND_MAX / 480 );

   ce->dx = rand() / ( RAND_MAX / 10 );
   ce->dy = rand() / ( RAND_MAX / 10 );

   if( ryn() )
      ce->dx = -ce->dx;

   if( ryn() )
      ce->dy = -ce->dy;

   ce->sp = 0;
   ce->aant = 0;
   ce->rep = 0;
   ce->out = 0;
}

int ryn( )
{
   if( rand() < ( RAND_MAX / 2 ) )
     return( 1 );

   return( 0 );
}

int bounceC( C *ce )
{
   if( !ce->act )
      return( 0 );

   if( ce->x < 0 )
   {
      ce->x = 0;
      ce->dx = -ce->dx;
   }
   if( ce->x > 640 )
   {
      ce->x = 640;
      ce->dx = -ce->dx;
   }
   if( ce->y < 0 )
   {
      ce->y = 0;
      ce->dy = -ce->dy;
   }
   if( ce->y > 480 )
   {
      ce->y = 480;
      ce->dy = -ce->dy;
   }
}

int mvC( C *ce )
{
   ce->x += ce->dx;
   ce->y += ce->dy;
}

int copyC( C ce[], int m, int orig )
{
  int i;

  for( i = 0; i < m; i ++ )
  {
     if( ce[i].act == 0 )
     {
	initC( &ce[i] );

	ce[i].act = 1;

	ce[i].x = ce[orig].x;
	ce[i].y = ce[orig].y;

	ce[i].rep = 0;
	ce[i].sp = ce[orig].sp;
	break;
     }
  }
}

int run_xyb( double dx1,
	     double dy1,
	     double *a1
	    )
{
   /* calculate angle to each object */
   if( dx1 != 0 )
   {
      *a1=atan2( dy1, dx1 );
   }
   else
   {
      if( dy1 > 0 )
	 *a1=1.5708;
      if( dy1 < 0 )
	 *a1=4.7124;
      if( dy1 == 0 )
	 return( 1 );
   }
   if( *a1 < 0 )
      *a1=*a1+6.28319;
   if( *a1 > 6.28319 )
      *a1=*a1-6.28319;
}

