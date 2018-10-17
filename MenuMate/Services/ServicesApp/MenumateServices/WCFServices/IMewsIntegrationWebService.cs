using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using MewsIntegration.Domain;

namespace MenumateServices.WCFServices
{
    [ServiceContract]
    public interface IMewsIntegrationWebService
    {
        [OperationContract]
        List<Service> GetMewsServices(string platformAddress, BasicInquiry basicInquiry);
        [OperationContract]
        List<AccountingCategory> GetMewsAccountingCategories(string platformAddress, BasicInquiry basicInquiry);
        [OperationContract]
        List<Outlet> GetMewsOutlets(string platformAddress, BasicInquiry basicInquiry);
        [OperationContract]
        List<CustomerDetailsMews> SearchCustomers(string platformAddress, CustomerSearch customerSearch);
        [OperationContract]
        SpaceDetails GetSpaceIds(string platformAddress, BasicInquiry customerSearch);
        [OperationContract]
        string PostOrder(string platformAddress, Order order);
        [OperationContract]
        bool PostBill(string platformAddress, Order order);
    }
}
