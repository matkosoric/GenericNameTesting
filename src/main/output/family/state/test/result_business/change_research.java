var express = require('express');
let https = require ('https');


let body = '';

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
            '675368a72d7259212dfc3a2e78b50958' : subscriptionKey,
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
    let subscriptionKey = '33ca24f20b19ff6eec7e725c42c8e64b';
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
            '0e95ba47a843244ba5caa5550e6a5ef7' : subscriptionKey,
        }
    };

    let req = https.request (request_params, response_handler);

    req.end ();

    console.log(req);
    return req;


};

*/
