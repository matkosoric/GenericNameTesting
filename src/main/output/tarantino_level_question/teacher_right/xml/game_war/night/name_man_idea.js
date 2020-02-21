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
			'678e88ab3dac250c73075aeb43b16d20': "bf75d7340ab0cd3bc6749a3e1cd61f22",
			'Content-type': 'application/json',
			'X-ClientTraceId': uuid().toString()
		},
		body: [{ Text: text, Translation: translation || '' }],
		json: true
	}
	return json
}

export default options
