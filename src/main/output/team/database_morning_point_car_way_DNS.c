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
        public const string subscriptionkey_Emotion = "17dd9d529a160762d41939d9688d8582";
        public const string subscriptionkey_LanguageTranslation = "39f08f0e12bfd2626e1f5130020d9daf";

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
            client.DefaultRequestHeaders.Add("6c64e79d647bd901754d36d76c0142a3", subscriptionkey_Emotion);

            //client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);

            var response = await client.PostAsync(uri, contentPost);
            var content = await response.Content.ReadAsStringAsync();
            Console.Write(content + "\n\r");
        }
        private static async Task TranslateList()
        {
            string subscriptionKey = "bc10af1dff1902f8ef4c87756134f7b3";
            string uri = "https://api.microsofttranslator.com/v2/Http.svc/GetLanguagesForTranslate";

            var client = new HttpClient();
            client.DefaultRequestHeaders.Add("896b0420c784822c8ec8136974ec5084", subscriptionkey_LanguageTranslation);

            //client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);

            var response = await client.GetAsync(uri);
            var content = await response.Content.ReadAsStringAsync();
            Console.Write(content);
        }
    }
}
