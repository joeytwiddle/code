 //////////////////////////////////////////////////////
/**********     Fly.c by Micha³ W¹sowicz     **********/
 //////////////////////////////////////////////////////

/******************************************************/
/*  It is a simple program using double buffering for */
/* displaying a randomly flying circle (I didn't want */
/*   to use sprites, because "fly" is just a simple   */
/* demonstration of random() function for begginers.) */
/******************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "allegro.h"		// I use ALLEGRO library
		       // by Shawn Hargreaves.

/* Pointer to a buffer, where the circle will be drawn. */
BITMAP *buffer;

int num;
int count = 0;
int radius = 2;
int flag = 1;
int x = 160;			// Start position of the fly.

int y = 100;

main ()
{
  int vx, vy, x1, y1;
  int count = 0;

  allegro_init ();
  install_keyboard ();
  install_timer ();

  set_gfx_mode (GFX_AUTODETECT, 320, 240, 0, 0);
  set_pallete (desktop_palette);

  buffer = create_bitmap (320, 240);

  for (;;)
    {
      count++;

      clear (buffer);

      circlefill (buffer, x, y, 3, 255);

      textout_centre (buffer, font, "Press SPACE!", 60, 220, 4);

      blit (buffer, screen, 0, 0, 0, 0, 320, 240);

      vx = (random () % 3);	// Randomize the next position

      vy = (random () % 3);	// of the fly.

      x1 = (random () % 3);
      y1 = (random () % 3);

      vx -= x1;			// Substracting allows negative numbers

      vy -= y1;			// (the fly can move up and to the left).

      x += vx;
      y += vy;


      if (x <= 0)		// Don't let the fly run away of the screen.

	x = 2;
      if (x >= 320)
	x = 318;
      if (y <= 0)
	y = 2;
      if (y >= 220)
	y = 218;

      if (count >= 1000)	// After a period of time the fly gets tired.

	{
	  count = 0;
	  num = 0;
	  tired ();
	}

      if (key[KEY_ESC])
	{
	  destroy_bitmap (buffer);
	  exit (0);
	}
      else if (key[KEY_SPACE])	// You can also make the fly tired

	count = 1000;		// by pressing SPACE.

    }
}

tired ()
{
  for (num == 0; num <= 50; num++)
    {
      clear (buffer);

/* Put the text under the fly. */
      textout_centre (buffer, font, "I'm tired!", x, y + 10, 5);

      circlefill (buffer, x, y, radius, 1);

      textout_centre (buffer, font, "Press ENTER!", 60, 220, 4);

      blit (buffer, screen, 0, 0, 0, 0, 320, 240);

      if (flag == 1)
	{
	  radius += 1;		// Change the size of the circle

	  flag = 2;
	}			// to get the effect of gasping.

      else if (flag == 2)
	{
	  radius += 1;
	  flag = 3;
	}
      else if (flag == 3)
	{
	  radius -= 2;
	  flag = 1;
	}

      rest (120);		// Delay a bit.

      if (key[KEY_ENTER])	// Fly again by pressing ENTER.

	break;
    }
  return;
}
//////////////////////////////////////////////////////////
