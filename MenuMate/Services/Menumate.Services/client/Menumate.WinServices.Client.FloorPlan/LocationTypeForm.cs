using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Menumate.WinServices.Client.FloorPlan
{
    public partial class LocationTypeForm : Form
    {
        public enum ImageOrientation
        {
            Portrait,
            Lanscape
        };

        public LocationTypeForm()
        {
            InitializeComponent();
        }

        public ImageOrientation imageOrientation
        {
            get
            {
                return this.radioButtonLandscape.Checked ? ImageOrientation.Lanscape : ImageOrientation.Portrait;
            }
            set
            {
                this.radioButtonLandscape.Checked = value == ImageOrientation.Lanscape;
            }
        }

    }
}
