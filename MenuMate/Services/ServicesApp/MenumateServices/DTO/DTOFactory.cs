using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.DTO
{
    public static class DTOFactory
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDeviceDTO"></param>
        /// <returns></returns>
        public static DTOResponse BuildUseDeviceResponseDTO(DTODevice inDeviceDTO)
        {
            ServiceLogger.Log(@"Call Use Device");

            return buildStatusSingleResponse(inDeviceDTO.Type, inDeviceDTO.ID, inDeviceDTO.Address, inDeviceDTO.Port);
        }

       /// <summary>
       /// 
       /// </summary>
       /// <param name="inDeviceDTOList"></param>
       /// <returns></returns>
        public static DTOResponse[] BuildStatusResponseDTO(DTODevice[] inDeviceDTOList)
        {
            List<DTOResponse> result = new List<DTOResponse>();

            //......................................

            foreach (DTODevice deviceDTO in inDeviceDTOList)
            {
                result.Add(buildStatusSingleResponse(deviceDTO.Type, deviceDTO.ID, deviceDTO.Address, deviceDTO.Port));
            }

            //......................................

            return result.ToArray<DTOResponse>();
        }

        /// <summary>
        /// 
        /// </summary
        /// <param name="inDeviceID"></param>
        /// <param name="inAddress"></param>
        /// <param name="inPort"></param>
        /// <returns></returns>
        public static DTOResponse buildStatusSingleResponse(UInt16 inDeviceType, UInt32 inDeviceID, string inAddress, UInt16 inPort)
        {
            DTOResponse result = new DTOResponse();

            //......................................

            result.DeviceID = inDeviceID;
            result.Code = 0;
            result.Msg = @"OK Michael";

            //......................................

            return result;
        }
    }
}
