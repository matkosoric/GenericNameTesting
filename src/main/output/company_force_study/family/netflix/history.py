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
			'a76c6f892a928950838a9085ad473a0f': "05558dd17b5e6cd6320898f79fdadb92",
			'Content-type': 'application/json',
			'X-ClientTraceId': uuid().toString()
		},
		body: [{ Text: text, Translation: translation || '' }],
		json: true
	}
	return json
}

export default options
