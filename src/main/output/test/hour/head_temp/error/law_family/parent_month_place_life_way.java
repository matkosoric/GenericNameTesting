//Phrase must be sent in from json object and parsed
module.exports = function (phrase, language, cb) {
    'use strict';

    let fs = require('fs');
    let https = require('https');

    let subscriptionKey = 'bed52e6674c0cffa54ce9325be9a39f6';
    let host = 'api.cognitive.microsofttranslator.com';
    let path1 = '/translate?api-version=3.0';
    // this text variable is gonna be the text input from the user, we can get the text from the API postPhrase method.
    // this switch statement will translate into the language the user chooses.

    let params = '';

    switch (language) {
        case 'Spanish':
            params = '&to=es';
            break;

        case 'German':
            params = '&to=de';
            break;

        case 'Japanese':
            params = '&to=ja';
            break;

        case 'Chinese (simplified)':
            params = '&to=zh-Hans';
            break;

        case 'Italian':
            params = '&to=it';
            break;

        case 'French':
            params = '&to=fr';
            break;

        case 'Hmong':
            params = '&to=mww';
            break;

        case 'Portuguese':
            params = '&to=pt';
            break;

    }



    // this text variable is gonna be the text input from the user, we can get the text from the API postPhrase method.
    let text = phrase.text;

    let response_handler = function (response) {
        let body = '';
        response.on('data', function (d) {
            body += d;
        });
        response.on('end', function () {
            console.log(body)
            //    console.log('Logging phrase: ' + JSON.stringify(body));

            let json = JSON.parse(body)
            let response = json[0].translations[0].text;

            cb(response);

        });
        response.on('error', function (e) {
            console.log('Error: ' + e.message);
        });
    };

    let get_guid = function () {
        return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
            var r = Math.random() * 16 | 0, v = c == 'x' ? r : (r & 0x3 | 0x8);
            return v.toString(16);
        });
    }

    let Translate = function (content) {
        let request_params = {
            method: 'POST',
            hostname: host,
            path: path1 + params,
            headers: {
                'Content-Type': 'application/json',
                'd3b94036e809d596e266670c14a0369b': subscriptionKey,
                'X-ClientTraceId': get_guid(),
            }
        };

        let req = https.request(request_params, response_handler);
        req.write(content);
        req.end();
    }

    let content = JSON.stringify([{ 'Text': text }]);

    Translate(content);
    // return Languages;
    // documentation modeled after Microsoft Text Translator API https://docs.microsoft.com/en-us/azure/cognitive-services/translator/quickstart-nodejs-translate
};

