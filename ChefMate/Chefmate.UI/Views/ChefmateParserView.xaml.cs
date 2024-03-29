﻿using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Input;
using Chefmate.Core.Commands;
using Chefmate.UI.Annotations;
using ChefMate.Database.Parser;
using Path = System.IO.Path;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for ChefmateParserView.xaml
    /// </summary>
    public partial class ChefmateParserView : Window, INotifyPropertyChanged
    {
        private ObservableCollection<string> _parserLogs;
        private bool _isParseVisible;
        public ChefmateParserView()
        {
            InitializeComponent();
            ParserLogs = new ObservableCollection<string>();
            ParserCommand = new DelegateCommand(ParserCommandHandler);
            CloseCommand = new DelegateCommand(CancelCommandHandler);
            IsParseVisible = true;
            this.DataContext = this;
        }
        public ChefmateParserView(string dbAddress, string dbPath) : this()
        {
            IsParseVisible = false;
            RunParser(dbAddress, dbPath);
        }
        public ICommand ParserCommand { get; set; }
        public ICommand CloseCommand { get; set; }

        public bool IsParseVisible
        {
            get { return _isParseVisible; }
            set
            {
                _isParseVisible = value;
                OnPropertyChanged("IsParseVisible");
            }
        }
        public ObservableCollection<string> ParserLogs
        {
            get { return _parserLogs; }
            set
            {
                _parserLogs = value;
                OnPropertyChanged("ParserLogs");
            }
        }
        private void ParserCommandHandler(object sender)
        {
            var forlderBrowsingDialog = new FolderBrowserDialog();
            forlderBrowsingDialog.ShowNewFolderButton = true;
            var result = forlderBrowsingDialog.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                RunParser("127.0.0.1", Path.Combine(forlderBrowsingDialog.SelectedPath, "CHEFMATE.FDB"));
            }
        }
        private void RunParser(string dbAddress, string dbPath)
        {
            var chefmateParser = new ChefmateParser(dbAddress, dbPath);
            chefmateParser.StatusChangeEvent += ChefmateParser_StatusChangeEvent;
            chefmateParser.CreateDatabaseAndrunParser();
        }
        private void ChefmateParser_StatusChangeEvent(string status)
        {
            this.Dispatcher.Invoke(new Action(() =>
            {
                ParserLogs.Add(status);
            }));
        }
        private void CancelCommandHandler(object sender)
        {
            Close();
        }

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }


    }
}
