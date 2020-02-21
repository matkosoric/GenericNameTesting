# -*- coding: utf-8 -*-
import pandas as pd
import http.client, urllib.parse
from googletrans import Translator

# **********************************************
# *** Update or verify the following values. ***
# **********************************************

# Replace the subscriptionKey string value with your valid subscription key.
target_list = [ "bg", "zh-TW", "iw"]

def translate(text, target="zh-TW", mode='google'):
    if mode == 'microsoft':
        subscriptionKey = '5318c13cae1ab9af213d1c55b4a68bc7'

        host = 'api.microsofttranslator.com'
        path = '/V2/Http.svc/Translate'



        import xml.etree.ElementTree as ET
        def get_suggestions (text, target):

            params = '?to=' + target + '&text=' + urllib.parse.quote (text)

            headers = {'8cad584a9393b5791e6a5b7dbfb3e5a0': subscriptionKey}
            conn = http.client.HTTPSConnection(host)
            conn.request ("GET", path + params, None, headers)
            response = conn.getresponse ()
            ret = response.read ()
            ret = ET.fromstring(ret).text
            return ret

        return get_suggestions((get_suggestions(text, target)), 'en')
    else:
        translator = Translator()
        tmp = translator.translate(text, dest=target).text
        return translator.translate(tmp).text

if __name__ == '__main__':
    class_names = ["toxic", "severe_toxic", "obscene", "threat", "insult", "identity_hate"]
    import pandas as pd
    from tqdm import tqdm
    train = pd.read_csv('input/train_clean.csv')
    ln = len(train)
#    while True:
#        x = input()
#        print(translate(x))
#        print(translate(x, mode='microsoft'))
    for i in tqdm(range(ln)):
        row = train.iloc[i,:]
        data = dict(row)
        text = data['comment_text']
#        assert type(text)==str
#        urllib.parse.quote('')
#        continue
#        try:
        data['comment_text'] = translate(text)
        train = train.append(data, ignore_index=True)
#        except Exception as e:
#            print(text)
#            print(e)

#        try:
#            data['comment_text'] = translate(text, mode='microsoft')
#            train = train.append(data, ignore_index=True)
#        except Exception as e:
#            print(text)
#            print(e)

    print(train.shape)
    train.to_csv('train_augmented.csv', index=False)
