using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateUtils.Encrypt
{
    public class EncryptAttributes
    {
        /// <summary>
        /// 
        /// </summary>
        internal EncryptAttributes()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly EncryptAttributes instance = new EncryptAttributes();
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        public static EncryptAttributes Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public string Password
        {
            get { return @"InTheBeginningGodCreatedTheHeavensAndTheEarthMenuMateCameLater"; }
        }
        #endregion
    }
}
