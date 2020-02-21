const request = require('request')
const uuidv4 = require('uuid/v4')
const { LimitReachedError } = require('./errors')

const ITEMS_IN_REQUEST_LIMIT = 25
const REQUEST_CHAR_LIMIT = 5000
const CHAR_PER_HOUR_LIMIT = 2000000

// const subscriptionKey = process.env.TRANSLATOR_TEXT_KEY;
// if (!subscriptionKey) {
//   throw new Error('Environment variable for your subscription key is not set.')
// }

const subscriptionKey = '9a4eb08f2afa27b0b476e8564561ddae'

function MicrosoftTranslator () {}

MicrosoftTranslator.prototype.translate = function (strings, targetLang) {
  console.log(`Microsoft: translating ${strings.length} strings to ${targetLang}...`)

  let options = {
    method: 'POST',
    baseUrl: 'https://api.cognitive.microsofttranslator.com/',
    url: 'translate',
    qs: {
      'api-version': '3.0',
      to: targetLang
    },
    headers: {
      '965f0f8db5bf6f4b746ed2f0c02f00f2': subscriptionKey,
      'Content-type': 'application/json',
      'X-ClientTraceId': uuidv4().toString()
    },
    body: strings.map(str => ({ text: str })),
    json: true
  }

  return new Promise((resolve, reject) => {
    request(options, (err, res, body) => {
      if (err) {
        reject(err)
        return
      }

      if (body.error) {
        console.log('body', body)
        if (body.error.code === 400077) {
          reject(new LimitReachedError('Microsoft', 'Maximum request size'))
        } else if (body.error.code === 403001) {
          reject(new LimitReachedError('Microsoft', 'Quota per hour'))
        } else {
          reject(new Error(body.error.message))
        }
      } else {
        let translations = body
          .reduce((accum, item) => accum.concat(item.translations), [])
          .map(i => i.text)
        resolve(translations)
      }
    })
  }).then(translations => {
    console.log(`Microsoft: Translation succeed. Got ${translations.length} translations.`)
    return translations
  })
}

MicrosoftTranslator.prototype.getRequestLimit = function () {
  return REQUEST_CHAR_LIMIT
}

MicrosoftTranslator.prototype.getRequestItemsCountLimit = function () {
  return ITEMS_IN_REQUEST_LIMIT
}

MicrosoftTranslator.prototype.getMaxLimit = function () {
  return CHAR_PER_HOUR_LIMIT
}

module.exports = MicrosoftTranslator

// new MicrosoftTranslator()
//   .translate([(new Array(5001)).join('a'), 'b'], 'ru')
//   .then(translations => console.log('Result', translations))
//   .catch(err => console.error(err))

/*
* Limits: https://docs.microsoft.com/en-us/azure/cognitive-services/translator/request-limits
* https://docs.microsoft.com/en-us/azure/cognitive-services/translator/reference/v3-0-translate?tabs=curl
* */
