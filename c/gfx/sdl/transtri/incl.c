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
#define screen_pitch (1280)
#define screen_BytesPerPixel (4)
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
// Optimised for 32bit
#define screen_GetRGB(pixel,r,g,b) { q32_GetRGB(pixel,r,g,b); }
#define screen_MapRGB(r,g,b) ( SDLwrap_MapRGB(screen_Rloss,screen_Rshift,screen_Gloss,screen_Gshift,screen_Bloss,screen_Bshift,screen_Amask,(r),(g),(b)) )
#define screen_regPixel(x,y) ( SDLwrap_regPixel((screen),screen_pixelType,(screen_pitch),(screen_BytesPerPixel),(x),(y)) )
#define screen_getPixel(x,y) ( SDLwrap_getPixel((screen),screen_pixelType,(screen_pitch),(screen_BytesPerPixel),(x),(y)) )
#define screen_setPixel(x,y,c) SDLwrap_setPixel((screen),screen_pixelType,(screen_pitch),(screen_BytesPerPixel),(x),(y),c)

#ifndef CONST
#ifndef STATIC
// Default definitions for bgtexture:
#define bgtexture_w bgtexture->w
#define bgtexture_h bgtexture->h
/* WARNING: this is a guess! */
#define bgtexture_pixelType Uint32
#define bgtexture_pitch bgtexture->pitch
#define bgtexture_BytesPerPixel bgtexture->format->BytesPerPixel
#define bgtexture_Rmask bgtexture->format->Rmask
#define bgtexture_Rloss bgtexture->format->Rloss
#define bgtexture_Rshift bgtexture->format->Rshift
#define bgtexture_Gmask bgtexture->format->Gmask
#define bgtexture_Gloss bgtexture->format->Gloss
#define bgtexture_Gshift bgtexture->format->Gshift
#define bgtexture_Bmask bgtexture->format->Bmask
#define bgtexture_Bloss bgtexture->format->Bloss
#define bgtexture_Bshift bgtexture->format->Bshift
#define bgtexture_Amask bgtexture->format->Amask
#else
// Static definitions not yet defined.
// They may be implemented to fill certain final variables in the program at init time.
#endif
#else
// Constant definitions for bgtexture:
#define bgtexture_w (512)
#define bgtexture_h (256)
#define bgtexture_pixelType Uint32
#define bgtexture_pitch (1536)
#define bgtexture_BytesPerPixel (3)
#define bgtexture_Rmask (16711680)
#define bgtexture_Rloss (0)
#define bgtexture_Rshift (16)
#define bgtexture_Gmask (65280)
#define bgtexture_Gloss (0)
#define bgtexture_Gshift (8)
#define bgtexture_Bmask (255)
#define bgtexture_Bloss (0)
#define bgtexture_Bshift (0)
#define bgtexture_Amask (0)
#endif
// Includes for bgtexture:
// NOT optimised
#define bgtexture_GetRGB(pixel,r,g,b) { SDLwrap_GetRGB( bgtexture_Rmask,bgtexture_Rshift,bgtexture_Rloss, bgtexture_Gmask,bgtexture_Gshift,bgtexture_Gloss, bgtexture_Bmask,bgtexture_Bshift,bgtexture_Bloss, (pixel), (r), (g), (b) ); }
#define bgtexture_MapRGB(r,g,b) ( SDLwrap_MapRGB(bgtexture_Rloss,bgtexture_Rshift,bgtexture_Gloss,bgtexture_Gshift,bgtexture_Bloss,bgtexture_Bshift,bgtexture_Amask,(r),(g),(b)) )
#define bgtexture_regPixel(x,y) ( SDLwrap_regPixel((bgtexture),bgtexture_pixelType,(bgtexture_pitch),(bgtexture_BytesPerPixel),(x),(y)) )
#define bgtexture_getPixel(x,y) ( SDLwrap_getPixel((bgtexture),bgtexture_pixelType,(bgtexture_pitch),(bgtexture_BytesPerPixel),(x),(y)) )
#define bgtexture_setPixel(x,y,c) SDLwrap_setPixel((bgtexture),bgtexture_pixelType,(bgtexture_pitch),(bgtexture_BytesPerPixel),(x),(y),c)

#ifndef CONST
#ifndef STATIC
// Default definitions for bgtexture2:
#define bgtexture2_w bgtexture2->w
#define bgtexture2_h bgtexture2->h
/* WARNING: this is a guess! */
#define bgtexture2_pixelType Uint32
#define bgtexture2_pitch bgtexture2->pitch
#define bgtexture2_BytesPerPixel bgtexture2->format->BytesPerPixel
#define bgtexture2_Rmask bgtexture2->format->Rmask
#define bgtexture2_Rloss bgtexture2->format->Rloss
#define bgtexture2_Rshift bgtexture2->format->Rshift
#define bgtexture2_Gmask bgtexture2->format->Gmask
#define bgtexture2_Gloss bgtexture2->format->Gloss
#define bgtexture2_Gshift bgtexture2->format->Gshift
#define bgtexture2_Bmask bgtexture2->format->Bmask
#define bgtexture2_Bloss bgtexture2->format->Bloss
#define bgtexture2_Bshift bgtexture2->format->Bshift
#define bgtexture2_Amask bgtexture2->format->Amask
#else
// Static definitions not yet defined.
// They may be implemented to fill certain final variables in the program at init time.
#endif
#else
// Constant definitions for bgtexture2:
#define bgtexture2_w (512)
#define bgtexture2_h (256)
#define bgtexture2_pixelType Uint32
#define bgtexture2_pitch (1536)
#define bgtexture2_BytesPerPixel (3)
#define bgtexture2_Rmask (16711680)
#define bgtexture2_Rloss (0)
#define bgtexture2_Rshift (16)
#define bgtexture2_Gmask (65280)
#define bgtexture2_Gloss (0)
#define bgtexture2_Gshift (8)
#define bgtexture2_Bmask (255)
#define bgtexture2_Bloss (0)
#define bgtexture2_Bshift (0)
#define bgtexture2_Amask (0)
#endif
// Includes for bgtexture2:
// NOT optimised
#define bgtexture2_GetRGB(pixel,r,g,b) { SDLwrap_GetRGB( bgtexture2_Rmask,bgtexture2_Rshift,bgtexture2_Rloss, bgtexture2_Gmask,bgtexture2_Gshift,bgtexture2_Gloss, bgtexture2_Bmask,bgtexture2_Bshift,bgtexture2_Bloss, (pixel), (r), (g), (b) ); }
#define bgtexture2_MapRGB(r,g,b) ( SDLwrap_MapRGB(bgtexture2_Rloss,bgtexture2_Rshift,bgtexture2_Gloss,bgtexture2_Gshift,bgtexture2_Bloss,bgtexture2_Bshift,bgtexture2_Amask,(r),(g),(b)) )
#define bgtexture2_regPixel(x,y) ( SDLwrap_regPixel((bgtexture2),bgtexture2_pixelType,(bgtexture2_pitch),(bgtexture2_BytesPerPixel),(x),(y)) )
#define bgtexture2_getPixel(x,y) ( SDLwrap_getPixel((bgtexture2),bgtexture2_pixelType,(bgtexture2_pitch),(bgtexture2_BytesPerPixel),(x),(y)) )
#define bgtexture2_setPixel(x,y,c) SDLwrap_setPixel((bgtexture2),bgtexture2_pixelType,(bgtexture2_pitch),(bgtexture2_BytesPerPixel),(x),(y),c)

