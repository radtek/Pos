using System;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;

using Menumate.WinServices.Implementation.Contracts.DTO;

namespace Menumate.WinServices.Client.FloorPlan
{
    class Helper
    {
        //:::::::::::::::::::::::::::::::::

        Helper() 
        { 
        }

        public static Helper Instance
        {
            get
            {
               return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly Helper instance = new Helper();
        }

        //:::::::::::::::::::::::::::::::::

        /// <summary>
        /// Returns a InputBox
        /// </summary>
        public static DialogResult InputBox(string title, string promptText, ref string value)
        {
            Form form = new Form();
            Label label = new Label();
            TextBox textBox = new TextBox();
            Button buttonOk = new Button();
            Button buttonCancel = new Button();

            form.Text = title;
            label.Text = promptText;
            textBox.Text = value;

            buttonOk.Text = "OK";
            buttonCancel.Text = "Cancel";
            buttonOk.DialogResult = DialogResult.OK;
            buttonCancel.DialogResult = DialogResult.Cancel;

            label.SetBounds(9, 20, 372, 13);
            textBox.SetBounds(12, 36, 372, 20);
            buttonOk.SetBounds(228, 72, 75, 23);
            buttonCancel.SetBounds(309, 72, 75, 23);

            label.AutoSize = true;
            textBox.Anchor = textBox.Anchor | AnchorStyles.Right;
            buttonOk.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            buttonCancel.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;

            form.ClientSize = new Size(396, 107);
            form.Controls.AddRange(new Control[] { label, textBox, buttonOk, buttonCancel });
            form.ClientSize = new Size(Math.Max(300, label.Right + 10), form.ClientSize.Height);
            form.FormBorderStyle = FormBorderStyle.FixedDialog;
            form.StartPosition = FormStartPosition.CenterScreen;
            form.MinimizeBox = false;
            form.MaximizeBox = false;
            form.AcceptButton = buttonOk;
            form.CancelButton = buttonCancel;

            DialogResult dialogResult = form.ShowDialog();
            value = textBox.Text;
            return dialogResult;
        }

        /// <summary>
        /// Returns a TableInputBox
        /// </summary>
        public static DialogResult TableInputBox(string title, string promptText, string promptTextAux, ref string tableName, ref int tableShape, ref int tableNumber)
        {
            Form form = new Form();
            Label label = new Label();
            TextBox textBox = new TextBox();
            Label label1 = new Label();
            TextBox textBox1 = new TextBox();
            Button buttonOk = new Button();
            Button buttonCancel = new Button();

            GroupBox groupBox = new GroupBox();
            RadioButton rectTable = new RadioButton();
            RadioButton roundTable = new RadioButton();

            textBox1.KeyDown += textBoxTableNumber_KeyDown;

            //..............................

            form.Text = title;
            label.Text = promptText;
            textBox.Text = tableName;
            label1.Text = promptTextAux;
            textBox1.Text = string.Format(@"{0}", tableNumber);

            groupBox.Text = @"Table Shape";
            rectTable.Text = @"Square Table";
            roundTable.Text = @"Round Table";

            //...............................

            buttonOk.Text = "OK";
            buttonCancel.Text = "Cancel";
            buttonOk.DialogResult = DialogResult.OK;
            buttonCancel.DialogResult = DialogResult.Cancel;

            //................................

            label.SetBounds(9, 20, 191, 13);
            textBox.SetBounds(12, 36, 200, 20);
            label1.SetBounds(150, 20, 191, 13);
            textBox1.SetBounds(153, 36, 120, 20);
            buttonOk.SetBounds(228, 72 + 50, 75, 23);
            buttonCancel.SetBounds(309, 72 + 50, 75, 23);
            //buttonOk.SetBounds(228, 72, 75, 23);
            //buttonCancel.SetBounds(309, 72, 75, 23);

            groupBox.SetBounds(9, 60, 352, 50);
            rectTable.SetBounds(5, 15, 100, 30);
            roundTable.SetBounds(140, 15, 100, 30);

            groupBox.Controls.AddRange(new Control[] { rectTable, roundTable });

            //................................

            label.AutoSize = true;
            textBox.Anchor = textBox.Anchor | AnchorStyles.Right;
            buttonOk.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            buttonCancel.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;

            rectTable.Checked = (tableShape == 0);
            roundTable.Checked = !rectTable.Checked;

            //.................................

            form.ClientSize = new Size(396, 107 + 50);
            form.Controls.AddRange(new Control[] { label, textBox, label1, textBox1, buttonOk, buttonCancel, groupBox });
            form.ClientSize = new Size(Math.Max(300, label.Right + 10), form.ClientSize.Height);
            form.FormBorderStyle = FormBorderStyle.FixedDialog;
            form.StartPosition = FormStartPosition.CenterScreen;
            form.MinimizeBox = false;
            form.MaximizeBox = false;
            form.AcceptButton = buttonOk;
            form.CancelButton = buttonCancel;

            //..................................

            DialogResult dialogResult;

            bool tableNumberOK = false;
            do
            {
                dialogResult = form.ShowDialog();
                tableName = textBox.Text;
                tableNumber = Convert.ToInt16(textBox1.Text);

                if (rectTable.Checked)
                {
                    tableShape = 0;
                }
                else
                {
                    tableShape = 1;
                }

                if (dialogResult == DialogResult.OK)
                {
                    if (tableNumber == 0)
                    {
                        MessageBox.Show(@"Table Number cannot be zero");
                    }
                    else
                    {
                        tableNumberOK = checkForDuplicateTableNumber(tableNumber);
                        if (!tableNumberOK)
                        {
                            MessageBox.Show(@"Table Number already exist. Table Number cannot be duplicated");
                        }
                    }
                }           
            }
            while ((dialogResult != DialogResult.Cancel) && (((dialogResult == DialogResult.OK) && (tableNumber == 0)) || ((dialogResult == DialogResult.OK) && !tableNumberOK ) ));

            return dialogResult;
        }

        private static void textBoxTableNumber_KeyDown(object sender, KeyEventArgs e)
        {
            e.SuppressKeyPress = (Helper.keyIsALetter(e.KeyCode));
        }

        public static bool checkForDuplicateTableNumber(int inTableNumber)
        {
            bool result = true;

            EditingFloorPlan editingFP = EditingFloorPlan.Instance;

            DTOReservable[] tablesInLocation = editingFP.tablesInLocation;

            foreach (DTOReservable table in tablesInLocation)
            {
                if (table.Number == inTableNumber)
                {
                    result = false; // Table Number already exists
                    break;
                }
            }

            return result;
        }

        public static int nextTableNumber()
        {
            EditingFloorPlan editingFP = EditingFloorPlan.Instance;

            DTOReservable[] tablesInLocation = editingFP.tablesInLocation;

            int max = 0;
            if (tablesInLocation.Length > 0)
            {
                max = tablesInLocation[0].Number;
                foreach (DTOReservable table in tablesInLocation)
                {
                    if (table.Number > max)
                    {
                        max = table.Number;
                    }
                }

            }

            return max + 1;
        }

        public static void reArrangeTableNumbers()
        {
            EditingFloorPlan editingFP = EditingFloorPlan.Instance;

            DTOReservable[] tablesInLocation = editingFP.tablesInLocation;

            for (int i = 0; i < tablesInLocation.Length; i++)
            {
                tablesInLocation[i].Number = i+1;
            }

            editingFP.signalTablesChanged();
        }

        public static byte[] imageToByteArray(Image imageIn, ImageFormat inImgFormat)
        {
            MemoryStream ms = new MemoryStream();
            imageIn.Save(ms, inImgFormat);
            return ms.ToArray();
        }

        public static Image byteArrayToImage(byte[] byteArrayIn)
        {
            return Image.FromStream(new MemoryStream(byteArrayIn));
        }

        public static Timer createTimer(int inInterval)
        {
            Timer result = new Timer();

            result.Interval = inInterval;
            result.Enabled = true;
            result.Stop();

            return result;
        }

        public static bool keyIsANumber(Keys inKey)
        {
            return (inKey >= Keys.D0) && (inKey <= Keys.D9);
        }

        public static bool keyIsALetter(Keys inKey)
        {
            return (inKey >= Keys.A) && (inKey <= Keys.Z);
        }

        public static bool keyIsBackSpace(Keys inKey)
        {
            return inKey == Keys.Back;
        }

        public static bool keyIsDelete(Keys inKey)
        {
            return inKey == Keys.Delete;
        }
    }
}
