using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;

namespace MenumateServices.DTO.MenumateOnlineOrdering.DBOrders
{
    public class DayArcOrderTaxesAttributes
    {
        public long ArcOrderTaxesID {get; set;} 	                       
	    public long ArchiveId {get; set;}                   
	    public string TaxName {get; set;}                   
	    public double TaxValue {get; set;}                   
	    public TaxProfileType TaxType {get; set;}                    
    }
}
