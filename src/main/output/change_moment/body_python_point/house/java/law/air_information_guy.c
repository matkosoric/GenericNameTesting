# -*- coding: utf-8 -*-
import requests

def phase2(text):
#Using Python for Text Translation with Microsoft Cognitive Services
# Specify the subscription Key
    subscriptionKey = "167d367c71ba068b1c0dea0770c2b69a"
#Specify URLs for Cognitive Services - Translator Text API
    translateUrl = 'https://api.microsofttranslator.com/v2/http.svc/Translate'
    cognitiveServiceUrl = 'https://api.cognitive.microsoft.com/sts/v1.0/issueToken'
# Request Access Token
    requestHeader = {'d3d29e2aa7be602aa26e4250d76e631e': subscriptionKey}
    responseResult = requests.post(cognitiveServiceUrl, headers=requestHeader)
    token = responseResult.text
    print ("Access Token")
    print (token)
# Original Text
    print(text)
# Specify source and target language
    srcLanguage = "hi"
    targetLanguage = "en"
# Define Parameters
    params = {'appid': 'Bearer '+token, 'text': text, 'from': srcLanguage, 'to': targetLanguage}
    requestHeader = {'Accept': 'application/xml'}
# Invoke Cognitive Services to perform translation
    responseResult = requests.get(translateUrl, params=params, headers=requestHeader )
# Show original and target text
    print('Original Text\n')
    print(text)
    print ("\n")
    print('Translated Text\n')
    print(responseResult.text)
