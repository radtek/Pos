//---------------------------------------------------------------------------

#ifndef ColorH
#define ColorH
//---------------------------------------------------------------------------
typedef struct tag_hcl
{
	 int h; // hue
	 int c; // cromma
	 int l; // luminance
}HCL;

// this is what we arbitrarly set the number of distinct hues there are! (must be a multiple of 6)
#define HUENUM 510

// given number above this is 1*HPE6/6, 2*HPE6/6, 3*HPE6/6 etc.
#define HPE1 85
#define HPE2 170
#define HPE3 255
#define HPE4 340
#define HPE5 425
#define HPE6 510

// Hues range (0 - HUENUM-1)
#define HUEMAX 509

// max value for rgb components (also max for luminance)
#define RGBMAX 255
//
// given the color, this function calculates hue, cromma, luminance
//
HCL rgbToHcl(int col);
int hlcToRgb(HCL hcl);

#endif
