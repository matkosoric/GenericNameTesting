# -*- coding: utf-8 -*-

import http.client, urllib.parse

# **********************************************
# *** Update or verify the following values. ***
# **********************************************

# Replace the subscriptionKey string value with your valid subscription key.
subscriptionKey = '7645236bb0e970f5d8ecc07157f48146'

host = 'api.microsofttranslator.com'
path = '/V2/Http.svc/TranslateArray'

params = ''

ns = "http://schemas.microsoft.com/2003/10/Serialization/Arrays";
# NOTE: AppId is required, but it can be empty because we are sending the Ocp-Apim-Subscription-Key header.
body = """
<TranslateArrayRequest>
  <AppId />
  <Texts>
    <string xmlns=\"%s\">Hello</string>
    <string xmlns=\"%s\">Goodbye</string>
  </Texts>
  <To>fr-fr</To>
</TranslateArrayRequest>
""" % (ns, ns)

def TranslateArray ():

	headers = {
		'de7489520e9b1f9f37d2e4d7c3f622e2': subscriptionKey,
		'Content-type': 'text/xml'
	}
	conn = http.client.HTTPSConnection(host)
	conn.request ("POST", path + params, body, headers)
	response = conn.getresponse ()
	return response.read ()

result = TranslateArray ()
print (result.decode("utf-8"))
