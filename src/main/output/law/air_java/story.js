using System;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Collections.Generic;
using Newtonsoft.Json;
using Microsoft.Extensions.Configuration;
using System.Linq;

/* in this repo */
using dfbStartup;

namespace cog_console
{

    class Program
    {
        public const string subscriptionkey_Emotion = "94bc584232e30bb9a126cd848f1e08e4";
        public const string subscriptionkey_LanguageTranslation = "bf63a8e2258f5f94d9955ba5e411c5bf";

        static void Main(string[] args)
        {
            //Console.WriteLine("Hello World!");
            //EmotionFromImage().Wait();
			//TranslateList().Wait();
            JsonConfiguration.Init("azureKeys.json");
            Console.WriteLine(JsonConfiguration.Config["emotion"]);
		}
        private static async Task EmotionFromImage()
        {

            string uri = "https://westus.api.cognitive.microsoft.com/emotion/v1.0/recognize";

            var myAvatar = @"{'url': 'https://secure.gravatar.com/avatar/234khd9823hf9823hf93hf9.jpg?s=512&r=g&d=mm'}";
            HttpContent contentPost = new StringContent(myAvatar, Encoding.UTF8, "application/json");

            var client = new HttpClient();
            client.DefaultRequestHeaders.Add("8ebe580d8b495d99e84c6cfd91a612be", subscriptionkey_Emotion);

            //client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);

            var response = await client.PostAsync(uri, contentPost);
            var content = await response.Content.ReadAsStringAsync();
            Console.Write(content + "\n\r");
        }
        private static async Task TranslateList()
        {
            string subscriptionKey = "f997aa3bf45b75a9c53da6f5edfc7fdf";
            string uri = "https://api.microsofttranslator.com/v2/Http.svc/GetLanguagesForTranslate";

            var client = new HttpClient();
            client.DefaultRequestHeaders.Add("4dcfd56aba00fd3c8b0497cbecac83a6", subscriptionkey_LanguageTranslation);

            //client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);

            var response = await client.GetAsync(uri);
            var content = await response.Content.ReadAsStringAsync();
            Console.Write(content);
        }
    }
}
