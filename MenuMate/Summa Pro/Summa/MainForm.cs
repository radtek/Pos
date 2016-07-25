using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Xml;
using System.Diagnostics;

using LogFileManager;
using ConfigFileManager;

using Summa.Tools;
using SummaCL;

using XMLManager;

using System.Runtime.InteropServices;

//Notification process must be reset in order for Summa to be able to send an email in case on failure

namespace Summa
{
    using SiteIDMap = Dictionary<string, string>;

    public enum InitConfigResult
    {
        NewConfigFileCreated,
        ConfigFileLoaded,
        ConfigFileLoadFailed
    };

    public partial class MainForm : Form
    {
        InitConfigResult _initConfigResult;
        ConfigurationFileManager.ConfigFileError _configFileError;
        string _configFileErrorStr;

        Timer _startupTimer = null;

        FileLogger               _singletonLogger     = FileLogger.Instance;
        ConfigurationFileManager _singletonConfigFile = ConfigurationFileManager.Instance;

        Server server = null;

        // Create a Font object for the node tags.
        Font tagFont = new Font("Helvetica", 8, FontStyle.Bold);

        public MainForm()
        {
            InitializeComponent();
        }

        //.................................
                    
        [DllImport("user32.dll", SetLastError = true)]
        static extern int GetWindowLong(IntPtr hWnd, int nIndex);
        [DllImport("user32.dll", SetLastError = true)]
        static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);

        private const int TVS_NOTOOLTIPS = 0x80;
        private const int GWL_STYLE = (-16);

        private void MainForm_Load(object sender, EventArgs e)
        {
            this.labelVersion.Text = "Version VERSTR";

            initLogger();
            initConfigFileManager();

            _startupTimer = new Timer();
            _startupTimer.Interval = 1000;
            _startupTimer.Tick += new EventHandler(startup_Tick);
            _startupTimer.Start();

            //...............................

            int dwStyle = GetWindowLong(treeViewErrorMailBox.Handle, GWL_STYLE);
            if ((dwStyle & TVS_NOTOOLTIPS) == 0)
                SetWindowLong(treeViewErrorMailBox.Handle, GWL_STYLE, dwStyle | TVS_NOTOOLTIPS);
        }

        //.................................
        private void MainForm_Shown(object sender, EventArgs e)
        {
            //MessageBox.Show(string.Format(@"Summa Proccess ID: {0}", Process.GetCurrentProcess().Id));
        }

