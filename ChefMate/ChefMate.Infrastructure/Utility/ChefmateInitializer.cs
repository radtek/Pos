using Chefmate.Infrastructure.Builder;

namespace Chefmate.Infrastructure.Utility
{
    public static class ChefmateInitializer
    {

        public static void Start()
        {
            Builder.Builder.Instance.Start();
            FileSystemInitializer.Instance.Start();
        }

        public static void Stop()
        {
            Builder.Builder.Instance.Stop();
            FileSystemInitializer.Instance.Stop();
        }
    }
}
