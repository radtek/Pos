using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.DTO.MenumateOnlineOrdering.DBOrders
{
    public class SecurityAttributes
    {
        public long SecurityId {get; set;}
        public long SecurityRef {get; set;}        
        public string SecurityEvent {get; set;}     
        public string FromVal {get; set;}          
        public string ToVal {get; set;}            
        public string Note {get; set;}             
		public string TerminalName {get; set;}     
        public long UserId {get; set;}            
    }
}
