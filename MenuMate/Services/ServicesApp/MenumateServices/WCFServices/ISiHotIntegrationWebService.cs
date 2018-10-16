using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using SiHotIntegration.Domain;
using System.Collections.Generic;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface ISiHotIntegrationWebService
    {
        [OperationContract]
        RoomDetails GetRoomDetails(RoomRequest roomRequest, int timeOut, string apiKey);
        [OperationContract]
        RoomChargeResponse PostRoomCharge(RoomChargeDetails roomChargeDetails, int timeOut, string apiKey);
        [OperationContract]
        bool ValidateCreadentials(string address, int port, int transno);
    }
}
