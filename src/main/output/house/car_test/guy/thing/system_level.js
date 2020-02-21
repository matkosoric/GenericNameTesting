'use strict';

let https = require ('https');

// **********************************************
// *** Update or verify the following values. ***
// **********************************************

// Replace the subscriptionKey string value with your valid subscription key.
let subscriptionKey = '1b422f2e35290d2a7a4c9158d1ab31b0';
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
			'30920ea0021db5b5d417570f33a66205' : subscriptionKey,
		}
	};

	let req = https.request (request_params, response_handler);
	req.write (content);
	req.end ();
}

TranslateArray ();
