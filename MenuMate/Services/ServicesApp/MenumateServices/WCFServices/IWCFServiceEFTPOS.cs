using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

using MenumateServices.DTO;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IWCFServiceEFTPOS" in both code and config file together.
    [ServiceContract]
    public interface IWCFServiceEFTPOS
    {
        [OperationContract]
        DTOResponse useDevice(DTODevice inDeviceDTO);

        [OperationContract]
        DTOResponse[] status(DTODevice[] inDeviceDTOList);
    }
}
