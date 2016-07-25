using System;
using System.Collections.Generic;
using System.Text;

namespace IntaMate
{
    class PMSInterfaceFactory
    {
        public PMS PMSInterface;

        public enum e_PMSInterfaceType { None, Stub, Micros, Hirum, Phoenix };

        private e_PMSInterfaceType pmsInterfaceType;

        public PMSInterfaceFactory()
        {
            PMSInterface = new PMSInterfaceStub();
            pmsInterfaceType = e_PMSInterfaceType.Stub;
        }

        public string PMSInterfaceType
        {
            get { return pmsInterfaceType.ToString(); }
            set
            {                
                e_PMSInterfaceType NewValue = e_PMSInterfaceType.None;
                try
                {
                    NewValue = (e_PMSInterfaceType)Enum.Parse(typeof(e_PMSInterfaceType), value);
                }
                catch (System.ArgumentException ex)
                {

                }

                if (pmsInterfaceType != NewValue)
                {
                    pmsInterfaceType = NewValue;
                    if (pmsInterfaceType == e_PMSInterfaceType.None)
                    {
                        PMSInterface = new PMSInterfaceStub();
                    }
                    else if (pmsInterfaceType == e_PMSInterfaceType.Stub)
                    {
                        PMSInterface = new PMSInterfaceStub();                        
                    }
                    else if (pmsInterfaceType == e_PMSInterfaceType.Micros)
                    {
                        PMSInterface = new PMSInterfaceMicros();                        
                    }
                    else if (pmsInterfaceType == e_PMSInterfaceType.Hirum)
                    {
                        PMSInterface = new PMSInterfaceHirum();                        
                    }
                    else if (pmsInterfaceType == e_PMSInterfaceType.Phoenix)
                    {
                        PMSInterface = new PMSInterfaceStub();
                    }
                    else
                    {
                    }
                }
            }                   
        }
    }
}
