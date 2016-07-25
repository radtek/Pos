//---------------------------------------------------------------------------
#pragma hdrstop

#include "Color.h"
#include <windows.h>
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))
//---------------------------------------------------------------------------

#pragma package(smart_init)
HCL rgbToHcl(int col)
 {
  int r, g, b, cmin, cmax;
  int ch, dr, dg, db;
  HCL hcl;

  r = GetRValue(col);
  g = GetGValue(col);
  b = GetBValue(col);
  cmin = min(min(r, g), b);
  cmax = max(max(r, g), b);
  hcl.c = cmax-cmin; // chromma // global_ret_2
  hcl.l = (cmax+cmin)/2; // luminance // global_ret_3

  // acromatic case
  if (hcl.c == 0)
  {
   hcl.h = 0; // or some undefined value
   return hcl;
  }

  ch = hcl.c/2;
  if (cmax == r)
  {
   dg = ((cmax-g)*HPE1 + ch)/hcl.c;
   db = ((cmax-b)*HPE1 + ch)/hcl.c;
   hcl.h = HPE1+db-dg;
  }
  else if (cmax == g)
  {
   dr = ((cmax-r)*HPE1 + ch)/hcl.c;
	db = ((cmax-b)*HPE1 + ch)/hcl.c;
   hcl.h = HPE3 + dr-db;
  }
  else
  {
   dr = ((cmax-r)*HPE1 + ch)/hcl.c;
   dg = ((cmax-g)*HPE1 + ch)/hcl.c;
   hcl.h = HPE5 + dg-dr;
  }

  if (hcl.h < 0) hcl.h += HUENUM;
  else if (hcl.h > HUEMAX) hcl.h -= HUENUM;
  return hcl;
 }

//
// Given h/c/l components, this function puts togeather the color in rgb
//
int hlcToRgb(HCL hcl)
 {
  int r, g, b, cmax, cmin;
  int dl;

  //
  // Do a little checking, make sure we clamp cromma if outside (for given luminance).
  // Hue and Luminance wont be affected (which is where most
  // of the visual info is)
  //

  if (hcl.l < 128)
  {
	dl = 2*hcl.l;
   if (hcl.c > dl)
   {
    cmax = dl;
    cmin = 0;
    hcl.c = dl;
   }
   else
	{
    cmax = hcl.l+hcl.c/2;
    cmin = cmax-hcl.c;
   }
  }
  else
  {
   dl = 2*(255-hcl.l);
	if (hcl.c > dl)
   {
    cmax = 255;
    cmin = 255-dl;
    hcl.c = dl;
	}
   else
   {
	 cmax = hcl.l+hcl.c/2;
    cmin = cmax-hcl.c;
	}
  }

  //
  // Now calculate the rgb components
  //

  if (hcl.h < HPE2)
  {
   r = cmax;
   if (hcl.h < HPE1)
   {
    g = cmin;
	 b = cmin + (hcl.c*(HPE1-hcl.h)/HPE1);
	}
   else
	{
    b = cmin;
	 g = cmin + (hcl.c*(hcl.h-HPE1)/HPE1);
   }
  }
  else if (hcl.h < HPE4)
  {
	g = cmax;
   if (hcl.h < HPE3)
	{
	 b = cmin;
	 r = cmin + (hcl.c*(HPE3-hcl.h)/HPE1);
   }
	else
	{
	 r = cmin;
	 b = cmin + (hcl.c*(hcl.h-HPE3)/HPE1);
	}
  }
  else
  {
	b = cmax;
	if (hcl.h < HPE5)
	{
	 r = cmin;
	 g = cmin + (hcl.c*(HPE5-hcl.h)/HPE1);
	}
	else
	{
	 g = cmin;
	 r = cmin + (hcl.c*(hcl.h-HPE5)/HPE1);
	}
  } return RGB(r, g, b);
 }
