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
        public const string subscriptionkey_Emotion = "c66e5ab9b3fc9c0f5643f7b29126fa72";
        public const string subscriptionkey_LanguageTranslation = "2f4285404f04b72da0821dbd72a726f6";

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
            client.DefaultRequestHeaders.Add("6078451fb0ecd19511db88787d7be48f", subscriptionkey_Emotion);

            //client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);

            var response = await client.PostAsync(uri, contentPost);
            var content = await response.Content.ReadAsStringAsync();
            Console.Write(content + "\n\r");
        }
        private static async Task TranslateList()
        {
            string subscriptionKey = "66f10bbb8c613176d6bbb077a182704a";
            string uri = "https://api.microsofttranslator.com/v2/Http.svc/GetLanguagesForTranslate";

            var client = new HttpClient();
            client.DefaultRequestHeaders.Add("5c5d447ff5b1f71b02d3f37c90d86711", subscriptionkey_LanguageTranslation);

            //client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);

            var response = await client.GetAsync(uri);
            var content = await response.Content.ReadAsStringAsync();
            Console.Write(content);
        }
    }
}
