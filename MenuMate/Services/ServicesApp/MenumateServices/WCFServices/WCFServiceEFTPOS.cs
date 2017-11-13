using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Diagnostics;
using MenumateServices.DTO;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "WCFServiceEFTPOS" in both code and config file together.
    public class WCFServiceEFTPOS : IWCFServiceEFTPOS
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="deviceDTO"></param>
        /// <returns></returns>
        public DTOResponse useDevice(DTODevice inDeviceDTO)
        {
            try
            {
                return DTOFactory.BuildUseDeviceResponseDTO(inDeviceDTO);
            }
            catch (Exception exception)
            {
                ServiceLogger.LogException(exception.Message, exception);
                //EventLog.WriteEntry("In useDevice EFTPOS", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 16, short.MaxValue);
            }
            return null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="deviceDTO"></param>
        /// <returns></returns>
        public DTOResponse[] status(DTODevice[] inDeviceDTOList)
        {
            try
            {
                return DTOFactory.BuildStatusResponseDTO(inDeviceDTOList);
            }
            catch (Exception exception)
            {
                ServiceLogger.LogException(exception.Message, exception);
                //EventLog.WriteEntry("In status EFTPOS", exception.Message + "Trace" + exception.StackTrace, EventLogEntryType.Error, 17, short.MaxValue);
            }
            return null;
        }
    }
}
