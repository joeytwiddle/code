#define CONST
#include "incl.c"
#undef CONST

#ifdef REDEFINE

void printSurfaceDefaults(FILE *fp,char *name) {
	fprintf(fp,"// Default definitions for %s:\n",name);
	fprintf(fp,"#define %s_w %s->w\n",name,name);
	fprintf(fp,"#define %s_h %s->h\n",name,name);
	fprintf(fp,"/* WARNING: this is a guess! */\n");
	fprintf(fp,"#define %s_pixelType Uint32\n",name);
	fprintf(fp,"#define %s_pitch %s->pitch\n",name,name);
	fprintf(fp,"#define %s_BytesPerPixel %s->format->BytesPerPixel\n",name,name);
	fprintf(fp,"#define %s_Rmask %s->format->Rmask\n",name,name);
	fprintf(fp,"#define %s_Rloss %s->format->Rloss\n",name,name);
	fprintf(fp,"#define %s_Rshift %s->format->Rshift\n",name,name);
	fprintf(fp,"#define %s_Gmask %s->format->Gmask\n",name,name);
	fprintf(fp,"#define %s_Gloss %s->format->Gloss\n",name,name);
	fprintf(fp,"#define %s_Gshift %s->format->Gshift\n",name,name);
	fprintf(fp,"#define %s_Bmask %s->format->Bmask\n",name,name);
	fprintf(fp,"#define %s_Bloss %s->format->Bloss\n",name,name);
	fprintf(fp,"#define %s_Bshift %s->format->Bshift\n",name,name);
	fprintf(fp,"#define %s_Amask %s->format->Amask\n",name,name);
}

void printSurfaceStatic(FILE *fp,char *name) {
	fprintf(fp,"// Static definitions not yet defined.\n");
	fprintf(fp,"// They may be implemented to fill certain final variables in the program at init time.\n");
}

void printSurfaceConstants(FILE *fp,char *name, SDL_Surface *surface) {
	int bpp;
	fprintf(fp,"// Constant definitions for %s:\n",name);
	fprintf(fp,"#define %s_w (%i)\n",name,surface->w);
	fprintf(fp,"#define %s_h (%i)\n",name,surface->h);
	bpp=surface->format->BitsPerPixel;
	if (bpp!=8 && bpp!=16 && bpp!=32) bpp=32; // other length words do not exist on my system
	fprintf(fp,"#define %s_pixelType Uint%i\n",name,bpp);
	fprintf(fp,"#define %s_pitch (%i)\n",name,surface->pitch);
	fprintf(fp,"#define %s_BytesPerPixel (%i)\n",name,surface->format->BytesPerPixel);
	fprintf(fp,"#define %s_Rmask (%i)\n",name,surface->format->Rmask);
	fprintf(fp,"#define %s_Rloss (%i)\n",name,surface->format->Rloss);
	fprintf(fp,"#define %s_Rshift (%i)\n",name,surface->format->Rshift);
	fprintf(fp,"#define %s_Gmask (%i)\n",name,surface->format->Gmask);
	fprintf(fp,"#define %s_Gloss (%i)\n",name,surface->format->Gloss);
	fprintf(fp,"#define %s_Gshift (%i)\n",name,surface->format->Gshift);
	fprintf(fp,"#define %s_Bmask (%i)\n",name,surface->format->Bmask);
	fprintf(fp,"#define %s_Bloss (%i)\n",name,surface->format->Bloss);
	fprintf(fp,"#define %s_Bshift (%i)\n",name,surface->format->Bshift);
	fprintf(fp,"#define %s_Amask (%i)\n",name,surface->format->Amask);
}

void printSurfaceInclude(FILE *fp,char *name,SDL_Surface *surface) {
	fprintf(fp,"// Includes for %s:\n",name);
	if (surface->format->BitsPerPixel == 32) {
		fprintf(fp,"// Optimised for 32bit\n");
		fprintf(fp,"#define %s_GetRGB(pixel,r,g,b) { q32_GetRGB(pixel,r,g,b); }\n",name);
		// fprintf(fp,"#define %s_MapRGB(r,g,b) ( q32_MapRGB(r,g,b) )\n",name);
	} else {
		fprintf(fp,"// NOT optimised\n");
		fprintf(fp,"#define %s_GetRGB(pixel,r,g,b) { SDLwrap_GetRGB( %s_Rmask,%s_Rshift,%s_Rloss, %s_Gmask,%s_Gshift,%s_Gloss, %s_Bmask,%s_Bshift,%s_Bloss, (pixel), (r), (g), (b) ); }\n",name,name,name,name,name,name,name,name,name,name);
	}
	fprintf(fp,"#define %s_MapRGB(r,g,b) ( SDLwrap_MapRGB(%s_Rloss,%s_Rshift,%s_Gloss,%s_Gshift,%s_Bloss,%s_Bshift,%s_Amask,(r),(g),(b)) )\n",name,name,name,name,name,name,name,name);
	fprintf(fp,"#define %s_regPixel(x,y) ( SDLwrap_regPixel((%s),%s_pixelType,(%s_pitch),(%s_BytesPerPixel),(x),(y)) )\n",name,name,name,name,name);
	fprintf(fp,"#define %s_getPixel(x,y) ( SDLwrap_getPixel((%s),%s_pixelType,(%s_pitch),(%s_BytesPerPixel),(x),(y)) )\n",name,name,name,name,name);
	fprintf(fp,"#define %s_setPixel(x,y,c) SDLwrap_setPixel((%s),%s_pixelType,(%s_pitch),(%s_BytesPerPixel),(x),(y),c)\n",name,name,name,name,name);
}

void printSurfaceDetails(FILE *fp,char *name,SDL_Surface *surface) {
	fprintf(fp,"#ifndef CONST\n");
	fprintf(fp,"#ifndef STATIC\n");
	printSurfaceDefaults(fp,name);
	fprintf(fp,"#else\n");
	printSurfaceStatic(fp,name);
	fprintf(fp,"#endif\n");
	fprintf(fp,"#else\n");
	printSurfaceConstants(fp,name,surface);
	fprintf(fp,"#endif\n");
	printSurfaceInclude(fp,name,surface);
	fprintf(fp,"\n");
}

#endif
