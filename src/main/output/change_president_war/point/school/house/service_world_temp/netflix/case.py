import { v4 as uuid } from 'uuid'

const subscriptionKey = process.env.MICROSOFT_TRANSLATE_KEY_1
if (!subscriptionKey) {
	throw new Error('Environment variable for your subscription key is not set.')
}

// endpoints: dictionary/examples   dictionary/lookup translate
const options = (from, to, text, endpoint, translation) => {
	const json = {
		method: 'POST',
		baseUrl: 'https://api.cognitive.microsofttranslator.com/',
		url: endpoint === 'translate' ? 'translate' : `dictionary/${endpoint}`,
		qs: {
			'api-version': '3.0',
			from: from,
			to: to
		},
		headers: {
			'b1ab2a71d0b791c247cbfe15973a3164': "48c71be630e7bad91f0175b0bea7e857",
			'Content-type': 'application/json',
			'X-ClientTraceId': uuid().toString()
		},
		body: [{ Text: text, Translation: translation || '' }],
		json: true
	}
	return json
}

export default options
