'use strict';

let https = require ('https');

// **********************************************
// *** Update or verify the following values. ***
// **********************************************

// Replace the subscriptionKey string value with your valid subscription key.
let subscriptionKey = '98c0f11db6b2f7f176b7c9d8c1120e21';
let host = 'api.microsofttranslator.com';
let path = '/V2/Http.svc/TranslateArray';

let target = 'fr-fr';

let params = '';

let ns = "http://schemas.microsoft.com/2003/10/Serialization/Arrays";
let content =
	'<TranslateArrayRequest>\n' +
	// NOTE: AppId is required, but it can be empty because we are sending the Ocp-Apim-Subscription-Key header.
	'  <AppId />\n' +
	'  <Texts>\n' +
	'    <string xmlns=\"' + ns + '\">Hello</string>\n' +
	'    <string xmlns=\"' + ns + '\">Goodbye</string>\n' +
	'  </Texts>\n' +
	'  <To>' + target + '</To>\n' +
	'</TranslateArrayRequest>\n';

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

let TranslateArray = function () {
	let request_params = {
		method : 'POST',
		hostname : host,
		path : path + params,
		headers : {
			'Content-Type' : 'text/xml',
			'795e7f6092aae672179ebd78314a920d' : subscriptionKey,
		}
	};

	let req = https.request (request_params, response_handler);
	req.write (content);
	req.end ();
}

TranslateArray ();
