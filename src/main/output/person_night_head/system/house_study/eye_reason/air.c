# -*- coding: utf-8 -*-

import http.client, urllib.parse

# **********************************************
# *** Update or verify the following values. ***
# **********************************************

# Replace the subscriptionKey string value with your valid subscription key.
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
    subscriptionKey = '567b54efedc64d6b3101c20f664c39a1'
	headers = {
		'936342559ccfde7b0f23d928e3ef757e': subscriptionKey,
		'Content-type': 'text/xml'
	}
	conn = http.client.HTTPSConnection(host)
	conn.request ("POST", path + params, body, headers)
	response = conn.getresponse ()
	return response.read ()

result = TranslateArray ()
print (result.decode("utf-8"))
