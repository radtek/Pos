//---------------------------------------------------------------------------

#ifndef PorttalkH
#define PorttalkH

#include <windows.h>
#include <winioctl.h>


#define PORTTALK_TYPE 40000 /* 32768-65535 are reserved for customers */

// The IOCTL function codes from 0x800 to 0xFFF are for customer use.

#define IOCTL_IOPM_RESTRICT_ALL_ACCESS \
    CTL_CODE(PORTTALK_TYPE, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_IOPM_ALLOW_EXCUSIVE_ACCESS \
    CTL_CODE(PORTTALK_TYPE, 0x901, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SET_IOPM \
    CTL_CODE(PORTTALK_TYPE, 0x902, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ENABLE_IOPM_ON_PROCESSID \
    CTL_CODE(PORTTALK_TYPE, 0x903, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_READ_PORT_UCHAR \
    CTL_CODE(PORTTALK_TYPE, 0x904, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_WRITE_PORT_UCHAR \
    CTL_CODE(PORTTALK_TYPE, 0x905, METHOD_BUFFERED, FILE_ANY_ACCESS)


class TPortTalk
{
   HANDLE PortTalk_Handle;        /* Handle for PortTalk Driver */

   bool OpenPortTalk(void);
   void ClosePortTalk(void);

   void outportb(unsigned short PortAddress, unsigned char byte);
   unsigned char inportb(unsigned short PortAddress);

   void InstallPortTalkDriver(void);
   unsigned char StartPortTalkDriver(void);

   public:
      bool Initialise();
      TPortTalk();
      ~TPortTalk();
      void OutPort(unsigned short PortAddress, unsigned char byte);
      unsigned char InPort(unsigned short PortAddress);


};

//---------------------------------------------------------------------------
#endif
