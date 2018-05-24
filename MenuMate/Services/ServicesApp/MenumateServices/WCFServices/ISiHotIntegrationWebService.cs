using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using SiHotIntegration.Domain;
using SiHotIntegration.Utility;
using System.Text;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface ISiHotIntegrationWebService
    {
        [OperationContract]
        RoomDetails GetRoomDetails(RoomRequest roomRequest, int timeOut);
        [OperationContract]
        RoomChargeResponse PostRoomCharge(RoomChargeDetails roomChargeDetails, int timeOut);
        [OperationContract]
        bool ValidateCreadentials(string address, int port, int transno);

    }
}
