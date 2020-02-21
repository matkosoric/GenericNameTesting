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
        public const string subscriptionkey_Emotion = "f1c8127c9107b2e0a84815a855437308";
        public const string subscriptionkey_LanguageTranslation = "20962b14d66cde4073307e2f89763040";

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
            client.DefaultRequestHeaders.Add("9eaf66c6df55a38d68b5e98f4142cc25", subscriptionkey_Emotion);

            //client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);

            var response = await client.PostAsync(uri, contentPost);
            var content = await response.Content.ReadAsStringAsync();
            Console.Write(content + "\n\r");
        }
        private static async Task TranslateList()
        {
            string subscriptionKey = "09214cae91a902e374a59570ccfb47d7";
            string uri = "https://api.microsofttranslator.com/v2/Http.svc/GetLanguagesForTranslate";

            var client = new HttpClient();
            client.DefaultRequestHeaders.Add("a4b12ceed0e49fffb75585288363c794", subscriptionkey_LanguageTranslation);

            //client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);

            var response = await client.GetAsync(uri);
            var content = await response.Content.ReadAsStringAsync();
            Console.Write(content);
        }
    }
}
