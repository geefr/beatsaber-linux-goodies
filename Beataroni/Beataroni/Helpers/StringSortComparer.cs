using System.Collections.Generic;
using System.Globalization;
namespace Beataroni.Helpers
{
    /// <summary>
    /// Compare strings case insensitive and using en-US culture.
    /// </summary>
    public class StringInvariantSortComparerUS : IComparer<string>
    {
        public int Compare(string first, string second)
        {
            int compareResult = string.Compare(first, second, System.Globalization.CultureInfo.GetCultureInfo("en-US"), CompareOptions.IgnoreCase | CompareOptions.StringSort);
            if (compareResult > 0)
            {
                return 1;
            }
            else if (compareResult < 0)
            {
                return -1;
            }

            return 0;
        }
    }
}