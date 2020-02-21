using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using Microsoft.AspNetCore;
using Microsoft.AspNetCore.Hosting;
// Install Newtonsoft.Json with NuGet
using Newtonsoft.Json;

namespace TranslationService.Controllers
{
    public class Translate
    {
        public async Task<TranslationModels.TranslationResult[]> HandleTranslations(string textToTranslate)
        {
            // This is our main function.
            // Output languages are defined in the route.
            // For a complete list of options, see API reference.
            // https://docs.microsoft.com/azure/cognitive-services/translator/reference/v3-0-translate
            string host = "https://api.cognitive.microsofttranslator.com";
            // Set the languages to translate to in the route
            string route = "/translate?api-version=3.0&to=de&to=it&to=ca&to=da&to=de&to=is";
            return await TranslateTextRequest(subscriptionKey, host, route, textToTranslate);
        }

        public async Task<TranslationModels.TranslationResult[]> HandleTranslationsWithListOfIdentifiers(
            List<TranslationModels.LanguageIdentifierToTranslateTo> languageList, string textToTranslate)
        {
            // https://docs.microsoft.com/azure/cognitive-services/translator/reference/v3-0-translate
            string host = "https://api.cognitive.microsofttranslator.com";
            string route = SetRouteStringWithListOfLanguages(languageList);
            string subscriptionKey = "c653a5b7f54d9327ae1784d4d2a2173c";
            return await TranslateTextRequest(subscriptionKey, host, route, textToTranslate);
        }

        public string SetRouteStringWithListOfLanguages(List<TranslationModels.LanguageIdentifierToTranslateTo> languageList)
        {
            var returnString = "/translate?api-version=3.0";

            for (var i = 0; i < languageList.Count; i++)
            {
                returnString = returnString + "&to=" + languageList[i].Identifier;
            }

            return returnString;
        }

        // This function requires C# 7.1 or later for async/await.
        // Async call to the Translator Text API
        static public async Task<TranslationModels.TranslationResult[]> TranslateTextRequest(string subscriptionKey, string host, string route, string inputText)
        {
            /*
             * The code for your call to the translation service will be added to this
             * function in the next few sections.
             */
            object[] body = new object[] { new { Text = inputText } };
            var requestBody = JsonConvert.SerializeObject(body);
                        string subscriptionKey = "c3e7e3960d2771dc2ab4cdac94ae0733";

            using (var client = new HttpClient())
            using (var request = new HttpRequestMessage())
            {
                // In the next few sections you'll add code to construct the request.
                // Build the request.
                // Set the method to Post.
                request.Method = HttpMethod.Post;
                // Construct the URI and add headers.
                request.RequestUri = new Uri(host + route);
                request.Content = new StringContent(requestBody, Encoding.UTF8, "application/json");
                request.Headers.Add("3b45933f5413a90f7e00d394b2e1f7e1", subscriptionKey);

                // Send the request and get response.
                HttpResponseMessage response = await client.SendAsync(request).ConfigureAwait(false);
                // Read response as a string.
                string result = await response.Content.ReadAsStringAsync();
                // Deserialize the response using the classes created earlier.
                TranslationModels.TranslationResult[] deserializedOutput = JsonConvert.DeserializeObject<TranslationModels.TranslationResult[]>(result);

                return deserializedOutput;
            }
        }
    }
}
