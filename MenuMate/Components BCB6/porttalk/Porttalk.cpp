//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include "Porttalk.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TPortTalk::TPortTalk()
{
}

TPortTalk::~TPortTalk()
{
   ClosePortTalk();
}

bool TPortTalk::Initialise()
{
   return OpenPortTalk();
}

void TPortTalk::OutPort(unsigned short PortAddress, unsigned char byte)
{
   outportb(PortAddress,byte);
}

unsigned char TPortTalk::InPort(unsigned short PortAddress)
{
   return inportb(PortAddress);
}


void TPortTalk::outportb(unsigned short PortAddress, unsigned char byte)
{
    unsigned int error;
    DWORD BytesReturned;        
    unsigned char Buffer[3];
    unsigned short * pBuffer;
    pBuffer = (unsigned short *)&Buffer[0];
    *pBuffer = PortAddress;
    Buffer[2] = byte;

    error = DeviceIoControl(PortTalk_Handle,
                            IOCTL_WRITE_PORT_UCHAR,
                            &Buffer,
                            3,
                            NULL,
                            0,
                            &BytesReturned,
                            NULL);

    if (!error)
    {
		UnicodeString Error;
		Error.printf(UnicodeString("Error occured during outportb while talking to PortTalk driver %d\n").w_str(),GetLastError());
		throw Exception(Error);
    }
}

unsigned char TPortTalk::inportb(unsigned short PortAddress)
{
    unsigned int error;
    DWORD BytesReturned;
    unsigned char Buffer[3];
    unsigned short * pBuffer;
    pBuffer = (unsigned short *)&Buffer;
    *pBuffer = PortAddress;

    error = DeviceIoControl(PortTalk_Handle,
                            IOCTL_READ_PORT_UCHAR,
                            &Buffer,
                            2,
                            &Buffer,
                            1,
                            &BytesReturned,
                            NULL);
    if (!error)
    {
      UnicodeString Error;
		Error.printf(UnicodeString("Error occured during inportb while talking to PortTalk driver %d\n").w_str(),GetLastError());
		throw Exception(Error);
    }
    return(Buffer[0]);
}

bool TPortTalk::OpenPortTalk(void)
{
    /* Open PortTalk Driver. If we cannot open it, try installing and starting it */
	 PortTalk_Handle = CreateFile(_T("\\\\.\\PortTalk"),
											GENERIC_READ,
                                 0,
                                 NULL,
											OPEN_EXISTING,
											FILE_ATTRIBUTE_NORMAL,
											NULL);

    if(PortTalk_Handle == INVALID_HANDLE_VALUE) {
            /* Start or Install PortTalk Driver */
            StartPortTalkDriver();
            /* Then try to open once more, before failing */
				PortTalk_Handle = CreateFile(_T("\\\\.\\PortTalk"),
                                         GENERIC_READ, 
                                         0, 
                                         NULL,
                                         OPEN_EXISTING, 
                                         FILE_ATTRIBUTE_NORMAL, 
                                         NULL);

            if(PortTalk_Handle == INVALID_HANDLE_VALUE)
            {
					throw Exception("PortTalk: Couldn't access PortTalk Driver, Please ensure driver is loaded.\n\n");
            }
    }
    return true;
}

void TPortTalk::ClosePortTalk(void)
{
    CloseHandle(PortTalk_Handle);
}

