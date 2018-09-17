using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MewsIntegration;
using MewsIntegration.Domain;

namespace MenumateServices.WCFServices
{
    public class MewsIntegrationWebService : IMewsIntegrationWebService
    {
        public List<Service> GetMewsServices(string platformAddress, BasicInquiry basicInquiry)
        {
            List<Service> service = new List<Service>();
            try
            {
                MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                service = mewsCommunicationController.GetMewsServices(platformAddress, basicInquiry);
            }
            catch (Exception exception)
            {
                int i = 0;
            }
            return service;
        }

        public List<AccountingCategory> GetMewsAccountingCategories(string platformAddress, BasicInquiry basicInquiry)
        {
            List<AccountingCategory> accountingCategory = new List<AccountingCategory>();
            try
            {
                MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                accountingCategory = mewsCommunicationController.GetMewsAccountingCategories(platformAddress, basicInquiry);
            }
            catch (Exception exception)
            {
                int i = 0;
            }
            return accountingCategory;
        }
        public List<Outlet> GetMewsOutlets(string platformAddress, BasicInquiry basicInquiry)
        {
            List<Outlet> outlets = new List<Outlet>();
            try
            {
                MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                outlets = mewsCommunicationController.GetMewsOutlets(platformAddress, basicInquiry);
            }
            catch (Exception exception)
            {
                int i = 0;
            }
            return outlets;
        }
        public List<Customer> SearchCustomers(string platformAddress, CustomerSearch customerSearch)
        {
            string retValue = "";
            List<Customer> customers = new List<Customer>();
            try
            {
                MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                customers = mewsCommunicationController.GetCustomerSearchResult(platformAddress, customerSearch);
            }
            catch (Exception exception)
            {
                int i = 0;
            }
            return customers;
        }

        public SpaceDetails GetSpaceIds(string platformAddress, BasicInquiry basicInquiry)
        {
            string retValue = "";
            SpaceDetails spaceDetails = new SpaceDetails();
            try
            {
                MewsCommunicationController mewsCommunicationController = new MewsCommunicationController();
                spaceDetails = mewsCommunicationController.GetSpaceIds(platformAddress, basicInquiry);
            }
            catch (Exception exception)
            {
                int i = 0;
            }
            return spaceDetails;
        }
    }
}
