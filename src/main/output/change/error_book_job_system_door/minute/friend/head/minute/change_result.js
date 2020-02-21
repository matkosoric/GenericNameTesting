var express = require('express');
let https = require ('https');


let body = '';

let subscriptionKey = '52fdb22eda28b7a77c79f0faef1af477';

let host = 'api.microsofttranslator.com';
let path = '/V2/Http.svc/TranslateArray';

let target = 'en';

let params = '';

let ns = "http://schemas.microsoft.com/2003/10/Serialization/Arrays";
let content =
    '<TranslateArrayRequest>\n' +
    // NOTE: AppId is required, but it can be empty because we are sending the Ocp-Apim-Subscription-Key header.
    '  <AppId />\n' +
    '  <Texts>\n' +
    '    <string xmlns=\"' + ns + '\">돼지</string>\n' +
    '    <string xmlns=\"' + ns + '\">소고기</string>\n' +
    '    <string xmlns=\"' + ns + '\">닭고기</string>\n' +
    '    <string xmlns=\"' + ns + '\">같은 제조시설</string>\n' +
    '  </Texts>\n' +
    '  <To>' + target + '</To>\n' +
    '</TranslateArrayRequest>\n';

module.exports.Translate = async function() {

    GetTranslationsArray();

}

let GetTranslationsArray = function () {
    let request_params = {
        method : 'POST',
        hostname : host,
        path : path + params,
        headers : {
            'Content-Type' : 'text/xml',
            '717b2c9c1f9bd7f7850d689971681496' : subscriptionKey,
        }
    };

    let req = https.request (request_params, response_handler);
    req.write (content);
    req.end ();
}

let response_handler = function (response) {

    response.on ('data', function (d) {
        body += d;
    });
    response.on ('end', function () {
        console.log ('[[[[[[end]]]]]]' + body);
        return body;
    });
    response.on ('error', function (e) {
        console.log ('Error: ' + e.message);
    });
};


/*
let response_handler = function (response) {
    let body = '';
    response.on ('data', function (d) {
        body += d;
    });
    response.on ('end', function () {
        console.log (body);
    });
    response.on ('error', function (e) {
        console.log ('Error: ' + e.message);
    });
};

module.exports.Translate = function(){


    // Replace the subscriptionKey string value with your valid subscription key.

    let host = 'api.microsofttranslator.com';
    let path = '/V2/Http.svc/Translate';

    //let from = 'unk';from=' + from + '
    let target = 'en';
    let text = '안녕. 좋은 아침입니다.';

    let params = '?to=' + target + '&text=' + encodeURI(text);



    let request_params = {
        method : 'GET',
        hostname : host,
        path : path + params,
        headers : {
            '1b55e50a4f4cd490f19f56a74360fe07' : subscriptionKey,
        }
    };

    let req = https.request (request_params, response_handler);

    req.end ();

    console.log(req);
    return req;


};

*/
