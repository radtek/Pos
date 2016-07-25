//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeviceImage.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
TDeviceImage::TDeviceImage()
{
   ID.Name = "Device Image";
   ID.Type = devPC;
   ID.IP = "Unknown";
	ID.Location = "";
   ID.DeviceKey = 0;
   ID.LocationKey = 0;
   ID.ProfileKey = 0;
}

void TDeviceImage::Initialise()
{
}


void TDeviceImage::Copy(TDeviceImage *Image)
{
	ID = Image->ID;
}

void TDeviceImage::Decode(TMemoryStream * Stream)
{
   // CLEAR DEVICE ID STRUCTURE
   ID.Name = "";
   ID.IP = "";
   // Type
   Stream->Read(&ID.Type,sizeof(ID.Type));
   // Name
   char input = NULL;
   do
   {
      Stream->Read(&input,sizeof(input));
      if (input != NULL)
      {
         ID.Name += (char)(input);
      }
   }while(input != NULL);

   input = NULL;
   do
   {
      Stream->Read(&input,sizeof(input));
      if (input != NULL)
      {
         ID.IP += (char)(input);
      }
   }while(input != NULL);
}

void TDeviceImage::Encode(TMemoryStream * Stream)
{
   // DEVICE ID STRUCTURE
   unsigned char nully = NULL;
   // Type
   Stream->Write(&ID.Type,sizeof(ID.Type));
   // Name
   Stream->Write(ID.Name.c_str(),ID.Name.Length());
   Stream->Write(&nully,sizeof(nully));
   Stream->Write(ID.IP.c_str(),ID.IP.Length());
   Stream->Write(&nully,sizeof(nully));
}
