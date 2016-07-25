using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Windows.Forms;
using System.Drawing;

namespace Summa.Tools
{
    public sealed class Helper
    {
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

        //:::::::::::::::::::::::::::::::::::

        /// <summary>
        /// Returns a InputBox
        /// </summary>
        public DialogResult SiteIDMapInputBox(ref string inSiteID, ref string inSummaSiteID)
        {
            Form form = new Form();

            Label   labelSiteID   = new Label();
            TextBox textBoxSiteID = new TextBox();

            Label   labelSummaSiteID   = new Label();
            TextBox textBoxSummaSiteID = new TextBox();

            Button buttonOk = new Button();
            Button buttonCancel = new Button();

            form.Text = @"Site ID Map";

            labelSiteID.Text   = @"Site ID";
            textBoxSiteID.Text = inSiteID;

            labelSummaSiteID.Text   = @"Summa ID";
            textBoxSummaSiteID.Text = inSummaSiteID;

            buttonOk.Text = "OK";
            buttonCancel.Text = "Cancel";
            buttonOk.DialogResult = DialogResult.OK;
            buttonCancel.DialogResult = DialogResult.Cancel;

            labelSiteID.SetBounds(9, 20, 372, 13);
            textBoxSiteID.SetBounds(12, 36, 372, 20);

            labelSummaSiteID.SetBounds(9, 60, 372, 13);
            textBoxSummaSiteID.SetBounds(12, 76, 372, 20);

            buttonOk.SetBounds(228, 112, 75, 23);
            buttonCancel.SetBounds(309, 112, 75, 23);

            labelSiteID.AutoSize = true;
            textBoxSiteID.Anchor = textBoxSiteID.Anchor | AnchorStyles.Right;

            labelSummaSiteID.AutoSize = true;
            textBoxSummaSiteID.Anchor = textBoxSummaSiteID.Anchor | AnchorStyles.Right;

            buttonOk.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            buttonCancel.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;

            form.ClientSize = new Size(396, 157);
            form.Controls.AddRange(new Control[] { labelSiteID, textBoxSiteID, labelSummaSiteID, textBoxSummaSiteID, buttonOk, buttonCancel });
            form.ClientSize = new Size(Math.Max(300, labelSiteID.Right + 10), form.ClientSize.Height);
            form.FormBorderStyle = FormBorderStyle.FixedDialog;
            form.StartPosition = FormStartPosition.CenterScreen;
            form.MinimizeBox = false;
            form.MaximizeBox = false;
            form.AcceptButton = buttonOk;
            form.CancelButton = buttonCancel;

            DialogResult dialogResult = form.ShowDialog();

            if (dialogResult == DialogResult.OK)
            {
                inSiteID      = textBoxSiteID.Text;
                inSummaSiteID = textBoxSummaSiteID.Text;
            }

            return dialogResult;
        }
    }
}
