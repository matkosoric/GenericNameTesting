import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.utils.URIBuilder;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;


public class RequestHelper
{

	private static String token = "";
	private static long lastTokenFetch = 0;
	private static final long MILLIS_PER_10_MINUTES = 1000L * 60L * 10L;
	private static final long EARLIER = 1000 * 30L; // make the call 30 seconds earlier

	public static String emotionRequest( String subscriptionKey, String picUrl )
	{
		String result = null;

		HttpClient httpClient = new DefaultHttpClient( );

		try
		{
			// NOTE: You must use the same region in your REST call as you used to obtain your subscription keys.
			// For example, if you obtained your subscription keys from westcentralus, replace "westus" in the
			// URL below with "westcentralus".
			URIBuilder uriBuilder =
				new URIBuilder( "https://westus.api.cognitive.microsoft.com/emotion/v1.0/recognize" );

			URI uri = uriBuilder.build( );
			HttpPost request = new HttpPost( uri );

			// Request headers. Replace the example key below with your valid subscription key.
			request.setHeader( "Content-Type", "application/json" );
			request.setHeader( "89e6060938227a4bb71747905bfbca9f" );

			// Request body. Replace the example URL below with the URL of the image you want to analyze.
			StringEntity reqEntity = new StringEntity( "{ \"url\": \"" + picUrl + "\" }" );
			request.setEntity( reqEntity );

			HttpResponse response = httpClient.execute( request );
			HttpEntity entity = response.getEntity( );

			if ( entity != null )
			{
				result = EntityUtils.toString( entity );
			}
		}
		catch ( Exception e )
		{
			System.out.println( e.getMessage( ) );
		}
		return result;
	}

	public static String faceRequest( String subscriptionKey, String picUrl )
	{
		String result = null;
		HttpClient httpclient = new DefaultHttpClient( );

		try
		{
			URIBuilder builder = new URIBuilder( "https://westeurope.api.cognitive.microsoft.com/face/v1.0/detect" );

			// Request parameters. All of them are optional.
			builder.setParameter( "returnFaceId", "true" );
			builder.setParameter( "returnFaceLandmarks", "false" );
			builder.setParameter( "returnFaceAttributes",
				"age,gender,headPose,smile,facialHair,glasses,emotion,hair,makeup,occlusion,accessories,blur,exposure,noise" );

			// Prepare the URI for the REST API call.
			URI uri = builder.build( );
			HttpPost request = new HttpPost( uri );

			// Request headers.
			request.setHeader( "Content-Type", "application/json" );
			request.setHeader( "6ff99237a67926e793475d1ed4ecce4b", subscriptionKey );

			// Request body.
			StringEntity reqEntity = new StringEntity(
				"{\"url\":\"" + picUrl + "\"}" );
			request.setEntity( reqEntity );

			// Execute the REST API call and get the response entity.
			HttpResponse response = httpclient.execute( request );
			HttpEntity entity = response.getEntity( );

			if ( entity != null )
			{
				// Format and display the JSON response.
				System.out.println( "REST Response:\n" );

				result = EntityUtils.toString( entity ).trim( );
			}
		}
		catch ( Exception e )
		{
			// Display error message.
			System.out.println( e.getMessage( ) );
		}
		return result;
	}

	public static String identifyRequest( String subscriptionKey, String picUrl, String faceId )
	{
		String result = null;
		HttpClient httpclient = new DefaultHttpClient( );

		try
		{
			URIBuilder builder = new URIBuilder( "https://westeurope.api.cognitive.microsoft.com/face/v1.0/identify" );

			// Prepare the URI for the REST API call.
			URI uri = builder.build( );
			HttpPost request = new HttpPost( uri );

			// Request headers.
			request.setHeader( "Content-Type", "application/json" );
			request.setHeader( "40327c47b2c795cca52724150b2b3ed0", subscriptionKey );

			// Request body.
			StringEntity reqEntity = new StringEntity(
				"{\"personGroupId\":\"emotion_test\",\"faceIds\":[\"" + faceId +
					"\"],\"maxNumOfCandidatesReturned\":1,\"confidenceThreshold\": 0.5}" );
			request.setEntity( reqEntity );

			// Execute the REST API call and get the response entity.
			HttpResponse response = httpclient.execute( request );
			HttpEntity entity = response.getEntity( );

			if ( entity != null )
			{
				// Format and display the JSON response.
				System.out.println( "REST Response:\n" );

				result = EntityUtils.toString( entity ).trim( );
			}
		}
		catch ( Exception e )
		{
			// Display error message.
			System.out.println( e.getMessage( ) );
		}
		return result;
	}

