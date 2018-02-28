using System;
using System.Collections.Generic;
using SalesForceCommunication.SalesforceReference;

namespace SalesForceCommunication.Utilities
{
    public class SalesForceCommunicationHelper
    {
        private readonly SforceService _salesforceService;

        public SalesForceCommunicationHelper(SforceService salesforceService)
        {
            _salesforceService = salesforceService;
        }

        public QueryResult ExecuteSelectQuery(string filterClause, IEnumerable<string> selectColumnNames)
        {
            try
            {
                filterClause = filterClause.Replace("\"", "'");
                var querySelectClause = string.Join(", ", selectColumnNames);

                var query = string.Format("SELECT {0} FROM ACCOUNT WHERE {1}", querySelectClause, filterClause);
                var queryResult = _salesforceService.query(query);

                return queryResult;
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error retrieving query from sales force.") { Source = exception.Source };
            }
        }

        public bool UpdateSalesForceColumnValue<TColumnType>(string accountName, TColumnType newValue, string columnName, string accountPropertyName = null)
        {
            try
            {
                var selectColumnNames = new List<string> { columnName };

                accountName = accountName.Replace("\"", "'");
                var filterClause = string.Format("Name = {0}", accountName);
                filterClause = filterClause.Insert((filterClause.Length), " AND Active_Customer__c = 'Yes'");

                var queryResult = ExecuteSelectQuery(filterClause, selectColumnNames);

                if (queryResult != null && queryResult.records != null && queryResult.records.Length > 0)
                {
                    sObject[] records = queryResult.records;
                    var account = (Account)records[0];


                    accountPropertyName = accountPropertyName ?? columnName;
                    var accountType = account.GetType();
                    var accountProperty = accountType.GetProperty(accountPropertyName);

                    if (accountProperty != null)
                    {
                        accountProperty.SetValue(account, newValue, null);
                        sObject[] updatedRecords = { account };
                        _salesforceService.update(updatedRecords);
                    }
                }
                return true;
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error updating values on sales force.") { Source = exception.Source };
            }
        }

        public bool UpdateSalesForceColumnValues(string accountName, IList<Tuple<string, object>> propertyWithNewValues, params string[] projectionColumns)
        {
            try
            {
                accountName = accountName.Replace("\"", "'");
                var filterClause = string.Format("Name = {0}", accountName);
                filterClause = filterClause.Insert((filterClause.Length), " AND Active_Customer__c = 'Yes'");

                var queryResult = ExecuteSelectQuery(filterClause, projectionColumns);

                if (queryResult != null && queryResult.records != null && queryResult.records.Length > 0)
                {
                    sObject[] records = queryResult.records;
                    var account = (Account)records[0];

                    var accountType = account.GetType();
                    foreach (var propertyWithNewValue in propertyWithNewValues)
                    {
                        var accountProperty = accountType.GetProperty(propertyWithNewValue.Item1);
                        if (accountProperty != null)
                        {
                            accountProperty.SetValue(account, propertyWithNewValue.Item2, null);
                        }
                    }
                    sObject[] updatedRecords = { account };
                    _salesforceService.update(updatedRecords);
                }
                return true;
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error updating values on sales force.") { Source = exception.Source };
            }
        }
    }
}