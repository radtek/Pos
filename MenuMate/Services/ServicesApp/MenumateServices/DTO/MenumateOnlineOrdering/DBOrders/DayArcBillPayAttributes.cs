using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.DTO.MenumateOnlineOrdering.DBOrders
{
    public class DayArcBillPayAttributes
    {
        public long DayArcBillPayId {get; set;}  
        public long DayArcBillId {get; set;}  
        public string PayType {get; set;} 
		public double SubTotal {get; set;} 
        public double Rounding {get; set;}     
        public string Note {get; set;}     
        public string PayTypeDetails {get; set;}                
        public string Properties {get; set;}            
        public string PaymentCardType {get; set;}       
        public string PayGroup {get; set;}
        public double Tip { get; set; }   
    }
}
