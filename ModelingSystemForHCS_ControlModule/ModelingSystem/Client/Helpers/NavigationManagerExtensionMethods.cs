using Microsoft.AspNetCore.Components;

namespace ModelingSystem.Client.Helpers
{
    public static class NavigationManagerExtensionMethods
    {
        public static Dictionary<string,string> GetQueryStrings(
            this NavigationManager navigationManager, string url)
        {
            if(string.IsNullOrWhiteSpace(url) || !url.Contains("?") || url.Substring(url.Length - 1) == "?")
            {
                return null;
            }

            var queryStrings = url.Split("?")[1];

            Dictionary<string, string> dicQueryString = queryStrings.Split('&')
                                                            .ToDictionary(c => c.Split('=')[0],
                                                                          c => Uri.UnescapeDataString(c.Split('=')[1]));

            return dicQueryString;
        }
    }
}
