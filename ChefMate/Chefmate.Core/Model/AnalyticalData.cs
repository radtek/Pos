using System;

namespace Chefmate.Core.Model
{
    public class AnalyticalData : ModelBase
    {
        private int _currentOrdersCount;
        private int _totalOrdersCount;
        private int _currentItems;
        private string _averageMakeTime;
        private TimeSpan _totalMakeTime;
        public AnalyticalData()
        {
            AverageMakeTime = "00:00:00";
        }

        public int CurrentOrdersCount
        {
            get
            {
                return _currentOrdersCount;
            }
            set
            {
                if (value < 0)
                    _currentOrdersCount = 0;
                else

                    _currentOrdersCount = value;
                OnPropertyChanged("CurrentOrdersCount");
            }
        }
        public int TotalOrdersCount
        {
            get
            {
                return _totalOrdersCount;
            }
            set
            {
                if (value < 0)
                    _totalOrdersCount = 0;
                else

                    _totalOrdersCount = value;
                OnPropertyChanged("TotalOrdersCount");
            }
        }
        public int CurrentItems
        {
            get
            {
                return _currentItems;
            }
            set
            {
                if (value < 0)
                    _currentItems = 0;
                else
                    _currentItems = value;
                OnPropertyChanged("CurrentItems");
            }
        }
        public string AverageMakeTime
        {
            get
            {
                return _averageMakeTime;
            }
            set
            {
                _averageMakeTime = value;
                OnPropertyChanged("AverageMakeTime");
            }
        }
        public TimeSpan TotalMakeTime
        {
            get
            {
                return _totalMakeTime;
            }
            set
            {
                _totalMakeTime = value;
                CalculateAverageMakeTime();
            }
        }
        private void CalculateAverageMakeTime()
        {
            var completedOrders = TotalOrdersCount - CurrentOrdersCount;
            if (completedOrders <= 0)
                completedOrders = 1;
            var averageMakeTime = new TimeSpan(TotalMakeTime.Ticks / completedOrders);
            AverageMakeTime = averageMakeTime.ToString(@"%h\:mm\:ss");
        }
    }
}