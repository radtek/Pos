using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.DTO.MenumateOnlineOrdering.DBOrders
{
    public class DayArchiveAttributes
    {
        public long ArchiveId {get; set;}  
		public long ArcBillId {get; set;} 
		public string TerminalName {get; set;}  	
		public string MenuName {get; set;}
		public string CourseName {get; set;}
		public string ItemName {get; set;}
		public string ItemCategory {get; set;}  
		public string ItemShortName {get; set;} 	
		public long ItemId {get; set;}  	
		public string SizeName {get; set;}
		public long TableNumber {get; set;}  		  
		public string TableName {get; set;}  		  
		public int SeatNumber {get; set;}  		      
		public string ServerName {get; set;}  		  
		public string TabName {get; set;}  		      
		public string LoyaltyName {get; set;}  		  
		public int OrderType {get; set;}  		      
		public string OrderLocation {get; set;}  		
		public double Price {get; set;}  		      
		public double Cost {get; set;}  		      
		public string Note {get; set;} 		          
		public long SecurityRef {get; set;} 		  
		public int TimeKey {get; set;} 		          
		public double GstPercent {get; set;} 		  
		public double CostGstPercent {get; set;} 		
		public double Qty {get; set;} 		          
		public double Discount {get; set;} 		      
		public double Redeemed {get; set;} 		      
		public double PointsPercent {get; set;} 		
		public double PointsEarned {get; set;} 		  
		public long LoyaltyId {get; set;} 		      
		public long ThirdPartyCodesId {get; set;} 		
		public long CategoryId {get; set;} 		  
		public double PriceLevel0 {get; set;}		  
		public double PriceLevel1 {get; set;}		  
		public long ServingCousesId {get; set;}		
		public double BasePrice {get; set;}		      
        public double PriceIncl {get; set;}           
        public double PriceAdjust {get; set;}          
        public string OrderGuid {get; set;}           

    }
}
