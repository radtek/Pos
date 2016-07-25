using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ServiceModel;

using Menumate.WinServices.Implementation.Contracts.DTO;

namespace Menumate.WinServices.Implementation.Contracts
{
    [ServiceContract]
    public interface IPosIntegration
    {
        [OperationContract]
        DTOLocation[] GetAllLocations();

        [OperationContract]
        DTOReservable[] GetTablesForLocation(int locationId);

        [OperationContract]
        byte[] GetBackgroundImgDataForLocation(int locationid);

        [OperationContract]
        OperationResult AddLocation(DTOLocation locationDTO);

        [OperationContract]
        OperationResult AddReservable(DTOReservable tableDTO, DTOLocation locationDTO);

        [OperationContract]
        OperationResult DeleteLocation(DTOLocation locationDTO);

        [OperationContract]
        OperationResult DeleteReservable(DTOReservable tableDTO, DTOLocation locationDTO);

        [OperationContract]
        OperationResult UpdateLocation(DTOLocation location);

        [OperationContract]
        OperationResult UpdateReservable(DTOReservable reservable);

    }
}
