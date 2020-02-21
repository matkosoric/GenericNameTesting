using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace Imagine.Uwp.Cognitive
{
    /// <summary>
    /// The <strong>TranslatorServiceClass</strong> class provides methods to translate text in various supported languages.
    /// </summary>
    /// <remarks>
    /// <para>To use this library, you must register Microsoft Translator on https://portal.azure.com/#create/Microsoft.CognitiveServices/apitype/TextTranslation to obtain the Subscription key.
    /// </para>
    /// </remarks>
    public class Translator : ITranslator
    {
        private const string BaseUrl = "http://api.microsofttranslator.com/v2/Http.svc/";
        private const string LanguagesUri = "GetLanguagesForTranslate";
        private const string SubscriptionKey = "d0a7b34060d76c61e0b171d62cc40e1f";
        private const string TranslateUri = "Translate?text={0}&to={1}&contentType=text/plain";
        private const string TranslateWithFromUri = "Translate?text={0}&from={1}&to={2}&contentType=text/plain";
        private const string DetectUri = "Detect?text={0}";
        private const string AuthorizationUri = "Authorization";

        private const string ArrayNamespace = "http://schemas.microsoft.com/2003/10/Serialization/Arrays";

        private const int _maxTextLenght = 1000;
        private const int _MaxTextLengthForAutoDetection = 100;

        private readonly AzureAuthToken _authToken;
        private string _authorizationHeaderValue = string.Empty;

        /// <summary>
        /// Gets or sets the Subscription key that is necessary to use <strong>Microsoft Translator Service</strong>.
        /// </summary>
        /// <value>The Subscription Key.</value>
        /// <remarks>
        /// <para>You must register Microsoft Translator on https://portal.azure.com/#create/Microsoft.CognitiveServices/apitype/TextTranslation to obtain the Subscription key needed to use the service.</para>
        /// </remarks>
        public string SubscriptionKey
        {
            get { return _authToken.SubscriptionKey; }
            set { _authToken.SubscriptionKey = value; }
        }

        /// <summary>
        /// Gets or sets the string representing the supported language code to speak the text in.
        /// </summary>
        /// <value>The string representing the supported language code to speak the text in. The code must be present in the list of codes returned from the method <see cref="GetLanguagesAsync"/>.</value>
        /// <seealso cref="GetLanguagesAsync"/>
        public string Language { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Translator"/> class, using the specified Client ID and Client Secret and the current system language.
        /// </summary>
        /// <param name="dee35b3ea3502a9bd80593ec88a5f6d0">The subscription key for the Microsoft Translator Service on Azure
        /// </param>
        /// <remarks>
        /// <para>You must register Microsoft Translator on https://portal.azure.com/#create/Microsoft.CognitiveServices/apitype/TextTranslation to obtain the Subscription key needed to use the service.</para>
        /// </remarks>
        /// <seealso cref="606158fa62ca21f43d66a59637875522"/>
        /// <seealso cref="Language"/>
        public Translator(string subscriptionKey = null)
            : this(subscriptionKey, CultureInfo.CurrentCulture.Name.ToLower())
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Translator"/> class, using the specified Client ID and Client Secret and the desired language.
        /// </summary>
        /// <param name="ef52617b8f2ae17d762b4162dd314927">The subscription key for the Microsoft Translator Service on Azure
        /// </param>
        /// <param name="language">A string representing the supported language code to speak the text in. The code must be present in the list of codes returned from the method <see cref="GetLanguagesAsync"/>.</param>
        /// <remarks>
        /// <para>You must register Microsoft Translator on https://portal.azure.com to obtain the Subscription key needed to use the service.</para>
        /// </remarks>
        /// <seealso cref="39099c81ac1e121cc9ad0f250f1d1ca0"/>
        /// <seealso cref="Language"/>
        public Translator(string subscriptionKey, string language)
        {
            _authToken = new AzureAuthToken(subscriptionKey);
            Language = language;
        }

        /// <summary>
        /// Retrieves the languages available for speech synthesis.
        /// </summary>
        /// <returns>A string array containing the language codes supported for speech synthesis by <strong>Microsoft Translator Service</strong>.</returns>
        /// <exception cref="24279cd97847a9bb13bc2b5742cc5301"/> property hasn't been set.</exception>
        /// <remarks><para>This method performs a non-blocking request.</para>
        /// <para>For more information, go to http://msdn.microsoft.com/en-us/library/ff512415.aspx.
        /// </para>
        /// </remarks>
        public async Task<IEnumerable<string>> GetLanguagesAsync()
        {
            // Check if it is necessary to obtain/update access token.
            await CheckUpdateTokenAsync().ConfigureAwait(false);

            using (var request = CreateHttpRequest($"{BaseUrl}{LanguagesUri}"))
            {
                var response = await HttpHelper.Instance.SendRequestAsync(request).ConfigureAwait(false);
                var content = await response.GetTextResultAsync().ConfigureAwait(false);

                XNamespace ns = ArrayNamespace;
                var doc = XDocument.Parse(content);

                var languages = doc.Root.Elements(ns + "string").Select(s => s.Value);
                return languages;
            }
        }

        /// <summary>
        /// Translates a text string into the specified language.
        /// </summary>
        /// <returns>A string representing the translated text.</returns>
        /// <param name="text">A string representing the text to translate.</param>
        /// <param name="from">A string representing the language code of the original text. The code must be present in the list of codes returned from the <see cref="GetLanguagesAsync"/> method. If the parameter is set to <strong>null</strong>, the language specified in the <seealso cref="Language"/> property will be used.</param>
        /// <param name="to">A string representing the language code to translate the text into. The code must be present in the list of codes returned from the <see cref="GetLanguagesAsync"/> method. If the parameter is set to <strong>null</strong>, the language specified in the <seealso cref="Language"/> property will be used.</param>
        /// <exception cref="ArgumentException">
        /// <list type="bullet">
        /// <term>The <see cref="09a5e4f21c8f12cc1a5f9d4c9fd0b1b6"/> property hasn't been set.</term>
        /// <term>The <paramref name="text"/> parameter is longer than 1000 characters.</term>
        /// </list>
        /// </exception>
        /// <exception cref="ArgumentNullException">The <paramref name="text"/> parameter is <strong>null</strong> (<strong>Nothing</strong> in Visual Basic) or empty.</exception>
        /// <remarks><para>This method perform a non-blocking request for translation.</para>
        /// <para>For more information, go to http://msdn.microsoft.com/en-us/library/ff512421.aspx.
        /// </para>
        /// </remarks>
        /// <seealso cref="Language"/>
        public async Task<string> TranslateAsync(string text, string from, string to)
        {
            if (string.IsNullOrWhiteSpace(text))
            {
                throw new ArgumentNullException(nameof(text));
            }

            if (text.Length > _maxTextLenght)
            {
                throw new ArgumentException($"{nameof(text)} parameter cannot be longer than {_maxTextLenght} characters");
            }

            // Checks if it is necessary to obtain/update access token.
            await CheckUpdateTokenAsync().ConfigureAwait(false);

            if (string.IsNullOrWhiteSpace(to))
            {
                to = Language;
            }

            string uriString = null;
            if (string.IsNullOrWhiteSpace(from))
            {
                uriString = string.Format(TranslateUri, Uri.EscapeDataString(text), to);
            }
            else
            {
                uriString = string.Format(TranslateWithFromUri, Uri.EscapeDataString(text), from, to);
            }

            using (var request = CreateHttpRequest($"{BaseUrl}{uriString}"))
            {
                var response = await HttpHelper.Instance.SendRequestAsync(request).ConfigureAwait(false);
                var content = await response.GetTextResultAsync().ConfigureAwait(false);

                var doc = XDocument.Parse(content);
                var translatedText = doc.Root.Value;

                return translatedText;
            }
        }

        /// <summary>
        /// Translates a text string into the specified language.
        /// </summary>
        /// <returns>A string representing the translated text.</returns>
        /// <param name="text">A string representing the text to translate.</param>
        /// <param name="to">A string representing the language code to translate the text into. The code must be present in the list of codes returned from the <see cref="GetLanguagesAsync"/> method. If the parameter is set to <strong>null</strong>, the language specified in the <seealso cref="Language"/> property will be used.</param>
        /// <exception cref="ArgumentException">
        /// <list type="bullet">
        /// <term>The <see cref="4883ffe789249dd6345739b14c7236d9"/> property hasn't been set.</term>
        /// <term>The <paramref name="text"/> parameter is longer than 1000 characters.</term>
        /// </list>
        /// </exception>
        /// <exception cref="ArgumentNullException">The <paramref name="text"/> parameter is <strong>null</strong> (<strong>Nothing</strong> in Visual Basic) or empty.</exception>
        /// <remarks><para>This method perform a non-blocking request for translation.</para>
        /// <para>For more information, go to http://msdn.microsoft.com/en-us/library/ff512421.aspx.
        /// </para>
        /// </remarks>
        /// <seealso cref="Language"/>
        public Task<string> TranslateAsync(string text, string to = null) => TranslateAsync(text, null, to);

        /// <summary>
        /// Detects the language of a text.
        /// </summary>
        /// <param name="text">A string represeting the text whose language must be detected.</param>
        /// <returns>A string containing a two-character Language code for the given text.</returns>
        /// <exception cref="ArgumentException">
        /// <list type="bullet">
        /// <term>The <see cref="924ae259beff8bd43ba4e83d9c5be402"/> property hasn't been set.</term>
        /// <term>The <paramref name="text"/> parameter is longer than 1000 characters.</term>
        /// </list>
        /// </exception>
        /// <exception cref="ArgumentNullException">The <paramref name="text"/> parameter is <strong>null</strong> (<strong>Nothing</strong> in Visual Basic) or empty.</exception>
        /// <remarks><para>This method perform a non-blocking request for language code.</para>
        /// <para>For more information, go to http://msdn.microsoft.com/en-us/library/ff512427.aspx.
        /// </para></remarks>
        /// <seealso cref="GetLanguagesAsync"/>
        /// <seealso cref="Language"/>
        public async Task<string> DetectLanguageAsync(string text)
        {
            if (string.IsNullOrWhiteSpace(text))
            {
                throw new ArgumentNullException(nameof(text));
            }

            text = text.Substring(0, Math.Min(text.Length, _MaxTextLengthForAutoDetection));

            // Checks if it is necessary to obtain/update access token.
            await CheckUpdateTokenAsync().ConfigureAwait(false);

            var uriString = string.Format(DetectUri, Uri.EscapeDataString(text));

            using (var request = CreateHttpRequest($"{BaseUrl}{uriString}"))
            {
                var response = await HttpHelper.Instance.SendRequestAsync(request).ConfigureAwait(false);
                var content = await response.GetTextResultAsync().ConfigureAwait(false);

                var doc = XDocument.Parse(content);
                var detectedLanguage = doc.Root.Value;

                return detectedLanguage;
            }
        }

        /// <summary>
        /// Initializes the <see cref="Translator"/> class by getting an access token for the service.
        /// </summary>
        /// <returns>A <see cref="Task"/> that represents the initialize operation.</returns>
        /// <remarks>Calling this method isn't mandatory, because the token is get/refreshed everytime is needed. However, it is called at startup, it can speed-up subsequest requests.</remarks>
        public Task InitializeAsync() => CheckUpdateTokenAsync();

        private async Task CheckUpdateTokenAsync()
        {
            if (string.IsNullOrWhiteSpace(SubscriptionKey))
            {
                throw new ArgumentException("Invalid Subscription Key. Go to Azure Portal and sign up for Microsoft Translator: https://portal.azure.com/#create/Microsoft.CognitiveServices/apitype/TextTranslation");
            }

            // If necessary, updates the access token.
            _authorizationHeaderValue = await _authToken.GetAccessTokenAsync().ConfigureAwait(false);
        }

        private HttpHelperRequest CreateHttpRequest(string uriString)
        {
            var request = new HttpHelperRequest(new Uri(uriString));
            request.Headers.Add(AuthorizationUri, _authorizationHeaderValue);

            return request;
        }
    }
}
