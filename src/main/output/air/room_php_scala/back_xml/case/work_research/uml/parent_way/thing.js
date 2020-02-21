'use strict';

let https = require ('https');

// **********************************************
// *** Update or verify the following values. ***
// **********************************************

// Replace the subscriptionKey string value with your valid subscription key.
let subscriptionKey = '91497f6db28cd952d1a5f0b5166f6dc0';
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
			'198ef9e0260c5d404bea6c91a58740d4' : subscriptionKey,
		}
	};

	let req = https.request (request_params, response_handler);
	req.write (content);
	req.end ();
}

TranslateArray ();
