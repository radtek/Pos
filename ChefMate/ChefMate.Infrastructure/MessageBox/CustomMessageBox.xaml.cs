using System.ComponentModel;
using System.Windows;


namespace Chefmate.Infrastructure.MessageBox
{
    /// <summary>
    /// Interaction logic for CustomMessageBox.xaml
    /// </summary>
    public partial class CustomMessageBox : Window, INotifyPropertyChanged
    {

        private string _windowCaption;
        private string _message;
        private bool _modalResult;
        private bool _isCancleVisible;

        public CustomMessageBox()
        {
            InitializeComponent();
            this.DataContext = this;
        }

        public string WindowCaption
        {
            get { return _windowCaption; }
            set
            {
                _windowCaption = value;
                OnPropertyChanged("WindowCaption");
            }
        }
        public string Message
        {
            get { return _message; }
            set
            {
                _message = value;
                OnPropertyChanged("Message");
            }
        }
        public bool ModalResult
        {
            get { return _modalResult; }
            set
            {
                _modalResult = value;
                OnPropertyChanged("ModalResult");
            }
        }
        public bool IsCancleVisible
        {
            get { return _isCancleVisible; }
            set
            {
                _isCancleVisible = value;
                CancelButton.Visibility = value ? Visibility.Visible : Visibility.Collapsed;
            }
        }
        private void Ok_OnClick(object sender, RoutedEventArgs e)
        {
            ModalResult = true;
            Close();
        }

        private void Cancel_OnClick(object sender, RoutedEventArgs e)
        {
            ModalResult = false;
            Close();
        }

        #region PropertyChanged Implementation
        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged(string propertyName = null)
        {
            if(PropertyChanged!=null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion


    }
}
