#ifndef CONST
#ifndef STATIC
// Default definitions for screen:
#define screen_w screen->w
#define screen_h screen->h
/* WARNING: this is a guess! */
#define screen_pixelType Uint32
#define screen_pitch screen->pitch
#define screen_BytesPerPixel screen->format->BytesPerPixel
#define screen_Rmask screen->format->Rmask
#define screen_Rloss screen->format->Rloss
#define screen_Rshift screen->format->Rshift
#define screen_Gmask screen->format->Gmask
#define screen_Gloss screen->format->Gloss
#define screen_Gshift screen->format->Gshift
#define screen_Bmask screen->format->Bmask
#define screen_Bloss screen->format->Bloss
#define screen_Bshift screen->format->Bshift
#define screen_Amask screen->format->Amask
#else
// Static definitions not yet defined.
// They may be implemented to fill certain final variables in the program at init time.
#endif
#else
// Constant definitions for screen:
#define screen_w (320)
#define screen_h (200)
#define screen_pixelType Uint32
#define screen_pitch (960)
#define screen_BytesPerPixel (3)
#define screen_Rmask (16711680)
#define screen_Rloss (0)
#define screen_Rshift (16)
#define screen_Gmask (65280)
#define screen_Gloss (0)
#define screen_Gshift (8)
#define screen_Bmask (255)
#define screen_Bloss (0)
#define screen_Bshift (0)
#define screen_Amask (0)
#endif
// Includes for screen:
// NOT optimised
#define screen_GetRGB(pixel,r,g,b) { SDLwrap_GetRGB( screen_Rmask,screen_Rshift,screen_Rloss, screen_Gmask,screen_Gshift,screen_Gloss, screen_Bmask,screen_Bshift,screen_Bloss, (pixel), (r), (g), (b) ); }
#define screen_MapRGB(r,g,b) ( SDLwrap_MapRGB(screen_Rloss,screen_Rshift,screen_Gloss,screen_Gshift,screen_Bloss,screen_Bshift,screen_Amask,(r),(g),(b)) )
#define screen_regPixel(x,y) ( SDLwrap_regPixel((screen),screen_pixelType,(screen_pitch),(screen_BytesPerPixel),(x),(y)) )
#define screen_getPixel(x,y) ( SDLwrap_getPixel((screen),screen_pixelType,(screen_pitch),(screen_BytesPerPixel),(x),(y)) )
#define screen_setPixel(x,y,c) SDLwrap_setPixel((screen),screen_pixelType,(screen_pitch),(screen_BytesPerPixel),(x),(y),c)

