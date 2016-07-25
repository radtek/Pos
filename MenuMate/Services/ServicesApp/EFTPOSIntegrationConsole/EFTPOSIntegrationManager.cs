using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using MenumateServices.DTO;

namespace EFTPOSIntegrationConsole
{
    class EFTPOSIntegrationManager
    {
        WCFServiceEFTPOSClient _eftposClient = new WCFServiceEFTPOSClient();
        List<DTODevice> _deviceList = new List<DTODevice>();

        EFTPOSIntegrationManager()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public static EFTPOSIntegrationManager Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly EFTPOSIntegrationManager instance = new EFTPOSIntegrationManager();
        }

        private string AddressField;

        private uint IDField;

        private ushort PortField;

        private ushort TypeField;

        //public bool addDevice(uint inDeviceType, )
        //{
        //    return _eftposClient.useDevice(inDevice);
        //}
    }
}
