using SystemTrayApp.SignalR;

namespace SystemTrayApp
{
    public class ContextMenuHelper
    {
        public ContextMenuHelper()
        {
            
        }

        public void ToggleOnConnect()
        {
            var icon = Client.GetNotifyIcon();
            for (int i = 0; i < icon.ContextMenuStrip.Items.Count; i++)
            {
                if (icon.ContextMenuStrip.Items[i].Name == @"Disconnect")
                {
                    icon.ContextMenuStrip.Items[i].Text = "Disconnect";
                    icon.ContextMenuStrip.Items[i].Enabled = true;
                }
                else if (icon.ContextMenuStrip.Items[i].Name == @"Connect")
                {
                    icon.ContextMenuStrip.Items[i].Text = "Connected";
                    icon.ContextMenuStrip.Items[i].Enabled = false;
                }
            }
        }

        public void ToggleOnDisconnect()
        {
            var icon = Client.GetNotifyIcon();
            for (int i = 0; i < icon.ContextMenuStrip.Items.Count; i++)
            {
                if (icon.ContextMenuStrip.Items[i].Name == @"Connect")
                {
                    icon.ContextMenuStrip.Items[i].Text = "Connect";
                    icon.ContextMenuStrip.Items[i].Enabled = true;
                }
                else if (icon.ContextMenuStrip.Items[i].Name == @"Disconnect")
                {
                    icon.ContextMenuStrip.Items[i].Text = "Disconnected";
                    icon.ContextMenuStrip.Items[i].Enabled = false;
                }
            }
        }
    }
}
