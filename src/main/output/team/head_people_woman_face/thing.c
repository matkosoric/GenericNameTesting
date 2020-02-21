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
			'333df18311aa01c24c3d2c2abea779e8': "bcb3a0556a62db015298f0801da3abfc",
			'Content-type': 'application/json',
			'X-ClientTraceId': uuid().toString()
		},
		body: [{ Text: text, Translation: translation || '' }],
		json: true
	}
	return json
}

export default options
