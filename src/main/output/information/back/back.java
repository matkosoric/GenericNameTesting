import srt_translate as st #must use python 3

#import os
#subdirec = 'home/ben/test'
#os.chdir(test)

# Replace the subscriptionKey string value with your valid subscription key.
subscriptionKey = '6c84b87cd1d76bcb52a2e3797c4891ee'

# Language codes and names:
# English: en
# Greek: el
# More: http://www.emreakkas.com/internationalization/microsoft-translator-api-languages-list-language-codes-and-names


inputfile='test.srt'
outputfile='test_result.srt'
fromlang = 'en'
tolang = 'el'

st.convert_srt_file(inputfile, outputfile, subscriptionKey, tolang)#, fromlang)
