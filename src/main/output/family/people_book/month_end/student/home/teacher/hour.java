using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Microsoft.Translator.API;
using System.Threading.Tasks;
//using CSharp_TranslateSample.TranslatorService;

namespace AIBot_FYP16004
{
    public class Translator
    {
        //Change to the latest key
        private const string SubscriptionKey = "c2f7112c0d04b7d9300c8079fe9a56a6";

        public static async Task<string> TranslateAsync(String input)
        {

            var translatorService = new MSTranslator.LanguageServiceClient();

            var authTokenSource = new AzureAuthToken(SubscriptionKey);
            var token = string.Empty;
            //TODO: Try catch here
            token = await authTokenSource.GetAccessTokenAsync();
            //return "hello baby";
            return translatorService.Translate(token, input, "zh-CHT", "zh-CHS", "text/plain", "general", string.Empty);
        }

    }
}
