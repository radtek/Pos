using System;

public class TMachineInfo 
{
    // Class to put Registration Fields into.	    
    public String CompanyName;
    public String TerminalName;
    public String HardwareKey;
    public String CustomerKey;		// MD5 ([KeyHardware] [Company Name])
    public String RegistrationKey;	// MD5 ([Prod Name] [Liciences] [Cust Key])
    public String ProductName;
    public int DbKey;				// Index into DB ordered by Product:Terminal_Name:MachineKey

    public UInt32 POSLicences;
    public UInt32 PalmLicences;
    public UInt32 OfficeLicences;

    public UInt32 MenuMateModules;		// Bit field
    public DateTime Expiry;

    public TMachineInfo()
	{

	}
}
