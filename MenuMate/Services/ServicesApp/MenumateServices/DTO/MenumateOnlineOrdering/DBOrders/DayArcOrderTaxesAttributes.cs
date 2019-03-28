using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.DTO.MenumateOnlineOrdering.DBOrders
{
    public class DayArcOrderTaxesAttributes
    {
        public long ArcOrderTaxesID {get; set;} 	                       
	    public long ArchiveId {get; set;}                   
	    public string TaxName {get; set;}                   
	    public double TaxValue {get; set;}                   
	    public int TaxType {get; set;}                    
    }
}
