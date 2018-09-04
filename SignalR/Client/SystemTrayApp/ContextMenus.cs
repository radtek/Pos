﻿using System;
using System.Windows.Forms;
using SystemTrayApp.Properties;
using SystemTrayApp.SignalR;
using Microsoft.AspNet.SignalR.Client;

namespace SystemTrayApp
{
    /// <summary>
    /// 
    /// </summary>
    class ContextMenus
	{
		/// <summary>
		/// Is the About box displayed?
		/// </summary>
		bool isAboutLoaded = false;

		/// <summary>
		/// Creates this instance.
		/// </summary>
		/// <returns>ContextMenuStrip</returns>
		public ContextMenuStrip Create()
		{
			// Add the default menu options.
			ContextMenuStrip menu = new ContextMenuStrip();
			ToolStripMenuItem item;
			ToolStripSeparator sep;

			// Windows Explorer.
			item = new ToolStripMenuItem();
			item.Text = @"Connect";
            item.Name = "Connect";
            item.Click += new EventHandler(SignalRConnect_Click);
			item.Image = Resources.Connected;
			menu.Items.Add(item);
		    // Windows Explorer.
		    item = new ToolStripMenuItem();
		    item.Text = @"Disconnect";
            item.Name = "Disconnect";
            item.Click += new EventHandler(SignalRDisconnect_Click);
		    item.Image = Resources.DisConnected;
		    menu.Items.Add(item);
            // About.
   //         item = new ToolStripMenuItem();
			//item.Text = "About";
			//item.Click += new EventHandler(About_Click);
			//item.Image = Resources.About;
			//menu.Items.Add(item);

			// Separator.
			sep = new ToolStripSeparator();
			menu.Items.Add(sep);

			// Exit.
			item = new ToolStripMenuItem();
			item.Text = "Exit";
			item.Click += new System.EventHandler(Exit_Click);
			item.Image = Resources.Exit;
			menu.Items.Add(item);

			return menu;
		}

		/// <summary>
		/// Handles the Click event of the Explorer control.
		/// </summary>
		/// <param name="sender">The source of the event.</param>
		/// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
		void Explorer_Click(object sender, EventArgs e)
		{
			//Process.Start("explorer", null);
		}
	    void SignalRConnect_Click(object sender, EventArgs e)
	    {
	        if (Client.Connection.State != ConnectionState.Connected)
	        {
	            if (Client.InitializeClient())
	            {
                    ContextMenuHelper helper = new ContextMenuHelper();
                    helper.ToggleOnConnect();
	            }
	        }
        }
	    void SignalRDisconnect_Click(object sender, EventArgs e)
	    {
            if (Client.Connection.State == ConnectionState.Connected)
            {
                if (Client.Disconnect())
                {
                    ContextMenuHelper helper = new ContextMenuHelper();
                    helper.ToggleOnDisconnect();
                }
            }
        }
        /// <summary>
        /// Handles the Click event of the About control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        void About_Click(object sender, EventArgs e)
		{
			if (!isAboutLoaded)
			{
				isAboutLoaded = true;
				new AboutBox().ShowDialog();
				isAboutLoaded = false;
			}
		}

		/// <summary>
		/// Processes a menu item.
		/// </summary>
		/// <param name="sender">The sender.</param>
		/// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
		void Exit_Click(object sender, EventArgs e)
		{
			// Quit without further ado.
			Application.Exit();
		}
	}
}