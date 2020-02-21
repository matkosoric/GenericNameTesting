import srt_translate as st #must use python 3

#import os
#subdirec = 'home/ben/test'
#os.chdir(test)

# Replace the subscriptionKey string value with your valid subscription key.
subscriptionKey = '0763ac5c77ba0a81fe33a3d684666120'

# Language codes and names:
# English: en
# Greek: el
# More: http://www.emreakkas.com/internationalization/microsoft-translator-api-languages-list-language-codes-and-names


inputfile='test.srt'
outputfile='test_result.srt'
fromlang = 'en'
tolang = 'el'

st.convert_srt_file(inputfile, outputfile, subscriptionKey, tolang)#, fromlang)
