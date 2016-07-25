using System;

using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Runtime.InteropServices;
using System.Net;
using System.Diagnostics;
using System.Threading;
using IntaMate.Properties;
using NLog;
using System.Reflection;


namespace IntaMate
{

    public partial class frmMain : Form
    {

        PMSInterfaceFactory PMSFactory;
        TCPServer TCP;
        private static Logger logger = LogManager.GetCurrentClassLogger();

        public frmMain()
        {
            // Configure Logging.

            PMSFactory = new PMSInterfaceFactory();
            PMSFactory.PMSInterfaceType = IntaMate.Properties.Settings.Default.InterfaceAPI;

            TCP = new TCPServer(IPAddress.Any, Convert.ToInt32(IntaMate.Properties.Settings.Default.ListenPort), PMSFactory);

            InitializeComponent();

            this.noneToolStripMenuItem.Tag = NLog.LogLevel.Off;
            this.debugToolStripMenuItem.Tag = NLog.LogLevel.Debug;
            this.traceToolStripMenuItem.Tag = NLog.LogLevel.Trace;
            this.infoToolStripMenuItem.Tag = NLog.LogLevel.Info;
            this.warningsToolStripMenuItem.Tag = NLog.LogLevel.Warn;
            this.errorsToolStripMenuItem.Tag = NLog.LogLevel.Error;
            this.fatalToolStripMenuItem.Tag = NLog.LogLevel.Fatal;
            UpdateOnlineLoggingLevel();

            List<NLog.LogLevel> MyList = new List<LogLevel>();
            MyList.Add(NLog.LogLevel.Off);
            MyList.Add(NLog.LogLevel.Debug);
            MyList.Add(NLog.LogLevel.Trace);
            MyList.Add(NLog.LogLevel.Info);
            MyList.Add(NLog.LogLevel.Warn);
            MyList.Add(NLog.LogLevel.Error);
            MyList.Add(NLog.LogLevel.Fatal);
            MyList.Sort();

            cbAPI.Items.Clear();
            cbAPI.Items.AddRange(Enum.GetNames(typeof(PMSInterfaceFactory.e_PMSInterfaceType)));

            cbTransport.Items.Clear();
            cbTransport.Items.AddRange(Enum.GetNames(typeof(e_IOType)));

            cbMsgType.Items.Clear();
            cbMsgType.Items.AddRange(Enum.GetNames(typeof(e_PMSMSGType)));

            TCP.Start();
            Online();
            UpdateOnlineLoggingLevel();
        }

        private void Online()
        {
            PMSFactory.PMSInterfaceType = IntaMate.Properties.Settings.Default.InterfaceAPI;
            if (PMSFactory.PMSInterface.Connect())
            {
                LogBox.Clear();
                bool Active = PMSFactory.PMSInterface.IsConnected();
                logger.Info("PMS API Status : " + (Active ? "On line" : "Off Line"));
                if (Active)
                {
                    lbEnabled.Text = "Active";
                    lbEnabled.ForeColor = System.Drawing.Color.Green;
                }
                else
                {
                    lbEnabled.Text = "Disabled";
                    lbEnabled.ForeColor = this.lbEnabled.ForeColor = System.Drawing.Color.Red;
                }
            }
            else
            {
                lbEnabled.Text = "Disabled";
                lbEnabled.ForeColor = this.lbEnabled.ForeColor = System.Drawing.Color.Red;
            }
        }

        ~frmMain()
        {
            TCP.Stop();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            LogBox.Clear();
            Online();
        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == WindowState)
                Hide();
        }

