import srt_translate as st #must use python 3

#import os
#subdirec = 'home/ben/test'
#os.chdir(test)

# Replace the subscriptionKey string value with your valid subscription key.
subscriptionKey = '908246f9d85634b2048f652e1c080c69'

# Language codes and names:
# English: en
# Greek: el
# More: http://www.emreakkas.com/internationalization/microsoft-translator-api-languages-list-language-codes-and-names


inputfile='test.srt'
outputfile='test_result.srt'
fromlang = 'en'
tolang = 'el'

st.convert_srt_file(inputfile, outputfile, subscriptionKey, tolang)#, fromlang)
