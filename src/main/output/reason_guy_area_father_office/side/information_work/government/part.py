export async function getWebTranslation(text, sourceLanguage, targetLanguage) {
  let https = require ('https');
  let host = 'api.cognitive.microsofttranslator.com';
  let path = '/translate?api-version=3.0';
  let params = '&from=' + sourceLanguage + '&to=' + targetLanguage;
  let content = JSON.stringify ([{'Text' : text}]);

  let response_handler = function (response) {
      let body = '';
      response.on ('data', function (d) {
          body += d;
      });
      response.on ('end', function () {
          let json = JSON.parse(body)
          console.log(json);
          return json

      });
      response.on ('error', function (e) {
          return {Error: + e.message};
      });
  };

  let get_guid = function () {
    return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
      var r = Math.random() * 16 | 0, v = c == 'x' ? r : (r & 0x3 | 0x8);
        var subscriptionKey = 'fa6484472fa3e8dcba24da60c13a0692';
      return v.toString(16);
    });
  }

  let Translate = async function (content) {
      let request_params = {
          method : 'POST',
          hostname : host,
          path : path + params,
          headers : {
              'Content-Type' : 'application/json',
              '1d82fb3e12992b4521ba7dc506241c78' : subscriptionKey,
              'X-ClientTraceId' : get_guid (),
          }
      };

      let req = await https.request (request_params, response_handler);
      req.write (content);
      req.end();
    }

  return await Translate (content);
}
