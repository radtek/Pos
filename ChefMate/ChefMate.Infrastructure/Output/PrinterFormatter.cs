using System;
using System.Windows.Navigation;
using Chefmate.Core;
using Chefmate.Core.Model;

namespace Chefmate.Infrastructure.Output
{
    class PrinterFormatter
    {
        private string _chitValue = "Chit:  {0}";
        private string _customerName = "Name:  {0}";
        private string _customerEmail = "Email:  {0}";
        private string _customerPhone = "Phone:  {0}";
        private string _customerAddress = "Address:  {0}";
        private string _expectedTime = "Expected Time:  {0}";
        private string _paymentStatus = "Payment Status:  {0}";

        public PrinterFormatter()
        {
        }

        /*  
         *  The following methods are current using a hard coded docket layout that will likely be subject to change in the future.
         *  For however the layout of the docket will be the following:
         *      Chit Information
         *      Kitchen Timings
         *        Full Line
         *          Item Name
         *        Full Line
         *      Server Name
         *      Date
         *        Cut
         */
        public DocketLayout BuildDocketLayout(Order inOrder)
        {
            DocketLayout docketLayout = new DocketLayout();
            BuildHeader(docketLayout, inOrder);
            foreach (var group in inOrder.ServingCourseGroups)
            {
                foreach (var item in group.Items)
                {
                    BuildItem(docketLayout, item);
                }
            }
            BuildFooter(docketLayout, inOrder);

            return docketLayout;
        }

        public DocketLayout BuildDocketLayout(Group inGroup)
        {
            DocketLayout docketLayout = new DocketLayout();
            BuildHeader(docketLayout, inGroup.Order);
            foreach (var item in inGroup.Items)
            {
                BuildItem(docketLayout, item);
            }
            BuildFooter(docketLayout, inGroup.Order);

            return docketLayout;
        }

        public DocketLayout BuildDocketLayout(Item inItem)
        {
            DocketLayout docketLayout = new DocketLayout();
            BuildHeader(docketLayout, inItem.SCourseGroup.Order);
            BuildItem(docketLayout, inItem);
            BuildFooter(docketLayout, inItem.SCourseGroup.Order);
            return docketLayout;
        }

        private void BuildHeader(DocketLayout outDocketLayout, Order inOrder)
        {
            // Chit Information
            if (!string.IsNullOrWhiteSpace(inOrder.ChitValue))
                outDocketLayout.AddInstruction(GetInstruction(string.Format(_chitValue, inOrder.ChitValue)));
            if (inOrder.OrderAction == ChefmateConstants.WebOrderAction)
            {
                BuildDocketLayoutForWebOrder(outDocketLayout, inOrder);
            }
            else
            {
                // Kitchen Timings
                outDocketLayout.AddInstruction(GetInstruction("IN  " + inOrder.ArrivalTime.ToString("T") + "\n" + "OUT " + DateTime.Now.ToString("T"), true));
            }


        }

        private void BuildDocketLayoutForWebOrder(DocketLayout outDocketLayout, Order inOrder)
        {
            // Kitchen Timings
            outDocketLayout.AddInstruction(GetInstruction("IN  " + inOrder.ArrivalTime.ToString("T") + "\n" + "OUT " + DateTime.Now.ToString("T")));
            // Customer Name
            outDocketLayout.AddInstruction(GetInstruction(string.Format(_customerName, inOrder.CustomerName)));
            // Customer Email
            outDocketLayout.AddInstruction(GetInstruction(string.Format(_customerEmail, inOrder.CustomerEmail)));
            // Customer Phone
            outDocketLayout.AddInstruction(GetInstruction(string.Format(_customerPhone, inOrder.CustomerPhone)));
            // Customer Address
            if (!string.IsNullOrWhiteSpace(inOrder.CustomerAddress))
            {
                outDocketLayout.AddInstruction(GetInstruction(string.Format(_customerAddress, inOrder.CustomerAddress)));
            }
            // Expected Time 
            outDocketLayout.AddInstruction(GetInstruction(string.Format(_expectedTime, inOrder.DeliveryTime.ToString("T"))));
            // Payment Status 
            outDocketLayout.AddInstruction(GetInstruction(string.Format(_paymentStatus, inOrder.PaymentStatus), true));
        }

        private PrinterInstruction GetInstruction(string instructionText, bool addLineAfter = false, bool addLineBefore = false, bool cutAfter = false)
        {
            var currentInstruction = new PrinterInstruction();
            currentInstruction.Value = instructionText;
            currentInstruction.DrawLineAfter = addLineAfter;
            currentInstruction.DrawLineBefore = addLineBefore;
            currentInstruction.CutAfter = cutAfter;
            return currentInstruction;
        }


        private void BuildItem(DocketLayout outDocketLayout, Item inItem)
        {
            PrinterInstruction currentInstruction;
            // Item Name
            currentInstruction = new PrinterInstruction();
            currentInstruction.Value = inItem.Name;
            outDocketLayout.AddInstruction(currentInstruction);
            foreach (var side in inItem.Sides)
            {
                currentInstruction = new PrinterInstruction();
                currentInstruction.Value = " > " + side.Name;
                outDocketLayout.AddInstruction(currentInstruction);
            }
            foreach (var option in inItem.Options)
            {
                currentInstruction = new PrinterInstruction();
                currentInstruction.Value = option.IsPlus ? " + " : " - ";
                currentInstruction.Value += option.Name;
                outDocketLayout.AddInstruction(currentInstruction);
            }
        }

        private void BuildFooter(DocketLayout outDocketLayout, Order inOrder)
        {
            // Server Name
            outDocketLayout.AddInstruction(GetInstruction(inOrder.ServerName, addLineBefore: true));
            // Date
            outDocketLayout.AddInstruction(GetInstruction(DateTime.Now.Date.ToString("d"), cutAfter: true));
        }
    }
}
