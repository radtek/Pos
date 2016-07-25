using System.ServiceModel;

using Menumate.WinServices.Implementation.Contracts.DTO;

namespace Menumate.WinServices.Implementation.Contracts
{
    [ServiceContract]
    public interface IReservationsCallback
    {
        [OperationContract]
        void OnLocationsChanged();

        [OperationContract]
        void OnSessionsChanged();

        #region Table Callbacks

        [OperationContract]
        void OnTablesAdded(DTOReservable[] tablesAdded);
        [OperationContract]
        void OnTablesDeleted(DTOReservable[] tablesDeleted);
        [OperationContract]
        void OnTablesUpdated(DTOReservable[] tablesChanged);

        #endregion

        [OperationContract]
        void OnReservationsChanged();
    }
}
