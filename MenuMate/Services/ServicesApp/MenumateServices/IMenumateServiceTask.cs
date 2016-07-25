using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices
{
    public interface IMenumateServiceTask
    {
        bool startServiceTask();
        bool pauseServiceTask();
        bool resumeServiceTask();
    }
}
