using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WebMateTestProgram
{
    public partial class ItemsForm : Form
    {
        private List<Item> items;
        private DataTable itemDataTable;

        public ItemsForm()
        {
            items = new List<Item>();

            InitializeComponent();
        }

        #region Public Methods
        public int GetItemsCount()
        {
            return items.Count;
        }

        public Item GetItemAtIndex(int i)
        {
            return items.ElementAt(i);
        }

        public void Clear()
        {
            items.Clear();
            itemDataTable.Clear();
        }
        #endregion

        #region Private Methods
        private void addItem(Item inItem)
        {
            items.Add(inItem);
            ItemsGrid.Rows.Add(inItem.ThirdPartyCode, inItem.Price.ToString("C2"), inItem.Qty);
        }

        private void addSideItem(SideItem inSideItem)
        {
            Item item = getSelectedItem();
            item.AddSideItem(inSideItem);
            SideItemsGrid.Rows.Add(inSideItem.ThirdPartyCode, inSideItem.Price.ToString("C2"), inSideItem.Qty);
        }

        private Item getSelectedItem()
        {
            if (ItemsGrid.SelectedCells.Count != 0)
            {
                return items.ElementAt(ItemsGrid.SelectedCells[0].RowIndex);
            }
            else
            {
                return null;
            }
        }

        private SideItem getSelectedSideItem()
        {
            if (ItemsGrid.SelectedCells.Count != 0)
            {
                return getSelectedItem().GetSideItemAtIndex(SideItemsGrid.SelectedCells[0].RowIndex);
            }
            else
            {
                return null;
            }
        }

        private void displaySideItems()
        {
            SideItemsGrid.Rows.Clear();
            if (ItemsGrid.SelectedCells.Count != 0)
            {
                Item item = getSelectedItem();
                for (int i = 0; i < item.GetSideItemsCount(); i++)
                {
                    SideItem sideItem = item.GetSideItemAtIndex(i);
                    SideItemsGrid.Rows.Add(sideItem.ThirdPartyCode, sideItem.Price.ToString("C2"), sideItem.Qty);
                }
            }
        }

        private void deleteSelectedItem()
        {
            int itemIndex = ItemsGrid.CurrentCell.RowIndex;
            ItemsGrid.Rows.RemoveAt(itemIndex);
            items.RemoveAt(itemIndex);
            displaySideItems();
        }

        private void deleteSelectedSideItem()
        {
            SideItemsGrid.Rows.RemoveAt(SideItemsGrid.SelectedCells[0].RowIndex);
            getSelectedItem().RemoveSideItem(getSelectedSideItem());
        }
        #endregion

        #region Event Handlers
        private void AddItemButton_Click(object sender, EventArgs e)
        {
            AddItemForm addItemForm = new AddItemForm();
            addItemForm.ShowDialog(this);
            if (!addItemForm.Canceled)
            {
                try
                {
                    string tpc = addItemForm.ThirdPartyCodeTextBox.Text;
                    decimal price = Convert.ToDecimal(addItemForm.PriceTextBox.Text);
                    uint qty = Convert.ToUInt32(addItemForm.QtyTextBox.Text);
                    addItem(new Item(tpc, price, qty));
                }
                catch (FormatException formatException)
                {
                    //TODO: report format exception to user
                }
            }
            addItemForm.Dispose();
        }

        private void AddSideButton_Click(object sender, EventArgs e)
        {
            AddItemForm addItemForm = new AddItemForm();
            addItemForm.Text = "Add Side Item";
            addItemForm.ShowDialog(this);
            if (!addItemForm.Canceled)
            {
                try
                {
                    string tpc = addItemForm.ThirdPartyCodeTextBox.Text;
                    decimal price = Convert.ToDecimal(addItemForm.PriceTextBox.Text);
                    uint qty = Convert.ToUInt32(addItemForm.QtyTextBox.Text);
                    addSideItem(new SideItem(tpc, price, qty));
                }
                catch (FormatException formatException)
                {
                    //TODO: report format exception to user
                }
            }
            addItemForm.Dispose();
        }

        private void DoneButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void ItemsGrid_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            displaySideItems();
        }

        private void DeleteItemButton_Click(object sender, EventArgs e)
        {
            deleteSelectedItem();
        }

        private void DeleteSideButton_Click(object sender, EventArgs e)
        {
            deleteSelectedSideItem();
        }

        private void SideItemsGrid_SelectionChanged(object sender, EventArgs e)
        {
            DeleteSideButton.Enabled = SideItemsGrid.SelectedCells.Count != 0;
        }

        private void ItemsGrid_SelectionChanged(object sender, EventArgs e)
        {
            bool itemSelected = ItemsGrid.SelectedCells.Count != 0;
            DeleteItemButton.Enabled = itemSelected;
            AddSideButton.Enabled = itemSelected;
            displaySideItems();
        }
        #endregion
    }
}
