using System;
using System.Collections.Generic;

namespace MenumateServices.DTO.Clipp
{
    public class ClippTabDetail
    {
        public string VenueReceiptId { get; set; }
        public string VenueTabRef { get; set; }
        public decimal TabLimit { get; set; }
        public DateTime ReceiptDate { get; set; }
        public decimal TotalAmount { get; set; }
        public decimal OutstandingAmount { get; set; }
        public decimal IncludedTaxAmount { get; set; }

        public List<Item> Items { get; set; }
        public List<Sundry> Sundries { get; set; }
        public List<Payment> Payments { get; set; }
    }
}