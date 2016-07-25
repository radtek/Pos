using System;
using Chefmate.Core.Model;

namespace Chefmate.Infrastructure.Output
{
    class PrinterFormatter
    {
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
            PrinterInstruction currentInstruction;
            // Chit Information
            currentInstruction = new PrinterInstruction();
            currentInstruction.Value = inOrder.ChitValue;
            outDocketLayout.AddInstruction(currentInstruction);
            // Kitchen Timings
            currentInstruction = new PrinterInstruction();
            currentInstruction.Value = "IN  " + inOrder.SaleStartTime.ToString("T") + "\n" + "OUT " + DateTime.Now.ToString("T");
            currentInstruction.DrawLineAfter = true;
            outDocketLayout.AddInstruction(currentInstruction);
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
            PrinterInstruction currentInstruction;
            // Server Name
            currentInstruction = new PrinterInstruction();
            currentInstruction.Value = inOrder.ServerName;
            currentInstruction.DrawLineBefore = true;
            outDocketLayout.AddInstruction(currentInstruction);
            // Date
            currentInstruction = new PrinterInstruction();
            currentInstruction.Value = DateTime.Now.Date.ToString("d");
            currentInstruction.CutAfter = true;
            outDocketLayout.AddInstruction(currentInstruction);
        }
    }
}
