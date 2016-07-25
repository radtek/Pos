using System.Text.RegularExpressions;
using System.Xml;

namespace Safemate.Configurator
{
    public class Schedule : XmlSerializable
    {
        public enum TimeUnit : int {
            EveryNDays,
            EveryNHours,
            EveryNMinutes,
            EveryNMonths,
            EveryNWeeks
        }

        internal override void
        Deserialize(
            XmlNode sourceNode)
        {
            scheduleForConfigurationFile = sourceNode.InnerText;
            ProcessSchedulestring();
        }

        void
        ProcessSchedulestring()
        {
            char periodTypeSpecifier =
                scheduleForConfigurationFile[
                    scheduleForConfigurationFile.Length - 1];
            string pattern;

            switch (periodTypeSpecifier) {
            case 'd':
                pattern = "^[1-7]d$";
                timeUnit = TimeUnit.EveryNDays;
                break;
            case 'h':
                pattern = "^([0-2][0-4]?|[0-9])h$";
                timeUnit = TimeUnit.EveryNHours;
                break;
            case 'm':
                pattern = "^([0-5]?[0-9](:[0-5][0-9])?)m$";
                timeUnit = TimeUnit.EveryNMinutes;
                break;
            case 'M':
                pattern = "^([1-9]|1[0-2])M$";
                timeUnit = TimeUnit.EveryNMonths;
                break;
            case 'w':
                pattern = "^([1-9]|[1-5][0-2])w$";
                timeUnit = TimeUnit.EveryNWeeks;
                break;
            default:
                pattern = null;
                break;
            }

            if (!(new Regex(pattern)).IsMatch(
                    scheduleForConfigurationFile))
                throw new ParseException(
                    ParseError.InvalidSchedule, null);

            scheduleForUserDisplay =
                scheduleForConfigurationFile.Substring(
                    0, scheduleForConfigurationFile.Length - 1);
        }

        internal
        Schedule(
            string rawSchedulestring)
        {
            scheduleForConfigurationFile = rawSchedulestring;
            ProcessSchedulestring();
        }

        ~Schedule()
        {
        }

        internal override XmlNode
        Serialize(XmlDocument serializedConfiguration)
        {
            XmlNode schedule =
                serializedConfiguration.CreateElement("schedule");

            schedule.InnerText = scheduleForConfigurationFile;
            return schedule;
        }

        public string ScheduleString
        {
            get {
                return scheduleForUserDisplay;
            }
        }

        public TimeUnit TimeUnitOfSchedule
        {
            get {
                return timeUnit;
            }
        }

        string scheduleForConfigurationFile;
        string scheduleForUserDisplay;
        TimeUnit timeUnit;
    };
};
