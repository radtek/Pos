using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FloorPlanClient
{
    public enum FloorPlanClientError
    {
        NoError, NotInitialized, InitFailed
    };

    public class FloorPlanManager
    {
        bool _xiManagerInitialized = false;
        FloorPlanRunner _floorPlanRunner = FloorPlanRunner.Instance;

        /// <summary>
        /// 
        /// </summary>
        public FloorPlanManager()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public static FloorPlanManager Instance
        {
            get
            {
                return Nested.instance;
            }
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

            internal static readonly FloorPlanManager instance = new FloorPlanManager();
        }

        #region Properties

        /// <summary>
        /// 
        /// </summary>
        public string ErrorMessage
        {
            get
            {
                return @"";
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public FloorPlanRunner FPlanRunner
        {
            get
            {
                return _floorPlanRunner;
            }
        }

        #endregion

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public FloorPlanClientError Init()
        {
            _xiManagerInitialized = true;

            return _floorPlanRunner.Init() ? FloorPlanClientError.NoError : FloorPlanClientError.InitFailed;
        }

        #region Private

        #endregion
    }
}
