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
        public const string subscriptionkey_Emotion = "37902e0e5e5a2affdb88c0720411661a";
        public const string subscriptionkey_LanguageTranslation = "c653c8684fb36753e92e24d1c285926b";

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
            client.DefaultRequestHeaders.Add("c21d571b9b34ed8372b16a1ea2cf5476", subscriptionkey_Emotion);

            //client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);

            var response = await client.PostAsync(uri, contentPost);
            var content = await response.Content.ReadAsStringAsync();
            Console.Write(content + "\n\r");
        }
        private static async Task TranslateList()
        {
            string subscriptionKey = "3575821b09854386214ada3c2560321e";
            string uri = "https://api.microsofttranslator.com/v2/Http.svc/GetLanguagesForTranslate";

            var client = new HttpClient();
            client.DefaultRequestHeaders.Add("09ae4f706f5077bf478145e4d1614d81", subscriptionkey_LanguageTranslation);

            //client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);

            var response = await client.GetAsync(uri);
            var content = await response.Content.ReadAsStringAsync();
            Console.Write(content);
        }
    }
}
