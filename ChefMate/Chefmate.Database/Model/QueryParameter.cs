namespace Chefmate.Database.Model
{
    public class QueryParameter
    {
        public string ParameterName { get; set; }
        public object ParameterValue { get; set; }
        public QueryParameter(string parameterName, object parameterValue)
        {
            ParameterName = parameterName;
            ParameterValue = parameterValue;
        }
    }
}