        private void notifyIcon1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            Show();
            WindowState = FormWindowState.Normal;
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            Show();
            WindowState = FormWindowState.Normal;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            LogBox.Clear();
        }

        private void frmMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            IntaMate.Properties.Settings.Default.Save();
            TCP.Stop();
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("Stopping IntaMate will stop MenuMate from being able to post to rooms. Really Quit?", "Are you sure you want to Quit?",
                MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation) == DialogResult.Cancel)
            {
                e.Cancel = true;
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            IntaMate.Properties.Settings.Default.Save();
            TCP = new TCPServer(IPAddress.Any, Convert.ToInt32(IntaMate.Properties.Settings.Default.ListenPort), PMSFactory);
            PMSFactory.PMSInterface.SaveConfig();
        }

        private void cbAPI_SelectedIndexChanged(object sender, EventArgs e)
        {
        }

        private void SetOnlineLoggingLevel(NLog.LogLevel Level, bool Checked)
        {
            foreach (NLog.Config.LoggingRule rule in LogManager.Configuration.LoggingRules)
            {
                // Only update the appropriate rule
                // TODO don't just check the 0-index target!
                foreach (NLog.Targets.Target CurrentTarget in rule.Targets)
                {
                    if (Level != NLog.LogLevel.Off)
                    {
                        if (Checked)
                        {
                            rule.EnableLoggingForLevel(Level);
                        }
                        else
                        {
                            rule.DisableLoggingForLevel(Level);
                        }
                    }
                    else if (Checked)
                    {
                        rule.DisableLoggingForLevel(NLog.LogLevel.Trace);
                        rule.DisableLoggingForLevel(NLog.LogLevel.Debug);
                        rule.DisableLoggingForLevel(NLog.LogLevel.Info);
                        rule.DisableLoggingForLevel(NLog.LogLevel.Warn);
                        rule.DisableLoggingForLevel(NLog.LogLevel.Error);
                        rule.DisableLoggingForLevel(NLog.LogLevel.Fatal);
                    }
                }
            }
            LogManager.ReconfigExistingLoggers();
        }

        private void UpdateOnlineLoggingLevel()
        {
            foreach (NLog.Config.LoggingRule rule in LogManager.Configuration.LoggingRules)
            {
                // Only update the appropriate rule
                // TODO don't just check the 0-index target!
                foreach (NLog.Targets.Target CurrentTarget in rule.Targets)
                {
                    foreach (ToolStripItem subItem in this.toolStripOnlineLogging.DropDownItems)
                    {
                        ToolStripMenuItem menuItem = subItem as ToolStripMenuItem;
                        if (menuItem != null && menuItem.Tag != null && ((NLog.LogLevel)menuItem.Tag != NLog.LogLevel.Off))
                        {
                            if (rule.IsLoggingEnabledForLevel((NLog.LogLevel)menuItem.Tag))
                            {
                                menuItem.Checked = true;
                                this.noneToolStripMenuItem.Checked = false;
                            }
                            else
                            {
                                menuItem.Checked = false;
                            }
                        }
                    }
                }
            }
        }

        private void enabledToolStripMenuItem_CheckStateChanged(object sender, EventArgs e)
        {
            ToolStripMenuItem item = sender as ToolStripMenuItem;
            NLog.LogLevel selectedLevel = (NLog.LogLevel)item.Tag;
            SetOnlineLoggingLevel(selectedLevel, item.Checked);
            UpdateOnlineLoggingLevel();
        }

        private void allToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item = sender as ToolStripMenuItem;
            SetOnlineLoggingLevel(NLog.LogLevel.Trace, true);
            SetOnlineLoggingLevel(NLog.LogLevel.Debug, true);
            SetOnlineLoggingLevel(NLog.LogLevel.Info, true);
            SetOnlineLoggingLevel(NLog.LogLevel.Warn, true);
            SetOnlineLoggingLevel(NLog.LogLevel.Error, true);
            SetOnlineLoggingLevel(NLog.LogLevel.Fatal, true);
            UpdateOnlineLoggingLevel();
        }

        private void tbIPAddress_TextChanged(object sender, EventArgs e)
        {
        }

        private void cbTransport_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (IntaMate.Properties.Settings.Default.IOInterfaceType == IntaMate.e_IOType.proto_TCPIP.ToString())
            {
                lbeCommport.Text = "IP Address";
                lbeCommport.Visible = true;
                cbComPort.Visible = false;
                tbIPAddress.Visible = true;
                tbPort.Visible = true;
                lbPort.Visible = true;
                tbListenPort.Visible = true;
                lbListenPort.Visible = true;
                tbTimeOut.Visible = true;
                PMSFactory.PMSInterface.SetIOType(e_IOType.proto_TCPIP);
            }
            else if (IntaMate.Properties.Settings.Default.IOInterfaceType == IntaMate.e_IOType.proto_RS232.ToString())
            {
                lbeCommport.Text = "Comm Port";
                lbeCommport.Visible = true;
                cbComPort.Visible = true;
                tbIPAddress.Visible = false;
                tbPort.Visible = false;
                lbPort.Visible = false;
                tbListenPort.Visible = true;
                lbListenPort.Visible = true;
                tbTimeOut.Visible = false;
                PMSFactory.PMSInterface.SetIOType(e_IOType.proto_RS232);
            }
            else if (IntaMate.Properties.Settings.Default.IOInterfaceType == IntaMate.e_IOType.proto_Stub.ToString())
            {
                lbeCommport.Text = "I/O Stub";
                lbeCommport.Visible = true;
                cbComPort.Visible = false;
                tbIPAddress.Visible = false;
                tbPort.Visible = false;
                lbPort.Visible = false;
                tbListenPort.Visible = true;
                lbListenPort.Visible = true;
                tbTimeOut.Visible = false;
                PMSFactory.PMSInterface.SetIOType(e_IOType.proto_Stub);
            }
        }

        private void cbMsgType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (IntaMate.Properties.Settings.Default.InterfaceMsgType == IntaMate.e_PMSMSGType.msgNone.ToString())
            {
                PMSFactory.PMSInterface.SetMsgType(e_PMSMSGType.msgNone);
            }
            else if (IntaMate.Properties.Settings.Default.InterfaceMsgType == IntaMate.e_PMSMSGType.msgFidelio.ToString())
            {
                PMSFactory.PMSInterface.SetMsgType(e_PMSMSGType.msgFidelio);
            }
            else if (IntaMate.Properties.Settings.Default.InterfaceMsgType == IntaMate.e_PMSMSGType.msgHirum.ToString())
            {
                PMSFactory.PMSInterface.SetMsgType(e_PMSMSGType.msgHirum);
            }
        }
    }
}