        //.................................

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("Quitting will stop sales information from being processed for Summa. Really Quit?", "Are you sure you want to Quit?",
                                    MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation) == DialogResult.Cancel)
            {
                e.Cancel = true;
            }
            else
            {
                if(server != null)
                {
                    server.Stop();
                }
            }
        }

        //.................................

        void startup_Tick(object sender, EventArgs e)
        {
            _startupTimer.Stop();

            if (_singletonConfigFile.LocationName == @"")
            {
                _singletonLogger.LogWarning(@"A name must be assigned to this location.");
            }
            else
            {
                this.Text = string.Format(@"Summa Pro: {0}", _singletonConfigFile.LocationName);

                _singletonLogger.setMailLoggerSubject(string.Format(@"Summa failed in location: {0}", _singletonConfigFile.LocationName));
            }
            
            //......................................

            switch (_initConfigResult)
            {
                case InitConfigResult.ConfigFileLoadFailed:
                {
                    tabPage2.Visible = false;
                    tabPage3.Visible = false;
                    tabPage4.Visible = false;
                    panelUploadNow.Visible = false;
                    panelPause.Visible = false;
                    panelClearLogs.Visible = false;
                    panelOpenUploadFolder.Visible = false;

                    return;
                } 

                case InitConfigResult.NewConfigFileCreated:
                {
                    _singletonLogger.Log(@"A new Configuration File has been created.");

                    MessageBox.Show(@"A new Config File has been creted.  Make sure the Site ID mapping is right");
                    tabControl1.SelectedIndex = 1;
                } break;

                case InitConfigResult.ConfigFileLoaded:
                {
                    if (_configFileError != ConfigurationFileManager.ConfigFileError.None)
                    {
                        _singletonLogger.LogWarning(_configFileErrorStr);
                    }

                    _singletonLogger.Log(@"The Configuration File has been loaded.");
                } break;
            }

            //......................................

            initConfigGUI();
            if (initSummaServer())
            {
                Use_Notify(@"Summa Pro has successfuly started");
            }
            else
            {
                Use_Notify(@"Summa Pro: Server failed to start");
            }

            //......................................

            panelUploadNow.Visible = server != null;
            panelPause.Visible = server != null;

            openTreeViewErrorMailBox();

            setAllControlsAsSaved();
        }

        //.................................

        void initLogger()
        {
            _singletonLogger.createLoggers();
 
            _singletonLogger.setOnErrorFunc(onErrorFunc);
        }

        //.................................

        void initConfigFileManager()
        {
            _singletonConfigFile.setNotifNewConfigFileCreatedFunc(notifNewConfigFileCreated);
            _singletonConfigFile.setNotifConfigFileLoadedFun(notifConfigFileLoaded);
            _singletonConfigFile.setNotifConfigFileLoadFailedFunc(notifConfigFileLoadFailed);

            _singletonConfigFile.initConfigFileManager();

            _singletonLogger.NotificationsEnabled = _singletonConfigFile.EnableNotifications == @"true";
        }

        //.................................

        bool initSummaServer()
        {
            StartupPath.text = Application.StartupPath; // Startup Path used internally by SummaCL Class Library

            server = new Server(LoadSettings());

            if (server.ServerReady)
            {
                server.Start();

                return true;
            }
            else
            {
                return false;
            }
        }

        //.......................................

        void resetSummaserver()
        {
            if (!server.stopped)
            {
                server.ResetServer();
                server.Start();

                //.............................

                this.panelPause.BackgroundImage = Image.FromFile(@".\Resources\pause snow80 tangerine80.png");
                linkLabelPause.Text = @"Pause";
                labelPause.Text = @"Stop receiving request from Menumate";
            }
            else
            {
                server.ResetServer();

                //.............................

                this.panelPause.BackgroundImage = Image.FromFile(@".\Resources\go snow80 tangerine80.png");
                linkLabelPause.Text = @"Go";
                labelPause.Text = @"Start receiving request from Menumate";
            }
        }

        //.......................................

        private void Use_Notify(string inBalloonTipText)
        {
            notifyIcon1.BalloonTipText = inBalloonTipText;
            notifyIcon1.BalloonTipTitle = "Summa Pro";
            notifyIcon1.ShowBalloonTip(1);
        }

        //.......................................

        private XmlDocument LoadSettings()
        {
            XmlDocument doc = null;
            try
            {
                doc = new XmlDocument();
                XmlTextReader reader = new XmlTextReader(_singletonConfigFile.ConfigPath);
                reader.Read();
                doc.Load(reader);

                reader.Close();

                return doc;
            }
            catch (Exception e)
            {
                //Console.WriteLine("Error loading Summa settings file: {0}", e.ToString());
                throw;
            }
        }

        //.................................

        private void richTextBoxError_TextChanged(object sender, EventArgs e)
        {
            //this.toolStripStatusLabelErrortxt.Text = ((RichTextBox)sender).Lines[((RichTextBox)sender).Lines.Length - 2];
        }

        //.................................

        delegate void notifFunc();
        FileLogger.LogErrorLevel _errorLevel;
        FileLogger.LogErrorLevelFile _errorLevelFile;
        string _timeStr;
        string _errorStr;
        string _errorStrDetail;
        bool _sent;

        private void onErrorFunc(FileLogger.LogErrorLevel inErrorLevel, string inTimeStr, FileLogger.LogErrorLevelFile inErrorLevelFile, string inErrorStr, string inErrorStrDetail, bool inSent)
        {
            if (this.treeViewErrorMailBox.InvokeRequired)
            {
                _errorLevel = inErrorLevel;
                _timeStr = inTimeStr;
                _errorLevelFile = inErrorLevelFile;
                _errorStr = inErrorStr;
                _errorStrDetail = inErrorStrDetail;
                _sent = inSent;

                this.treeViewErrorMailBox.Invoke(new notifFunc(onErrorFuncInvoke));

                return;
            }

            addErrorNode(inErrorLevel, inTimeStr, inErrorLevelFile, inErrorStr, inErrorStrDetail, inSent);            
        }

        private void onErrorFuncInvoke()
        {
            addErrorNode(_errorLevel, _timeStr, _errorLevelFile, _errorStr, _errorStrDetail, _sent);
        }

        private void openTreeViewErrorMailBox()
        {
            XmlDocument errorMailBox = _singletonLogger.CurrentErrorMailBox;
            treeViewErrorMailBox.Nodes.Clear();

            TreeNode root = treeViewErrorMailBox.Nodes.Add(@"Error Notification Outbox");
            int lockedEntryCount = 0;

            XmlNodeList entries = XMLDocManager.GetNodeList(errorMailBox, @"Entries/Entry");
            foreach(XmlNode entry in entries)
            {
                string entryName = _singletonLogger.GetLogErrorLevelStr((FileLogger.LogErrorLevel)Convert.ToInt32(entry.Attributes[@"id"].Value));

                TreeNode treeEntry = root.Nodes.Add(string.Format(@"Entry - {0}", entryName));
                XmlNodeList errors = XMLDocManager.GetNodeList(entry, @"Error");

                treeEntry.ImageIndex = 1;
                treeEntry.SelectedImageIndex = 1;

                foreach (XmlNode error in errors)
                {
                    TreeNode treeError;

                    if (error.Attributes[@"file"].Value == @"" )
                    {
                        treeError = treeEntry.Nodes.Insert(0, string.Format(@"{0}    {1}    {2}", error.Attributes[@"time"].Value, error.Attributes[@"name"].Value, error.Attributes[@"detail"].Value));
                        //treeError = treeEntry.Nodes.Add(string.Format(@"Time: {0}  Error: {1}  Detail: {2}", error.Attributes[@"time"].Value, error.Attributes[@"name"].Value, error.Attributes[@"detail"].Value));
                    }
                    else
                    {
                        treeError = treeEntry.Nodes.Insert(0, string.Format(@"{0}    {1}    {2}    {3}", error.Attributes[@"time"].Value, error.Attributes[@"file"].Value, error.Attributes[@"name"].Value, error.Attributes[@"detail"].Value));
                        //treeError = treeEntry.Nodes.Add(string.Format(@"Time: {0}  File: {1}  Error: {2}  Detail: {3}", error.Attributes[@"time"].Value, error.Attributes[@"file"].Value, error.Attributes[@"name"].Value, error.Attributes[@"detail"].Value));
                    }

                    treeError.ImageIndex = 2;
                    treeError.SelectedImageIndex = 2;                    //treeError.ToolTipText = @"";
                   
                }

                if(treeEntry.Nodes.Count > 0)
                {
                    lockedEntryCount += 1;
                    treeEntry.Text = string.Format(@"Entry - {0} [locked]", entryName);
                    treeEntry.ImageIndex = 3;
                    treeEntry.SelectedImageIndex = 3;
                }
            }


            if (lockedEntryCount > 0)
            {
                if (lockedEntryCount == 1)
                {
                    root.Text = @"Error Notification Outbox [1 entry locked]";
                }
                else
                {
                    root.Text = string.Format(@"Error Notification Outbox [{0} entries locked]", lockedEntryCount);
                }                
            }
            root.Expand();

            root.ImageIndex = 0;
            root.SelectedImageIndex = 0;
        }

        private void addErrorNode(FileLogger.LogErrorLevel inErrorLevel, string inTimeStr, FileLogger.LogErrorLevelFile inErrorLevelFile, string inErrorStr, string inErrorStrDetail, bool inSent)
        {
            TreeNode root = treeViewErrorMailBox.Nodes[0];
            TreeNode treeEntry = root.Nodes[(int)inErrorLevel-1];

            TreeNode treeError;

            if (inErrorLevelFile == FileLogger.LogErrorLevelFile.None)
            {
                treeError = treeEntry.Nodes.Insert(0, string.Format(@"{0}    {1}    {2}", inTimeStr, inErrorStr, inErrorStrDetail));
                //treeError = treeEntry.Nodes.Add(string.Format(@"Time: {0}  Error: {1}  Detail: {2}", inTimeStr, inErrorStr, inErrorStrDetail));
            }
            else
            {
                treeError = treeEntry.Nodes.Insert(0, string.Format(@"{0}    {1}    {2}    {3}", inTimeStr, _singletonLogger.GetLogErrorLevelFileStr(inErrorLevelFile), inErrorStr, inErrorStrDetail));
                //treeError = treeEntry.Nodes.Add(string.Format(@"Time: {0}  File: {1}  Error: {2}  Detail: {3}", inTimeStr, _singletonLogger.GetLogErrorLevelFileStr(inErrorLevelFile), inErrorStr, inErrorStrDetail));
            }

            treeError.ImageIndex = 2;
            treeError.SelectedImageIndex = 2;
            treeError.ToolTipText = @"";

            //..............................

            treeEntry.Text = string.Format(@"Entry - {0} [locked]", _singletonLogger.GetLogErrorLevelStr(inErrorLevel));

            //...............................

            treeEntry.ImageIndex = 3;
            treeEntry.SelectedImageIndex = 3;
            treeEntry.Expand();

            //................................

            int lockedEntryCount = 0;
            foreach(TreeNode entry in root.Nodes)
            {
                if (entry.Nodes.Count > 0)
                {
                    lockedEntryCount += 1;
                }
            }

            if (lockedEntryCount > 0)
            {
                if (lockedEntryCount == 1)
                {
                    root.Text = @"Error Notification Outbox [1 entry locked]";
                }
                else
                {
                    root.Text = string.Format(@"Error Notification Outbox [{0} entries locked]", lockedEntryCount);
                }
            }

            //................................

            Use_Notify(@"Summa Pro has failed. Have a look at the error logs");
        }

        //.................................

        private void setTreeNodeFont(DrawTreeNodeEventArgs inEvArgs)
        {
            //drawMailBoxNode(inEvArgs, inEvArgs.Node.NodeFont, inEvArgs.Node.ForeColor);

            switch(inEvArgs.Node.Level)
            {
                case 0: setRootNodeFont(inEvArgs); break; // Root
                case 1: setEntryNodeFont(inEvArgs); break; // Entry
                case 2: setErrorNodeFont(inEvArgs); break; // Error
            }
        }

        //.................................

        private int mailBoxErrorCount()
        {
            int result = 0;

            //:::::::::::::::::::::::::

            if (treeViewErrorMailBox.Nodes.Count > 0)
            {
                foreach (TreeNode node in treeViewErrorMailBox.Nodes[0].Nodes)
                {
                    result += node.Nodes.Count;
                }
            }

            //:::::::::::::::::::::::::

            return result;
        }

        //.................................

        private void setRootNodeFont(DrawTreeNodeEventArgs inEvArgs)
        {
            if (mailBoxErrorCount() == 0)
            {
                drawMailBoxNode(inEvArgs, inEvArgs.Node.NodeFont, inEvArgs.Node.ForeColor);
            }
            else
            {
                drawMailBoxNode(inEvArgs, inEvArgs.Node.NodeFont, Color.Gray);
                //drawMailBoxNode(inEvArgs, new Font("Microsoft Sans Serif", 12f, FontStyle.Bold), inEvArgs.Node.ForeColor);
            }       
        }

        //.................................

        private void setEntryNodeFont(DrawTreeNodeEventArgs inEvArgs)
        {
            if (inEvArgs.Node.Nodes.Count == 0)
            {
                drawMailBoxNode(inEvArgs, inEvArgs.Node.NodeFont, inEvArgs.Node.ForeColor);
            }
            else
            {
                drawMailBoxNode(inEvArgs, inEvArgs.Node.NodeFont, Color.Gray);
                //drawMailBoxNode(inEvArgs, new Font("Microsoft Sans Serif", 12f, FontStyle.Bold), inEvArgs.Node.ForeColor);
            }       
        }

        //.................................

        private void setErrorNodeFont(DrawTreeNodeEventArgs inEvArgs)
        {
            if (inEvArgs.Node.Index == ( inEvArgs.Node.Parent.Nodes.Count - 1 ) )
            {
                drawMailBoxNode(inEvArgs, inEvArgs.Node.NodeFont, inEvArgs.Node.ForeColor);
            }
            else
            {
                drawMailBoxNode(inEvArgs, inEvArgs.Node.NodeFont, Color.Gray);
            }       
        }

        //.................................

        // Draws a node.
        private void drawMailBoxNode(DrawTreeNodeEventArgs e, Font inNormalFont, Color inNormalForeColor)
        {
            // Retrieve the node font. If the node font has not been set,
            // use the TreeView font.
            //Font nodeFont = e.Node.NodeFont;
            Font nodeFont = inNormalFont;
            if (nodeFont == null) nodeFont = treeViewErrorMailBox.Font;

            e.Node.NodeFont = nodeFont;
            e.Node.ForeColor = inNormalForeColor;

            //e.Node.Text = TextTruncator.TruncateText(e.Node.Text, Convert.ToInt32(bounds.Width), e.Node.NodeFont.OriginalFontName, Convert.ToInt32(e.Node.NodeFont.SizeInPoints));

            // Draw the background and node text for a selected node.
            if ((e.State & TreeNodeStates.Selected) != 0)
            {
                // Draw the background of the selected node. The NodeBounds
                // method makes the highlight rectangle large enough to
                // include the text of a node tag, if one is present.
                //e.Graphics.FillRectangle(Brushes.SteelBlue, e.Node.Bounds);

                Rectangle bounds = NodeBounds(e.Node);

                e.Graphics.FillRectangle(Brushes.SteelBlue, bounds);

                // Draw the node text.
                e.Graphics.DrawString(e.Node.Text, nodeFont, Brushes.White, Rectangle.Inflate(bounds, 6, 0));
            }

            // Use the default background and node text.
            else
            {
                e.DrawDefault = true;
            }

            // If a node tag is present, draw its string representation 
            // to the right of the label text.
            if (e.Node.Tag != null)
            {
                e.Graphics.DrawString(e.Node.Tag.ToString(), tagFont, Brushes.Yellow, e.Bounds.Right + 2, e.Bounds.Top);
            }

            // If the node has focus, draw the focus rectangle large, making
            // it large enough to include the text of the node tag, if present.
            if ((e.State & TreeNodeStates.Focused) != 0)
            {
                using (Pen focusPen = new Pen(Color.Black))
                {
                    focusPen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dot;

                    Rectangle focusBounds = NodeBounds(e.Node);
                    focusBounds.Size = new Size(focusBounds.Width - 1, focusBounds.Height - 1);

                    e.Graphics.DrawRectangle(focusPen, focusBounds);
                }
            }
        }

        //.................................

        // Selects a node that is clicked on its label or tag text.
        private void myTreeView_MouseDown(object sender, MouseEventArgs e)
        {
            TreeNode clickedNode = treeViewErrorMailBox.GetNodeAt(e.X, e.Y);
            if (NodeBounds(clickedNode).Contains(e.X, e.Y))
            {
                treeViewErrorMailBox.SelectedNode = clickedNode;
            }
        }

        //.................................

        // Returns the bounds of the specified node, including the region 
        // occupied by the node label and any node tag displayed.
        private Rectangle NodeBounds(TreeNode node)
        {
            // Set the return value to the normal node bounds.
            Rectangle bounds = node.Bounds;
            Graphics g = treeViewErrorMailBox.CreateGraphics();
            if (node.Tag != null)
            {
                // Retrieve a Graphics object from the TreeView handle
                // and use it to calculate the display width of the tag.
                //Graphics g = treeViewErrorMailBox.CreateGraphics();
                int tagWidth = (int)g.MeasureString(node.Tag.ToString(), tagFont).Width + 2;

                // Adjust the node bounds using the calculated value.
                bounds.Offset(tagWidth / 2, 0);
                bounds = Rectangle.Inflate(bounds, tagWidth / 2, 0);
                g.Dispose();
            }
            else
            {
                //string text = TextTruncator.TruncateText(node.Text, 350, node.NodeFont.OriginalFontName, Convert.ToInt32(node.NodeFont.SizeInPoints));

                //string text = TextTruncator.TruncateText(node.Text, Convert.ToInt32(bounds.Width), node.NodeFont.OriginalFontName, Convert.ToInt32(node.NodeFont.SizeInPoints));
                //string text = TextTruncator.TruncateText(node.Text, Convert.ToInt32(bounds.Width), treeViewErrorMailBox.Font.OriginalFontName, Convert.ToInt32(treeViewErrorMailBox.Font.SizeInPoints));

                //int textWidth = (int)g.MeasureString(text, node.NodeFont).Width + 2;
                //int textHeight = node.Bounds.Height; // (int)g.MeasureString(text, node.NodeFont).Height;

                // Adjust the node bounds using the calculated value.
                //bounds.Offset(textWidth / 2, 0);
                //bounds = Rectangle.Inflate(bounds, textWidth / 2, 0);

                //bounds = new Rectangle(bounds.X, bounds.Y, textWidth, textHeight);
                //g.Dispose();
            }

            return bounds;
        }

        //.................................

        private void notifNewConfigFileCreated()
        {
            _initConfigResult = InitConfigResult.NewConfigFileCreated;
            this.Show();         
        }

        //.................................

        private void notifConfigFileLoaded(ConfigurationFileManager.ConfigFileError inError, string inErrorStr)
        {
            _configFileError    = inError;
            _configFileErrorStr = inErrorStr; 

            this.Hide();
            _initConfigResult = InitConfigResult.ConfigFileLoaded;
        }

        //.................................

        private void notifConfigFileLoadFailed(ConfigurationFileManager.ConfigFileError outError, string outErrorStr)
        {
            _singletonLogger.LogError(FileLogger.LogErrorLevel.AppFailedToStart, FileLogger.LogErrorLevelFile.None,
                                      @"Summa failed to create a new configuration file.", outErrorStr);

            this.Show();
            _initConfigResult = InitConfigResult.ConfigFileLoadFailed;
        }

        //.................................
                    
        private void initConfigGUI()
        {
            textBoxTCPPort.Text = _singletonConfigFile.TcpPort;

            updateSiteIDMapInfoListView(_singletonConfigFile.SiteIDMapInfo, listViewSiteIDMap);

            textBoxSchedule.Text      = _singletonConfigFile.Schedule;
            textBoxPattern.Text       = _singletonConfigFile.Pattern;
            textBoxUserName.Text      = _singletonConfigFile.UserName;
            textBoxPassword.Text      = _singletonConfigFile.Password;
            textBoxSourceFolder.Text  = _singletonConfigFile.SourceFolder;
            textBoxArchiveFolder.Text = _singletonConfigFile.ArchiveFolder;

            textBoxLocationName.Text = _singletonConfigFile.LocationName;
            checkBoxIsDoorAccess.Checked = _singletonConfigFile.IsDoorAccess == @"true";
            textBoxIsDoorAccessSchedule.Text = _singletonConfigFile.IsDoorAccessSchedule;
            textBoxIsDoorAccessSourceFolder.Text = _singletonConfigFile.IsDoorAccessSourceFolder;
            textBoxIsDoorAccessTargetFolder.Text = _singletonConfigFile.IsDoorAccessTargetFolder;
            textBoxIsDoorAccessArchiveFolder.Text = _singletonConfigFile.IsDoorAccessArchiveTargetFolder;
            textBoxIsDoorAccessPattern.Text = _singletonConfigFile.IsDoorAccessPattern;

            setLocationSectionEnable();

            checkBoxNotifications.Checked = _singletonConfigFile.EnableNotifications == @"true";
        }

        //.................................

        private void updateSiteIDMapInfoListView(SiteIDMap[] inSiteIDInfo, ListView inlistViewSiteIDMap)
        {
            inlistViewSiteIDMap.Items.Clear();

            foreach (SiteIDMap siteIDMap in inSiteIDInfo)
            {
                foreach (var pair in siteIDMap)
                {
                    ListViewItem item = new ListViewItem(pair.Key);
                    item.SubItems.Add(pair.Value);

                    inlistViewSiteIDMap.Items.Add(item);
                }
            }
        }

        //.................................

        private void addSiteIDMapInfo(string inSiteID, string inSummaID, ListView inlistViewSiteIDMap)
        {
            ListViewItem item = new ListViewItem(inSiteID);
            item.SubItems.Add(inSummaID);

            inlistViewSiteIDMap.Items.Add(item);
        }

        //.................................

        private void editSiteIDMapInfo(string inSiteID, string inSummaID, ListView inlistViewSiteIDMap)
        {
            listViewSiteIDMap.SelectedItems[0].SubItems[0].Text = inSiteID;
            listViewSiteIDMap.SelectedItems[0].SubItems[1].Text = inSummaID;
        }

        //.................................

        private void saveAllConfig()
        {
             _singletonConfigFile.saveAll(textBoxTCPPort.Text,
                                         listViewItemToSiteIDMapInfo(listViewSiteIDMap.Items),
                                         textBoxSchedule.Text,
                                         textBoxPattern.Text,
                                         textBoxUserName.Text,
                                         textBoxPassword.Text,
                                         textBoxSourceFolder.Text,
                                         textBoxArchiveFolder.Text);

             resetSummaserver();
        }

        //.......................................

        private void saveLocationAll()
        {
            string oldLocationName = _singletonConfigFile.LocationName;
            string isDoorAccess = checkBoxIsDoorAccess.Checked ? @"true" : @"false";

            _singletonConfigFile.saveLocation(textBoxLocationName.Text,
                                              isDoorAccess,
                                              textBoxIsDoorAccessSourceFolder.Text,
                                              textBoxIsDoorAccessTargetFolder.Text,
                                              textBoxIsDoorAccessArchiveFolder.Text,
                                              textBoxIsDoorAccessSchedule.Text,
                                              textBoxIsDoorAccessPattern.Text);


            if (oldLocationName != textBoxLocationName.Text)
            {
                this.Text = string.Format(@"Summa Pro: {0}", _singletonConfigFile.LocationName);

                _singletonLogger.setMailLoggerSubject(string.Format(@"Summa notification in location: {0}", this.Text));

                _singletonLogger.Log(string.Format(@"This location has been asigned a new name: {0}", textBoxLocationName.Text));
            }

            resetSummaserver();
        }

       //.......................................

        private void setDefaultLocationDoorAccess()
        {
            checkBoxIsDoorAccess.Checked = _singletonConfigFile.DefaulIsDoorAccess == @"true" ? true : false;
            textBoxIsDoorAccessSchedule.Text = _singletonConfigFile.DefaultIsDoorAccessSchedule;
            textBoxIsDoorAccessSourceFolder.Text = _singletonConfigFile.DefaulIsDoorAccessSourceFolder;
            textBoxIsDoorAccessTargetFolder.Text = _singletonConfigFile.DefaultIsDoorAccessTargetFolder;
            textBoxIsDoorAccessPattern.Text = _singletonConfigFile.DefaultIsDoorAccessPattern;

            setControlAsNotSaved(checkBoxIsDoorAccess);
            setControlAsNotSaved(textBoxIsDoorAccessSchedule);
            setControlAsNotSaved(textBoxIsDoorAccessSourceFolder);
            setControlAsNotSaved(textBoxIsDoorAccessTargetFolder);
            setControlAsNotSaved(textBoxIsDoorAccessPattern);
        }

        //.................................

        private void setDefaultLocationDoorAccessArchive()
        {
            textBoxIsDoorAccessArchiveFolder.Text = _singletonConfigFile.DefaultIsDoorAccessArchiveTargetFolder;

            setControlAsNotSaved(textBoxIsDoorAccessArchiveFolder);
        }

        //.................................
        
        private void setDefaultLocationAll()
        {
            textBoxLocationName.Text = _singletonConfigFile.DefaultLocationName;
            setControlAsNotSaved(textBoxLocationName);

            setDefaultLocationDoorAccess();
            setDefaultLocationDoorAccessArchive();
        }
        
        //.................................

        private void setLocationSectionEnable()
        {
            textBoxIsDoorAccessSchedule.Enabled = checkBoxIsDoorAccess.Checked;
            textBoxIsDoorAccessSourceFolder.Enabled = checkBoxIsDoorAccess.Checked;
            textBoxIsDoorAccessTargetFolder.Enabled = checkBoxIsDoorAccess.Checked;
            textBoxIsDoorAccessArchiveFolder.Enabled = checkBoxIsDoorAccess.Checked;
            textBoxIsDoorAccessPattern.Enabled = checkBoxIsDoorAccess.Checked;

            buttonLocationSetDefault.Enabled = checkBoxIsDoorAccess.Checked;
            buttonLocationSetDefaultArchive.Enabled = checkBoxIsDoorAccess.Checked;

            buttonIsDoorAccessBrowseSourceFolder.Enabled = checkBoxIsDoorAccess.Checked;
            buttonIsDoorAccessBrowseTargetFolder.Enabled = checkBoxIsDoorAccess.Checked;
            buttonIsDoorAccessBrowseArchiveFolder.Enabled = checkBoxIsDoorAccess.Checked;

            //panel8.Enabled = checkBoxIsDoorAccess.Checked;
            //panel11.Enabled = checkBoxIsDoorAccess.Checked;

            if (checkBoxIsDoorAccess.Checked)
            {
                textBoxIsDoorAccessSchedule.BackColor = Color.White;
                textBoxIsDoorAccessSourceFolder.BackColor = Color.White;
                textBoxIsDoorAccessTargetFolder.BackColor = Color.White;
                textBoxIsDoorAccessArchiveFolder.BackColor = Color.White;
                textBoxIsDoorAccessPattern.BackColor = Color.White;
            }
            else
            {
                textBoxIsDoorAccessSchedule.BackColor = Color.LightGray;
                textBoxIsDoorAccessSourceFolder.BackColor = Color.LightGray;
                textBoxIsDoorAccessTargetFolder.BackColor = Color.LightGray;
                textBoxIsDoorAccessArchiveFolder.BackColor = Color.LightGray;
                textBoxIsDoorAccessPattern.BackColor = Color.LightGray;
            }
        }

        //..................................

        private void setDefaultEnableNotificationAll()
        {
            checkBoxNotifications.Checked = _singletonConfigFile.DefaultEnableNotifications == @"true" ? true : false;
        }

        //..................................

        private void saveEnableNotificationsAll()
        {
            string enableNotifications = checkBoxNotifications.Checked ? @"true" : @"false";

            _singletonConfigFile.saveNotifications(enableNotifications);

            _singletonLogger.NotificationsEnabled = _singletonConfigFile.EnableNotifications == @"true";

            resetSummaserver();
        }

        //.......................................

        private void setDefaultEnableNotificationsAll()
        {
            checkBoxNotifications.Checked = _singletonConfigFile.DefaultEnableNotifications == @"true" ? true : false;

            setControlAsNotSaved(checkBoxNotifications);
        }
        
        //..................................

        private SiteIDMap[] listViewItemToSiteIDMapInfo(ListView.ListViewItemCollection inItems)
        {
            List<SiteIDMap> result = new List<SiteIDMap>();

            //::::::::::::::::::::::::::::::::::

            foreach(ListViewItem item in inItems)
            {
                SiteIDMap siteIDMap = new SiteIDMap();
                siteIDMap.Add(item.SubItems[0].Text, item.SubItems[1].Text);

                result.Add(siteIDMap);
            }

            //::::::::::::::::::::::::::::::::::

            return result.ToArray();
        }

        private void unlockAll()
        {
            _singletonLogger.ResetErrorMailBox();
            openTreeViewErrorMailBox();
        }

        //.................................

        private void unlockAppFailedToStart()
        {
            _singletonLogger.ResetErrorMailBoxEntry(FileLogger.LogErrorLevel.AppFailedToStart);
            openTreeViewErrorMailBox();
        }

        //.................................

        private void unlockSiteIDMapFailed()
        {
           _singletonLogger.ResetErrorMailBoxEntry(FileLogger.LogErrorLevel.SiteIDMapFailed);
           openTreeViewErrorMailBox();
        }

        //.................................

        private void unlockInvalidSiteID()
        {
           _singletonLogger.ResetErrorMailBoxEntry(FileLogger.LogErrorLevel.InvalidSiteID);
           openTreeViewErrorMailBox();
        }

        //.................................

        private void unlockInternalXMLError()
        {
            _singletonLogger.ResetErrorMailBoxEntry(FileLogger.LogErrorLevel.InternalXMLError);
            openTreeViewErrorMailBox();
        }

        //.................................

        private void unlockUploadFailed()
        {
            _singletonLogger.ResetErrorMailBoxEntry(FileLogger.LogErrorLevel.UploadFailed);
            openTreeViewErrorMailBox();
        }

        //.................................

        private void unlockCreateZIPFailed()
        {
            _singletonLogger.ResetErrorMailBoxEntry(FileLogger.LogErrorLevel.CreateZIPFailed);
            openTreeViewErrorMailBox();
        }

        //.................................

        private void unlockServerStartListeningFailed()
        {
            _singletonLogger.ResetErrorMailBoxEntry(FileLogger.LogErrorLevel.ServerStartListeningFailed);
            openTreeViewErrorMailBox();
        }

        //.................................

        private void setControlAsNotSaved(Control inControl)
        {
             inControl.ForeColor = Color.Red;
        }

        //.................................

        private void setControlAsSaved(Control inControl)
        {
            inControl.ForeColor = Color.Black;
        }

        //.................................

        private void setAllControlsAsSaved()
        {
            setConfigControlsAsSaved();
            setLocationControlsAsSaved();
            setNotificationControlsAsSaved();
        }

        //.................................

        private void setConfigControlsAsSaved()
        {
            setControlAsSaved(textBoxTCPPort);
            setControlAsSaved(listViewSiteIDMap);
            setControlAsSaved(textBoxSchedule);
            setControlAsSaved(textBoxUserName);
            setControlAsSaved(textBoxPassword);
            setControlAsSaved(textBoxSourceFolder);
            setControlAsSaved(textBoxPattern);
            setControlAsSaved(textBoxArchiveFolder);
            linkLabel9.LinkColor = Color.Black;
            //setControlAsSaved(linkLabel9);
        }

        //.................................

        private void setLocationControlsAsSaved()
        {
            setControlAsSaved(textBoxLocationName);
            setControlAsSaved(checkBoxIsDoorAccess);
            setControlAsSaved(textBoxIsDoorAccessSchedule);
            setControlAsSaved(textBoxIsDoorAccessSourceFolder);
            setControlAsSaved(textBoxIsDoorAccessTargetFolder);
            setControlAsSaved(textBoxIsDoorAccessPattern);
            setControlAsSaved(textBoxIsDoorAccessArchiveFolder);
        }

        //.................................

        private void setNotificationControlsAsSaved()
        {
            setControlAsSaved(checkBoxNotifications);
        }

        //:::::::::::::::::::::::::::::::::::::::

        private void panelNotifSent_Click(object sender, EventArgs e)
        {
        }

        //.................................

        private void panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void buttonDefaultGeneral_MouseClick(object sender, MouseEventArgs e)
        {
            textBoxTCPPort.Text = _singletonConfigFile.DefaultTcpPort;
            updateSiteIDMapInfoListView(_singletonConfigFile.DefaultSiteIDMapInfo, listViewSiteIDMap);

            setControlAsNotSaved(textBoxTCPPort);
            setControlAsNotSaved(listViewSiteIDMap);
        }

        private void buttonFTPServer_MouseClick(object sender, MouseEventArgs e)
        {
            textBoxSchedule.Text     = _singletonConfigFile.DefaultSchedule;
            textBoxPattern.Text      = _singletonConfigFile.DefaultPattern;
            textBoxUserName.Text     = _singletonConfigFile.DefaultUserName;
            textBoxPassword.Text     = _singletonConfigFile.DefaultPassword;
            textBoxSourceFolder.Text = _singletonConfigFile.DefaultSourceFolder;

            setControlAsNotSaved(textBoxSchedule);
            setControlAsNotSaved(textBoxPattern);
            setControlAsNotSaved(textBoxUserName);
            setControlAsNotSaved(textBoxPassword);
            setControlAsNotSaved(textBoxSourceFolder);
        }

        private void buttonDefaultArchive_MouseClick(object sender, MouseEventArgs e)
        {
            textBoxArchiveFolder.Text = _singletonConfigFile.DefaultArchiveFolder;

            setControlAsNotSaved(textBoxArchiveFolder);
        }

        private void buttonDefaultAll_MouseClick(object sender, MouseEventArgs e)
        {
            buttonDefaultGeneral_MouseClick(null, e);
            buttonFTPServer_MouseClick(null, e);
            buttonDefaultArchive_MouseClick(null, e);
        }

        private void buttonBrowseSourceFolder_MouseClick(object sender, MouseEventArgs e)
        {
            folderBrowserDialogSourceFolder.SelectedPath = textBoxSourceFolder.Text;
            folderBrowserDialogSourceFolder.Description = @"Summa BI's FTP Source Folder";

            if (folderBrowserDialogSourceFolder.ShowDialog() == DialogResult.OK)
            {
                textBoxSourceFolder.Text = folderBrowserDialogSourceFolder.SelectedPath;
            }
        }

        private void buttonBrowseArchive_MouseClick(object sender, MouseEventArgs e)
        {
            folderBrowserDialogArchive.SelectedPath = textBoxArchiveFolder.Text;
            folderBrowserDialogArchive.Description = @"Archive Folder";

            if (folderBrowserDialogArchive.ShowDialog() == DialogResult.OK)
            {
                textBoxArchiveFolder.Text = folderBrowserDialogArchive.SelectedPath;
            }
        }

        private void buttonAddSummaMap_Click(object sender, EventArgs e)
        {
            string siteID      = @"99";
            string summaSiteID = @"3100";

            if (Helper.Instance.SiteIDMapInputBox(ref siteID, ref summaSiteID) == DialogResult.OK)
            {
                addSiteIDMapInfo(siteID, summaSiteID, listViewSiteIDMap);
                setControlAsNotSaved(listViewSiteIDMap);
            }
        }

        private void buttonEditSummaMap_MouseClick(object sender, MouseEventArgs e)
        {
            try
            {
                string siteID = listViewSiteIDMap.SelectedItems[0].Text;
                string summaSiteID = listViewSiteIDMap.SelectedItems[0].SubItems[1].Text;

                if (Helper.Instance.SiteIDMapInputBox(ref siteID, ref summaSiteID) == DialogResult.OK)
                {
                    editSiteIDMapInfo(siteID, summaSiteID, listViewSiteIDMap);
                    setControlAsNotSaved(listViewSiteIDMap);
                    setControlAsNotSaved(linkLabel9);
                }
            }
            catch(Exception)
            {
                MessageBox.Show(@"No Map Pair selected.");
            }
        }

        private void buttonDelSummaMap_MouseClick(object sender, MouseEventArgs e)
        {
            try
            {
                listViewSiteIDMap.Items.Remove(listViewSiteIDMap.SelectedItems[0]);
                setControlAsNotSaved(listViewSiteIDMap);
                linkLabel9.LinkColor = Color.Red;
                //setControlAsNotSaved(linkLabel9);
            }
            catch(Exception)
            {
                MessageBox.Show(@"No Map Pair selected.");
            }
        }

        private void buttonSaveAll_Click(object sender, EventArgs e)
        {
            saveAllConfig();
            setConfigControlsAsSaved();
        }

        private void panelUploadNow_Click(object sender, EventArgs e)
        {
            server.Upload();
        }

        private void panel9_Click(object sender, EventArgs e)
        {

            if (server.stopped)
            {
                server.Start();

                this.panelPause.BackgroundImage = Image.FromFile(@".\Resources\pause snow80 tangerine80.png");
                linkLabelPause.Text = @"Pause";
                labelPause.Text     = @"Stop receiving request from Menumate";
            }
            else
            {
                server.Stop();

                this.panelPause.BackgroundImage = Image.FromFile(@".\Resources\go snow80 tangerine80.png");
                linkLabelPause.Text = @"Go";
                labelPause.Text = @"Start receiving request from Menumate";
            }
        }

        //.................................

        private void panel10_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("All the logs will be removed from the screen", "Clear now?",
                        MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation) == DialogResult.OK)
            {
                richTextBoxTrace.Clear();
            }
        }

        //.................................

        private void panelOpenUploadFolder_Click(object sender, EventArgs e)
        {
            Process p = new Process();
            p.StartInfo.FileName = "explorer";
            p.StartInfo.Arguments = _singletonConfigFile.SourceFolder;
            p.Start();
        }

        //.................................

        private void panel9_Click_1(object sender, EventArgs e)
        {
            this.Close();
        }

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        //.................................

        private void panel18_Click(object sender, EventArgs e)
        {
            InfoForm infoForm = new InfoForm();

            infoForm.SelectedTabIndex = 0;

            infoForm.ShowDialog();
        }

        //.................................

        private void panel22_Click(object sender, EventArgs e)
        {
            InfoForm infoForm = new InfoForm();

            infoForm.SelectedTabIndex = 1;

            infoForm.ShowDialog();
        }

        //.................................

        private void panel24_Click(object sender, EventArgs e)
        {
            InfoForm infoForm = new InfoForm();

            infoForm.SelectedTabIndex = 2;

            infoForm.ShowDialog();
        }

        //.................................

        private void panel8_Click(object sender, EventArgs e)
        {
            InfoForm infoForm = new InfoForm();

            infoForm.SelectedTabIndex = 3;

            infoForm.ShowDialog();
        }

        //.................................

        private void showMainWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (showMainWindowToolStripMenuItem.Checked)
            {
                this.Show();
                this.WindowState = FormWindowState.Normal;
            }
            else
            {

                this.WindowState = FormWindowState.Minimized;
                this.Hide();
            }
        }

        //.................................

        private void uploadNOWToolStripMenuItem_Click(object sender, EventArgs e)
        {
            panelUploadNow_Click(sender, e);
        }

        //.................................
    
        private void openUploadFolderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            panelOpenUploadFolder_Click(sender, e);
        }

        private void openLogsFolderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Process p = new Process();
            p.StartInfo.FileName = "explorer";
            p.StartInfo.Arguments = @".\logs\";
            p.Start();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void MainForm_ClientSizeChanged(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Minimized)
            {
                showMainWindowToolStripMenuItem.Checked = false;
                this.Hide();
            }
        }

        private void buttonSaveLocationName_Click(object sender, EventArgs e)
        {
        }

        private void MainForm_Activated(object sender, EventArgs e)
        {
            showMainWindowToolStripMenuItem.Checked = true;
        }

        private void buttonLocationSaveAll_Click(object sender, EventArgs e)
        {
            saveLocationAll();
            setLocationControlsAsSaved();
        }

        private void buttonLocationSetDefault_MouseClick(object sender, MouseEventArgs e)
        {
            setDefaultLocationDoorAccess();
        }

        private void buttonLocationSetDefaultArchive_MouseClick(object sender, MouseEventArgs e)
        {
            setDefaultLocationDoorAccessArchive();
        }

        private void buttonLocationSetAllToDefault_Click(object sender, EventArgs e)
        {
            setDefaultLocationAll();
        }

        private void checkBoxIsDoorAccess_CheckedChanged(object sender, EventArgs e)
        {
            setLocationSectionEnable();
            setControlAsNotSaved((Control)sender);
        }

        private void checkBoxNotifications_CheckedChanged(object sender, EventArgs e)
        {
            setControlAsNotSaved((Control)sender);
        }

        private void buttonSetAllDefaultNotifications_Click(object sender, EventArgs e)
        {
            setDefaultEnableNotificationsAll();
        }

        private void buttonSaveAllNotifications_Click(object sender, EventArgs e)
        {
            saveEnableNotificationsAll();
            setNotificationControlsAsSaved();
        }

        private void panelPause_Paint(object sender, PaintEventArgs e)
        {

        }

        private void treeViewErrorMailBox_DrawNode(object sender, DrawTreeNodeEventArgs e)
        {
            setTreeNodeFont(e);
        }

        private void toolStripMenuItemAll_Click(object sender, EventArgs e)
        {
            unlockAll();
        }

        private void appFailedToStartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            unlockAppFailedToStart();
        }

        private void siteIDMapFailedToolStripMenuItem_Click(object sender, EventArgs e)
        {
            unlockSiteIDMapFailed();
        }

        private void invalidSiteIDToolStripMenuItem_Click(object sender, EventArgs e)
        {
            unlockInvalidSiteID();
        }

        private void internalXMLErrorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            unlockInternalXMLError();
        }

        private void createZIPFailedToolStripMenuItem_Click(object sender, EventArgs e)
        {
            unlockCreateZIPFailed();
        }

        private void uploadFailedToolStripMenuItem_Click(object sender, EventArgs e)
        {
            unlockUploadFailed();
        }

        private void serverStartListeningFailedToolStripMenuItem_Click(object sender, EventArgs e)
        {
            unlockServerStartListeningFailed();
        }

        private void contextMenuStripTreeview_Opening(object sender, CancelEventArgs e)
        {
            ContextMenuStrip cmStrip = (ContextMenuStrip)sender;
            TreeNode selNode = treeViewErrorMailBox.SelectedNode;

            if(mailBoxErrorCount() == 0)
            {
                cmStrip.Items[0].ForeColor = Color.Gray;
                cmStrip.Items[0].Enabled = false;

                cmStrip.Items[1].ForeColor = Color.Gray;
                cmStrip.Items[1].Enabled = false;
            }
            else
            if ((selNode == null) || (selNode.Parent == null) || (selNode.Nodes.Count == 0))
            {
                cmStrip.Items[0].ForeColor = Color.Gray;
                cmStrip.Items[0].Enabled = false;
               
                cmStrip.Items[1].ForeColor = Color.Black; 
                cmStrip.Items[1].Enabled = true;
            }
            else 
            {
                cmStrip.Items[0].ForeColor = Color.Black;
                cmStrip.Items[0].Enabled = true;

                cmStrip.Items[1].ForeColor = Color.Black;
                cmStrip.Items[1].Enabled = true;
            }
        }

        private void unlockEntryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            switch (treeViewErrorMailBox.SelectedNode.Index)
            {
                case 0: unlockAppFailedToStart(); break;
                case 1: unlockSiteIDMapFailed(); break;
                case 2: unlockInvalidSiteID(); break;
                case 3: unlockInternalXMLError(); break;
                case 4: unlockCreateZIPFailed(); break;
                case 5: unlockUploadFailed(); break;
                case 6: unlockServerStartListeningFailed(); break;
            }
        }

        private void unlockAllEntriesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            unlockAll();
        }

        private void toolStripDropDownButton1_DropDownOpening(object sender, EventArgs e)
        {
            ToolStripDropDownButton ddBtn = (ToolStripDropDownButton)sender;
            TreeNode selNode = treeViewErrorMailBox.SelectedNode;

            if (mailBoxErrorCount() == 0)
            {
                foreach (ToolStripMenuItem item in ddBtn.DropDownItems)
                {
                    item.ForeColor = Color.Gray;
                    item.Enabled = false;
                }
            }
            else
            {
                ddBtn.DropDownItems[0].ForeColor = Color.Black;
                ddBtn.DropDownItems[0].Enabled = true;

                foreach(TreeNode node in treeViewErrorMailBox.Nodes[0].Nodes)
                {
                    if(node.Nodes.Count == 0)
                    {
                        ddBtn.DropDownItems[node.Index + 1].ForeColor = Color.Gray;
                        ddBtn.DropDownItems[node.Index + 1].Enabled = false;
                    }
                    else
                    {
                        ddBtn.DropDownItems[node.Index + 1].ForeColor = Color.Black;
                        ddBtn.DropDownItems[node.Index + 1].Enabled = true;
                    }
                }
            }
        }

        private void textBoxTCPPort_TextChanged(object sender, EventArgs e)
        {
            setControlAsNotSaved((Control)sender);
        }

        private void buttonDefaultGeneral_Click(object sender, EventArgs e)
        {

        }

        private void buttonFTPServer_Click(object sender, EventArgs e)
        {

        }

        private void panelUploadNow_Paint(object sender, PaintEventArgs e)
        {

        }

        private void buttonIsDoorAccessBrowseSourceFolder_MouseClick(object sender, MouseEventArgs e)
        {
            folderBrowserDialogSourceFolder.SelectedPath = textBoxIsDoorAccessSourceFolder.Text;
            folderBrowserDialogSourceFolder.Description = @"Summa BI's FTP Source Folder";

            if (folderBrowserDialogSourceFolder.ShowDialog() == DialogResult.OK)
            {
                textBoxIsDoorAccessSourceFolder.Text = folderBrowserDialogSourceFolder.SelectedPath;
            }
        }

        private void buttonIsDoorAccessBrowseTargetFolder_MouseClick(object sender, MouseEventArgs e)
        {
            folderBrowserDialogSourceFolder.SelectedPath = textBoxIsDoorAccessTargetFolder.Text;
            folderBrowserDialogSourceFolder.Description = @"Summa BI's FTP Source Folder";

            if (folderBrowserDialogSourceFolder.ShowDialog() == DialogResult.OK)
            {
                textBoxIsDoorAccessTargetFolder.Text = folderBrowserDialogSourceFolder.SelectedPath;
            }
        }

        private void buttonIsDoorAccessBrowseArchiveFolder_MouseClick(object sender, MouseEventArgs e)
        {
            folderBrowserDialogSourceFolder.SelectedPath = textBoxIsDoorAccessArchiveFolder.Text;
            folderBrowserDialogSourceFolder.Description = @"Summa BI's FTP Source Folder";

            if (folderBrowserDialogSourceFolder.ShowDialog() == DialogResult.OK)
            {
                textBoxIsDoorAccessArchiveFolder.Text = folderBrowserDialogSourceFolder.SelectedPath;
            }
        }

        private void treeViewErrorMailBox_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            RichTextBox rTB = richTextBoxErrorDetail;

            rTB.Clear();

            //if(e.Node.Parent != e.Node.TreeView.Nodes[0])
            if ((e.Node.Parent != null) && (e.Node.Parent.Parent != null))
            {
                Font fBold = new Font("Microsoft Sans Serif", 10, FontStyle.Bold);
                rTB.SelectionFont = fBold;
                rTB.SelectedText = "\n" + e.Node.Parent.Text + "\n\n";
                //rtResults.SelectionColor = Color.Red;

                Font fRegular = new Font("Microsoft Sans Serif", 10, FontStyle.Regular);
                rTB.SelectionFont = fRegular;
                rTB.SelectedText = e.Node.Text;
            }
        }

        //.................................
    }
}
