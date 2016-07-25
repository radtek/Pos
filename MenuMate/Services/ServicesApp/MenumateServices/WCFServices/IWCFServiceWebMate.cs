using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

using MenumateServices.WebMate.DTO;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IWCFServiceWebMate" in both code and config file together.
    [ServiceContract]
    public interface IWCFServiceWebMate
    {
        [OperationContract]
        DTO_WebOrderResponse OpenWebOrder(out string outHandle);

        [OperationContract]
        DTO_WebOrderResponse CommitOrder(string inOrderHandle);

        [OperationContract]
        DTO_WebOrderResponse SaveIncompleteOrderHeaderSection(
                                            string inOrderHandle,
                                            DTO_WebOrderHeader inOrderHeaderSection);

        [OperationContract]
        DTO_WebOrderResponse SaveIncompleteOrderFromSection(
                                            string inOrderHandle,
                                            DTO_WebOrderFrom inOrderFromSection);        

        [OperationContract]
        DTO_WebOrderResponse SaveIncompleteOrderAccountSection(
                                            string inOrderHandle,
                                            DTO_WebOrderAccount intOrderAccountSection);

        [OperationContract]
        DTO_WebOrderResponse RetrieveIncompleteOrderHeaderSection(
                                    string inOrderHandle,
                                out DTO_WebOrderHeader outHeaderSection);

        [OperationContract]
        DTO_WebOrderResponse RetrieveIncompleteOrderFromSection(
                                    string inOrderHandle,
                                out DTO_WebOrderFrom outFromSection);

        [OperationContract]
        DTO_WebOrderResponse RetrieveIncompleteOrderAccountSection(
                                    string inOrderHandle,
                                out DTO_WebOrderAccount outAccountSection);

        [OperationContract]
        DTO_WebOrderResponse GetCommittedWebOrderStatus(
                                    string inSiteName,
                                    string inStoreName,
                                    string inOrderHandle,
                                out WebOrderStatus outStatus);
    }
}