unsigned char TPortTalk::StartPortTalkDriver(void)
{
    SC_HANDLE  SchSCManager;
    SC_HANDLE  schService;
    BOOL       ret;
    DWORD      err;

    try
    {
       /* Open Handle to Service Control Manager */
       SchSCManager = OpenSCManager (NULL,                        /* machine (NULL == local) */
                                     NULL,                        /* database (NULL == default) */
                                     SC_MANAGER_ALL_ACCESS);      /* access required */

       if (SchSCManager == NULL)
       {
         if (GetLastError() == ERROR_ACCESS_DENIED)
         {
            /* We do not have enough rights to open the SCM, therefore we must */
            /* be a poor user with only user rights. */
            UnicodeString Error;
            Error = "PortTalk: You do not have rights to access the Service Control Manager and\n"
                    " the PortTalk driver is not installed or started. Please ask \n"
                    " your administrator to install the driver on your behalf.\n";
            throw Exception(Error);
         }
       }

       do {
            /* Open a Handle to the PortTalk Service Database */
            schService = OpenService(SchSCManager,         /* handle to service control manager database */
												 _T("PortTalk"),           /* pointer to name of service to start */
												 SERVICE_ALL_ACCESS);  /* type of access to service */

            if (schService == NULL)
               switch (GetLastError()){
                   case ERROR_ACCESS_DENIED:
                           throw Exception("PortTalk: You do not have rights to the PortTalk service database\n");
                   case ERROR_INVALID_NAME:
                           throw Exception("PortTalk: The specified service name is invalid.\n");
                   case ERROR_SERVICE_DOES_NOT_EXIST:
                           InstallPortTalkDriver();
                           break;
               }
            } while (schService == NULL);

       /* Start the PortTalk Driver. Errors will occur here if PortTalk.SYS file doesn't exist */
    
       ret = StartService (schService,    /* service identifier */
                           0,             /* number of arguments */
                           NULL);         /* pointer to arguments */
                    
       if (!ret)
       {
           err = GetLastError();
           if (err != ERROR_SERVICE_ALREADY_RUNNING)
           {
             throw Exception("PortTalk: Unknown error while starting PortTalk driver service.\n"
                             "PortTalk: Does PortTalk.SYS exist in your \\System32\\Drivers Directory?\n");
           }
       }

       /* Close handle to Service Control Manager */
       CloseServiceHandle (schService);
    }
    catch(Exception &Err)
    {
       CloseServiceHandle (schService);
       throw;
    }
    return(TRUE);
}

void TPortTalk::InstallPortTalkDriver(void)
{
    SC_HANDLE  SchSCManager;
    SC_HANDLE  schService;
    DWORD      err;
	 char    DriverFileName[200];

    /* Get Current Directory. Assumes PortTalk.SYS driver is in this directory.    */
    /* Doesn't detect if file exists, nor if file is on removable media - if this  */
    /* is the case then when windows next boots, the driver will fail to load and  */
    /* a error entry is made in the event viewer to reflect this */

    /* Get System Directory. This should be something like c:\windows\system32 or  */
    /* c:\winnt\system32 with a Maximum Character lenght of 20. As we have a       */
    /* buffer of 80 bytes and a string of 24 bytes to append, we can go for a max  */
	 /* of 55 bytes */
   try
   {
		if (!GetSystemDirectory(DriverFileName, 175))
      {
			 throw Exception("PortTalk: Failed to get System Directory. Is System Directory Path > 55 Characters?\n"
                          "PortTalk: Please manually copy driver to your system32/driver directory.\n");
      }

       /* Append our Driver Name */
		lstrcat(DriverFileName,_T("\\Drivers\\PortTalk.sys"));
		/* Copy Driver to System32/drivers directory. This fails if the file doesn't exist. */
		if (!CopyFile(_T("PortTalk.sys"), DriverFileName, FALSE))
		{
         throw Exception(UnicodeString("PortTalk: Failed to copy driver to ") + DriverFileName +
                         UnicodeString("\n Please manually copy driver to your system32/driver directory.\n"));
      }

       /* Open Handle to Service Control Manager */
       SchSCManager = OpenSCManager (NULL,                   /* machine (NULL == local) */
                                     NULL,                   /* database (NULL == default) */
                                     SC_MANAGER_ALL_ACCESS); /* access required */

       /* Create Service/Driver - This adds the appropriate registry keys in */
       /* HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services - It doesn't  */
       /* care if the driver exists, or if the path is correct.              */

       schService = CreateService (SchSCManager,                      /* SCManager database */
											  _T("PortTalk"),                        /* name of service */
											  _T("PortTalk"),                        /* name to display */
											  SERVICE_ALL_ACCESS,                /* desired access */
											  SERVICE_KERNEL_DRIVER,             /* service type */
											  SERVICE_DEMAND_START,              /* start type */
											  SERVICE_ERROR_NORMAL,              /* error control type */
											  _T("System32\\Drivers\\PortTalk.sys"), /* service's binary */
											  NULL,                              /* no load ordering group */
                                   NULL,                              /* no tag identifier */
                                   NULL,                              /* no dependencies */
                                   NULL,                              /* LocalSystem account */
                                   NULL                               /* no password */
											  );

       if (schService == NULL)
       {
            err = GetLastError();
            if (err != ERROR_SERVICE_EXISTS)
               throw Exception("PortTalk: Unknown error while creating Service.\n");
       }

       /* Close Handle to Service Control Manager */
       CloseServiceHandle (schService);
   }
   catch(Exception &Err)
   {
       CloseServiceHandle (schService);
       throw;
   }
}