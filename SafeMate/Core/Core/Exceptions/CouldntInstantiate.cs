using System;

namespace Safemate.Core.Exceptions
{
    public class CouldntInstantiate : Base
    {
        ~CouldntInstantiate()
        {
        }

        public CouldntInstantiate()
        : base()
        {
        }

        public CouldntInstantiate(Object instance,
                                  Exception innerException)
        : base(String.Format(
                   "Couldn't instantiate object of class {0}.",
                   instance.ToString()), innerException)
        {
        }
    };
};

