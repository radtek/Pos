using System.Collections.Generic;

namespace MenumateServices.DTO.Clipp
{
    public class Item
    {
        public string VenueItemRef { get; set; }
        public string Description { get; set; }
        public decimal Quantity { get; set; }
        public decimal Price { get; set; }
        public decimal TotalAmount { get; set; }
        public decimal DiscountAmount { get; set; }
        public string DiscountDescription { get; set; }
        public bool IsTaxInclusive { get; set; }
        public string Group { get; set; }
        public string Category { get; set; }
        public List<Item> SubItems { get; set; }
    }
}