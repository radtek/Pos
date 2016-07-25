using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using WebmateConsole.ServiceReferenceWebmate;

namespace WebmateConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            WebmateClientInterface clientInterface = WebmateClientInterface.Instance;

            try
            {
                // -- OPEN ORDER --
                string orderHandle = clientInterface.OpenOrder();

                //......................

                // -- Save HEADER SECTION --
                clientInterface.SaveHeaderSection(
                                    orderHandle,   // Order Handle
                                    Guid.NewGuid().ToString(), // Order GUID
                                    0.00m,         // Order Total
                                    DateTime.Now,  // Order Date
                                    DateTime.Now,  // Expected Date
                                    1,             // Scheduled
                                    @"Menumate's Store"); // Store Name

                //......................

                // -- Save FROM SECTION --
                clientInterface.SaveFromSection(
                    orderHandle,                // Order Handle
                    Guid.NewGuid().ToString()); // Store GUID

                //......................

                // Create ACCOUNT SECTION
                // Comments
                List<string> comments = new List<string>();
                comments.Add(@"Test Comment 001");
                comments.Add(@"Test Comment 002");
                comments.Add(@"Test Comment 003");

                // Options
                List<DTO_WebOrderItemOption> options1 = new List<DTO_WebOrderItemOption>();
                options1.Add(clientInterface.CreateAccountOrderItemOption(
                                1,         // Discount ID
                                @"101",    // GUID
                                1,         // Qty
                                101.00m,   // Base Price
                                @"Item Option1")); // ItemNote));

                List<DTO_WebOrderItemOption> options2 = new List<DTO_WebOrderItemOption>();
                options2.Add(clientInterface.CreateAccountOrderItemOption(
                                1,         // Discount ID
                                @"111",    // GUID
                                1,         // Qty
                                111.00m,  // Base Price;
                                @"Item Option1")); // ItemNote));
                options2.Add(clientInterface.CreateAccountOrderItemOption(
                                1,         // Discount ID
                                @"112",    // GUID
                                1,         // Qty
                                112.00m,   // Base Price;
                                @"Item Option2")); // ItemNote));

                List<DTO_WebOrderItemOption> options3 = new List<DTO_WebOrderItemOption>();
                options3.Add(clientInterface.CreateAccountOrderItemOption(
                                1,         // Discount ID
                                @"121",    // GUID
                                1,         // Qty
                                121.00m,   // Base Price;
                                @"Item Option3")); // ItemNote));
                options3.Add(clientInterface.CreateAccountOrderItemOption(
                                1,         // Discount ID
                                @"122",    // GUID
                                1,         // Qty
                                122.00m,   // Base Price
                                @"Item Option4")); // ItemNote));
                options3.Add(clientInterface.CreateAccountOrderItemOption(
                                1,         // Discount ID
                                @"123",    // GUID
                                1,         // Qty
                                123.00m,   // Base Price
                                @"Item Option5")); // ItemNote));

                // Sides 
                List<DTO_WebOrderItemSide> sides1 = new List<DTO_WebOrderItemSide>();
                sides1.Add(clientInterface.CreateAccountOrderItemSide(
                                1,         // Discount ID
                                @"2408",    // GUID
                                1,         // Qty
                                201.00m,   // Base Price
                                @"Item Side1")); // ItemNote));

                List<DTO_WebOrderItemSide> sides2 = new List<DTO_WebOrderItemSide>();
                sides2.Add(clientInterface.CreateAccountOrderItemSide(
                                1,         // Discount ID
                                @"2408",    // GUID
                                1,         // Qty
                                211.00m,  // Base Price
                                @"Item Side2")); // ItemNote))
                sides2.Add(clientInterface.CreateAccountOrderItemSide(
                                1,         // Discount ID
                                @"2409",    // GUID
                                1,         // Qty
                                212.00m,   // Base Price
                                @"Item Side2")); // ItemNote));

                List<DTO_WebOrderItemSide> sides3 = new List<DTO_WebOrderItemSide>();
                sides3.Add(clientInterface.CreateAccountOrderItemSide(
                                1,         // Discount ID
                                @"2408",    // GUID
                                1,         // Qty
                                221.00m,   // Base Price
                                @"Item Side2")); // ItemNote));
                sides3.Add(clientInterface.CreateAccountOrderItemSide(
                                1,         // Discount ID
                                @"2409",    // GUID
                                1,         // Qty
                                222.00m,   // Base Price
                                @"Item Side2")); // ItemNote));
                sides3.Add(clientInterface.CreateAccountOrderItemSide(
                                1,         // Discount ID
                                @"1111",    // GUID
                                1,         // Qty
                                223.00m,   // Base Price
                                @"Item Side2")); // ItemNote));

                // Items
                List<DTO_WebOrderItem> items = new List<DTO_WebOrderItem>();
                items.Add(clientInterface.CreateAccountOrderItem(
                             1,        // Discount ID
                             @"2405",     // GUID
                             1,        // Qty
                             1.00m,    // Base Price
                             @"Item1", // Item Note 
                             options1, // Item Options
                             sides1)); // Item Sides

                items.Add(clientInterface.CreateAccountOrderItem(
                             1,        // Discount ID
                             @"2406",     // GUID
                             2,        // Qty
                             2.00m,    // Base Price
                             @"Item2", // Item Note
                             options2, // Item Options
                             sides2)); // Item Sides

                items.Add(clientInterface.CreateAccountOrderItem(
                             1,        // Discount ID
                             @"2407",     // GUID
                             3,        // Qty
                             3.00m,    // Base Price
                             @"Item3", // Item Note
                             options3, // Item Options
                             sides3)); // Item Sides

                // -- Save ACCOUNT SECTION --
                clientInterface.SaveAccountSection(
                    orderHandle,  // Order Handle
                    1,            // Covers
                    @"0001",      // Member Number
                    @"Jorge",     // Member Name
                    comments, // Comments
                    clientInterface.CreateAccountDelivery( // Delivery Section          
                                        @"Recipient",
                                        @"Street No",
                                        @"Street Name",
                                        @"Suburb",
                                        @"City",
                                        @"Country",
                                        @"Phone No"),
                    clientInterface.CreateAccountPayments(true),
                    items);

                // -- COMMIT ORDER --
                clientInterface.CommitOrder(orderHandle);

                //.................................................................

            }
            catch(Exception exc)
            {
                Console.WriteLine(exc.Message);

                WaitForKey();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        static void WaitForKey()
        {
            ConsoleKeyInfo keyInfo = Console.ReadKey(true);
        }

    }
}
