using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;

using System.ServiceModel;
namespace MenumateServices
{
    partial class MenumateServiceEFTPOS : MenumateService
    {
        /// <summary>
        /// 
        /// </summary>
        public MenumateServiceEFTPOS(): base()
        {
            // Name the Windows Service
            ServiceName = serviceNameWithID(MenumateServicesID.EFTPosIntegration);
        }

        #region Protected

        /// <summary>
        /// Overrides the abstract class defined in MenumateService
        /// </summary>
        /// <returns></returns>
        protected override bool startServiceTask()
        {
            bool result = false;

            //::::::::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                result = true;
            }
            catch (Exception)
            {
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        #endregion
    }
}
