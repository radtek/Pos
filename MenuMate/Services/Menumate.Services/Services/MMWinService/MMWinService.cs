using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;

using System.Threading;

using System.ServiceModel;
using System.ServiceModel.Description;

using Menumate.WinServices.Implementation;
using Menumate.WinServices.Tools;

namespace Menumate.WinServices
{
    public partial class MMWinService : MMBaseWinService
    {
        public MMWinService(): base()
        {
            // Name the Windows Service
            ServiceName = "MMWinService";
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        protected override bool StartWebServices()
        {
            List<ServiceHost> webServices = WebServicesLoader.LoadWebServices(@"Config\MMWebServicesConfig.xml");

            bool result = webServices.Count > 0;

            foreach (ServiceHost sHost in webServices)
            {
                result |= LoadWebService(sHost);
            }

            return result;
        }
    }
}
