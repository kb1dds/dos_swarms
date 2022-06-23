/* BEES - Program to have dots move around the screen like a swarm of bees */
#include <stdio.h>
#include <graph.h>
#include <math.h>
#include <conio.h>

main()
{
	int lxv,lyv,lxpo,lypo,lxpn,lypn;
	int oxpo[20],oypo[20],oxpn[20],oypn[20],oxv[20],oyv[20];
	int oxvinc[20],oyvinc[20],xv,yv,x,y,maxspeed,i,j,k,l,test;
	int maxx,maxy,done;
	float momentum;

	test=0;
	maxspeed=5;
	momentum=1.5;
	maxx=320,maxy=200;  /*  Max x and y screen coordinates */
	lxpo=maxx/2,lypo=maxy/2;
	lxv=0,lyv=0;
	for(i=0;i<20;i=i+1)  /*  Set random bee start coords  */
	{
	   x=rand(),y=rand();
	   oxpo[i]=x/1000,oypo[i]=y/1000;
	}

	_setvideomode(_MRES4COLOR);

	while(test==0)   /* Go forever  */
	{
/*  Program section for the Lead fly  */

	   xv=rand(),yv=rand();   /* Determine the x / y accel for the fly */
	   xv=5-(xv/3000),yv=5-(yv/3000);
	   lxv=lxv+xv,lyv=lyv*momentum; /* To prevent a too jittery fly */
	   for(i=0;i<2;i=i+1)           /* do x one time and y the next */
	   {                            /* let momentum carry the one   */
	      if(lxv>maxspeed)          /* updated                      */
		      lxv=maxspeed;
	      if(lxv<-maxspeed)
		      lxv=-maxspeed;
	      if(lyv>maxspeed)         /* Prevent over speed  */
		      lyv=maxspeed;
	      if(lyv<-maxspeed)
		      lyv=-maxspeed;
	      lxpn=lxpo+lxv,lypn=lypo+lyv;
	      if(lxpn>maxx)
		      lxpn=maxx,lxv=-lxv;   /* Keep the fly on the screen */
	      if(lxpn<0)                    /* If he hits the edge, change*/
		      lxpn=0,lxv=-lxv;      /* sign on the velocity       */
	      if(lypn>maxy)
		      lypn=maxy-5,lyv=-lyv;
	      if(lypn<0)
		      lypn=0,lyv=-lyv;
	      _setcolor(0);           /* Erase the old point */
	      _setpixel(lxpo,lypo);
	      _setcolor(1);           /* Put on the new point */
	      _setpixel(lxpn,lypn);
	      lxv=lxv*momentum,lyv=lyv+yv;
	      lxpo=lxpn,lypo=lypn;
/*  Program section for the swarm of Bees  */

	      for(j=0;j<20;j=j+1)
	      {
	      x=rand(),y=rand();   /* Randomly set the x and y accel  */
	      x=x/8192;y=y/8192;   /* for each bee each pass through  */
	      oxvinc[j]=x,oyvinc[j]=y;
		 if(oxpo[j]-lxpn<0)     /* Determine the x dir to go */
		 {                      /* to get the fly            */
		    oxv[j]=oxv[j]+oxvinc[j];
		    if(oxv[j]>maxspeed)
		       oxv[j]=maxspeed;
		 }
		 else
		 {
		    oxv[j]=oxv[j]-oxvinc[j];
		    if(oxv[j]<-maxspeed)
		       oxv[j]=-maxspeed;
		 }
		 if(oypo[j]-lypn<0)     /* Determine the y dir to go */
		 {                      /* to ge the fly             */
		    oyv[j]=oyv[j]+oyvinc[j];
		    if(oyv[j]>maxspeed)
		       oyv[j]=maxspeed;
		 }
		 else
		 {
		    oyv[j]=oyv[j]-oyvinc[j];
		    if(oyv[j]<-maxspeed)
		       oyv[j]=-maxspeed;
		 }
		 oxpn[j]=oxpo[j]+oxv[j];
		 oypn[j]=oypo[j]+oyv[j];
		 _setcolor(0);
		 _setpixel(oxpo[j],oypo[j]);
		 _setcolor(2);
		 _setpixel(oxpn[j],oypn[j]);
		 oxpo[j]=oxpn[j],oypo[j]=oypn[j];
	      }
	      for (k=1;k<10000;k=k+1)
		   l=k/10;
	   }
	done=kbhit();         /*  Allow any key press to get out */
	if(done!=0)
	{
	   _setvideomode(_DEFAULTMODE);
	   break;
	}
	}
}
