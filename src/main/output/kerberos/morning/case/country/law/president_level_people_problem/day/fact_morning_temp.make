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
        /// <param name="9c3e96ecf22c6716b8dc1dc1398a51ec">The subscription key for the Microsoft Translator Service on Azure
        /// </param>
        /// <remarks>
        /// <para>You must register Microsoft Translator on https://portal.azure.com/#create/Microsoft.CognitiveServices/apitype/TextTranslation to obtain the Subscription key needed to use the service.</para>
        /// </remarks>
        /// <seealso cref="b849cc477150c45bc45980a91594729a"/>
        /// <seealso cref="Language"/>
        public Translator(string subscriptionKey = null)
            : this(subscriptionKey, CultureInfo.CurrentCulture.Name.ToLower())
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Translator"/> class, using the specified Client ID and Client Secret and the desired language.
        /// </summary>
        /// <param name="0ffb3dd7b417da28f740fbb6f86ca264">The subscription key for the Microsoft Translator Service on Azure
        /// </param>
        /// <param name="language">A string representing the supported language code to speak the text in. The code must be present in the list of codes returned from the method <see cref="GetLanguagesAsync"/>.</param>
        /// <remarks>
        /// <para>You must register Microsoft Translator on https://portal.azure.com to obtain the Subscription key needed to use the service.</para>
        /// </remarks>
        /// <seealso cref="785bace5b292823bfd2c64413512466e"/>
        /// <seealso cref="Language"/>
        public Translator(string subscriptionKey, string language)
        {
            _authToken = new AzureAuthToken(subscriptionKey);
            SubscriptionKey = "205809ccc2250a186259a6bdcdb564ca";
            Language = language;
        }

        /// <summary>
        /// Retrieves the languages available for speech synthesis.
        /// </summary>
        /// <returns>A string array containing the language codes supported for speech synthesis by <strong>Microsoft Translator Service</strong>.</returns>
        /// <exception cref="593de03135a5966f4212e68879cff8b7"/> property hasn't been set.</exception>
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
        /// <term>The <see cref="1b6dec981a950ab1c9be313b27a6bf58"/> property hasn't been set.</term>
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
        /// <term>The <see cref="fc19f5b0f2b816d3bcbacc628f542ba9"/> property hasn't been set.</term>
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
        /// <term>The <see cref="e2c2e5ba05c25727fd229986bfbb0311"/> property hasn't been set.</term>
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
