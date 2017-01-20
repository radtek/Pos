using System.ComponentModel;
using System.Windows;

namespace Chefmate.UI.Views
{
    /// <summary>
    /// Interaction logic for ProcessingBox.xaml
    /// </summary>
    public partial class ProcessingBox : Window, INotifyPropertyChanged
    {

        private string _processingMessage;

        public ProcessingBox()
        {
            InitializeComponent();
            this.DataContext = this;
            ProcessingMessage = "Processing......";
        }


        public string ProcessingMessage
        {
            get { return _processingMessage; }
            set
            {
                _processingMessage = value;
                OnPropertyChanged("ProcessingMessage");
            }
        }


        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
