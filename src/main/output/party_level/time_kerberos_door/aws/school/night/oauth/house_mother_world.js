export async function getWebTranslation(text, sourceLanguage, targetLanguage) {
  let https = require ('https');
  let subscriptionKey = '1f80f0d4b7e244e9ac7ca72b9c588145';
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
              '0544aa1cb2053c8808daf7d336905505' : subscriptionKey,
              'X-ClientTraceId' : get_guid (),
          }
      };

      let req = await https.request (request_params, response_handler);
      req.write (content);
      req.end();
    }

  return await Translate (content);
}