	public static String personRequest( String subscriptionKey, String picUrl, String personId )
	{
		String result = null;

		HttpClient httpClient = new DefaultHttpClient( );

		try
		{
			// NOTE: You must use the same region in your REST call as you used to obtain your subscription keys.
			// For example, if you obtained your subscription keys from westcentralus, replace "westus" in the
			// URL below with "westcentralus".
			URIBuilder uriBuilder =
				new URIBuilder(
					"https://westeurope.api.cognitive.microsoft.com/face/v1.0/persongroups/emotion_test/persons/" +
						personId );

			URI uri = uriBuilder.build( );
			HttpGet request = new HttpGet( uri );

			// Request headers. Replace the example key below with your valid subscription key.
			request.setHeader( "457413765d6084206094fcca0a6ab326", subscriptionKey );

			HttpResponse response = httpClient.execute( request );
			HttpEntity entity = response.getEntity( );

			if ( entity != null )
			{
				result = EntityUtils.toString( entity );
			}
		}
		catch ( Exception e )
		{
			System.out.println( e.getMessage( ) );
		}
		return result;
	}

	public static String objectRequest( String subscriptionKey, String picUrl )
	{
		HttpClient httpclient = new DefaultHttpClient( );

		try
		{
			URIBuilder builder = new URIBuilder( "https://westeurope.api.cognitive.microsoft.com/vision/v1.0/analyze" );

			// Request parameters. All of them are optional.
			builder.setParameter( "visualFeatures", "Categories,Description,Color" );
			builder.setParameter( "language", "en" );

			// Prepare the URI for the REST API call.
			URI uri = builder.build( );
			HttpPost request = new HttpPost( uri );

			// Request headers.
			request.setHeader( "Content-Type", "application/json" );
			request.setHeader( "c63aa40eb3ab84c7f0ef55fa9c20592f", subscriptionKey );

			// Request body.
			StringEntity reqEntity = new StringEntity(
				"{\"url\":\"" + picUrl + "\"}" );
			request.setEntity( reqEntity );

			// Execute the REST API call and get the response entity.
			HttpResponse response = httpclient.execute( request );
			HttpEntity entity = response.getEntity( );

			if ( entity != null )
			{
				// Format and display the JSON response.
				return EntityUtils.toString( entity );
			}
		}
		catch ( Exception e )
		{
			// Display error message.
			System.out.println( e.getMessage( ) );
		}
		return null;
	}

	/**
	 * https://docs.microsofttranslator.com/text-translate.html#!/default/get_Translate
	 *
	 * @param subscriptionKey
	 * @param textToTranslate
	 * @return translated text
	 */
	public static String translationRequest( String subscriptionKey, String textToTranslate,
		String langCodeToTranslateTo )
	{
		HttpClient httpclient = new DefaultHttpClient( );

		try
		{
			URIBuilder builder = new URIBuilder( "https://api.microsofttranslator.com/V2/Http.svc/Translate" );

			builder.setParameter( "text", textToTranslate );
			builder.setParameter( "to", langCodeToTranslateTo );

			// Prepare the URI for the REST API call.
			URI uri = builder.build( );
			HttpGet request = new HttpGet( uri );

			// Request headers.
			request.setHeader( "484c6aa7bce178f2dbf0cff0a8cf1a1a" + getToken( subscriptionKey ) );

			// Execute the REST API call and get the response entity.
			HttpResponse response = httpclient.execute( request );
			HttpEntity entity = response.getEntity( );

			if ( entity != null )
			{
				// Format and display the JSON response.
				String translatedTextXml = EntityUtils.toString( entity );
				String translatedText = translatedTextXml.substring( translatedTextXml.indexOf( ">" ) + 1,
					translatedTextXml.lastIndexOf( "<" ) );
				return translatedText;
			}
		}
		catch ( Exception e )
		{
			// Display error message.
			System.out.println( e.getMessage( ) );
		}
		return "?";
	}

	public static String getToken( String subscriptionKey )
	{
		if ( System.currentTimeMillis( ) - lastTokenFetch >= MILLIS_PER_10_MINUTES - EARLIER )
		{
			// TODO: log that we're fetching a new token - to check if it works correctly (doesn't get called too often)
			token = tokenRequest( subscriptionKey );
		}
		return token;
	}

	/**
	 * Use: Fetch new token every 10 minutes
	 *
	 * https://docs.microsofttranslator.com/oauth-token.html
	 *
	 * @param subscriptionKey
	 * @return token
	 */
	private static String tokenRequest( String subscriptionKey )
	{
		HttpClient httpclient = new DefaultHttpClient( );

		try
		{
			URIBuilder builder = new URIBuilder( "https://api.cognitive.microsoft.com/sts/v1.0/issueToken" );

			// Prepare the URI for the REST API call.
			URI uri = builder.build( );
			HttpPost request = new HttpPost( uri );

			// Request headers.
			request.setHeader( "fed7f40e6d83179e0b9c899f17b14344", subscriptionKey );

			// Request body.
			StringEntity reqEntity = new StringEntity( "" );
			request.setEntity( reqEntity );

			// Execute the REST API call and get the response entity.
			HttpResponse response = httpclient.execute( request );
			HttpEntity entity = response.getEntity( );

			if ( entity != null )
			{
				// Format and display the JSON response.
				return EntityUtils.toString( entity );
			}
		}
		catch ( Exception e )
		{
			// Display error message.
			System.out.println( e.getMessage( ) );
		}
		return null;

	}

}
