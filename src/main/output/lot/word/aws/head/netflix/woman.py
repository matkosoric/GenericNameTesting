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
        private const string SubscriptionKey = "f04fa403ce9928479522615cba375a24";

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
