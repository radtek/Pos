using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

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
            return DTOFactory.BuildUseDeviceResponseDTO(inDeviceDTO);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="deviceDTO"></param>
        /// <returns></returns>
        public DTOResponse[] status(DTODevice[] inDeviceDTOList)
        {
            return DTOFactory.BuildStatusResponseDTO(inDeviceDTOList);
        }
    }
}
