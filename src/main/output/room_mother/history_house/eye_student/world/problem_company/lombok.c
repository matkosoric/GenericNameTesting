using CategoriesPOC.TranslatorService;
using System;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;


namespace CategoriesPOC.Helpers
{
    public static class TranslatorHelper
    {
        private const string SubscriptionKey = "4697087fd92a3b9a357413d7a179c5c9";   //Enter here the Key from your Microsoft Translator Text subscription on http://portal.azure.com
        public static Task<string> Translate(string word, string lang="")
        {
            var translatorService = new TranslatorService.LanguageServiceClient();
            var authTokenSource = new AzureAuthToken(SubscriptionKey);
            var token = string.Empty;
            lang = string.IsNullOrEmpty(lang) ? DetectLanguage(word).Result : lang;

            if (lang == "en") return Task.FromResult<string>(word);

            try
            {
                token =  authTokenSource.GetAccessToken();
                return  translatorService.TranslateAsync(token, word, lang, "en", "text/plain", "general", string.Empty);
            }
            catch (HttpRequestException)
            {
                switch (authTokenSource.RequestStatusCode)
                {
                    case HttpStatusCode.Unauthorized:
                        Console.WriteLine("Request to token service is not authorized (401). Check that the Azure subscription key is valid.");
                        break;
                    case HttpStatusCode.Forbidden:
                        Console.WriteLine("Request to token service is not authorized (403). For accounts in the free-tier, check that the account quota is not exceeded.");
                        break;
                }
                throw;
            }

            //Console.WriteLine("Translated to French: {0}", translatorService.Translate(token, "Hello World", "en", "fr", "text/plain", "general", string.Empty));
        }

        public static Task<GetTranslationsResponse> GetTranslations(string word, string lang = "")
        {
            var translatorService = new TranslatorService.LanguageServiceClient();
            var authTokenSource = new AzureAuthToken(SubscriptionKey);
            var token = string.Empty;
            lang = string.IsNullOrEmpty(lang) ? DetectLanguage(word).Result : lang;

            try
            {
                token = authTokenSource.GetAccessToken();
                var options = new TranslateOptions();
                return translatorService.GetTranslationsAsync(token, word, lang, "en", 20, options);
            }
            catch (HttpRequestException)
            {
                switch (authTokenSource.RequestStatusCode)
                {
                    case HttpStatusCode.Unauthorized:
                        Console.WriteLine("Request to token service is not authorized (401). Check that the Azure subscription key is valid.");
                        break;
                    case HttpStatusCode.Forbidden:
                        Console.WriteLine("Request to token service is not authorized (403). For accounts in the free-tier, check that the account quota is not exceeded.");
                        break;
                }
                throw;
            }
        }
        public static Task<string> DetectLanguage(string str)
        {
            var translatorService = new TranslatorService.LanguageServiceClient();
            var authTokenSource = new AzureAuthToken(SubscriptionKey);
            var token = string.Empty;

            try
            {
                token =  authTokenSource.GetAccessToken();
                return translatorService.DetectAsync(token, str);
            }
            catch (HttpRequestException)
            {
                switch (authTokenSource.RequestStatusCode)
                {
                    case HttpStatusCode.Unauthorized:
                        Console.WriteLine("Request to token service is not authorized (401). Check that the Azure subscription key is valid.");
                        break;
                    case HttpStatusCode.Forbidden:
                        Console.WriteLine("Request to token service is not authorized (403). For accounts in the free-tier, check that the account quota is not exceeded.");
                        break;
                }
                throw;
            }

            //translatorService.Detect(token, str);
        }
    }
}
