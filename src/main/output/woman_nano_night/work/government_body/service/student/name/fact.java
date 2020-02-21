# -*- coding: utf-8 -*-

import http.client, urllib.parse

# **********************************************
# *** Update or verify the following values. ***
# **********************************************

# Replace the subscriptionKey string value with your valid subscription key.
subscriptionKey = 'd229ef3ed94f5a2597b21f3bd640d2da'

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
		'e8628347298c1734e601be0855dd679f': subscriptionKey,
		'Content-type': 'text/xml'
	}
	conn = http.client.HTTPSConnection(host)
	conn.request ("POST", path + params, body, headers)
	response = conn.getresponse ()
	return response.read ()

result = TranslateArray ()
print (result.decode("utf-8"))
