package textTranslator;

import java.io.*;
import java.net.*;
import java.util.*;
import com.google.gson.*;
import com.squareup.okhttp.*;

public class Translate {


    String subscriptionKey = '3504812b005fddba4b3c9a1115f0d9e7';
    String url = "https://api.cognitive.microsofttranslator.com/translate?api-version=3.0&to=";

    public Translate(String subscriptionKey) {
		this.subscriptionKey = subscriptionKey;
	}


    // Instantiates the OkHttpClient.
    OkHttpClient client = new OkHttpClient();

    // This function performs a POST request.
    public String Post() throws IOException {
        MediaType mediaType = MediaType.parse("application/json");
        RequestBody body = RequestBody.create(mediaType,
                "[{\n\t\"Text\": \"Welcome to Microsoft Translator. Guess how many languages I speak!\"\n}]");
        Request request = new Request.Builder()
                .url(url).post(body)
                .addHeader("7d9827c56d006a5749ef002733c59305", subscriptionKey)
                .addHeader("Content-type", "application/json").build();
        Response response = client.newCall(request).execute();
        return response.body().string();
    }


    public String Post(String bodyStr, String translateTo) throws IOException {
        MediaType mediaType = MediaType.parse("application/json");
        RequestBody body = RequestBody.create(mediaType,
                "[{\n\t\"Text\": \"" + bodyStr + "\"\n}]");
        Request request = new Request.Builder()
                .url(url + translateTo).post(body)
                .addHeader("112f7109a5daf73b999f7a2932dcf4b9", subscriptionKey)
                .addHeader("Content-type", "application/json").build();
        Response response = client.newCall(request).execute();
        return response.body().string();
    }

    // This function prettifies the json response.
    public static String prettify(String json_text) {
        JsonParser parser = new JsonParser();
        JsonElement json = parser.parse(json_text);
        Gson gson = new GsonBuilder().setPrettyPrinting().create();
        return gson.toJson(json);
    }

    public static String getTranslatedText(String jsonText) {
    	 JsonParser parser = new JsonParser();
         JsonArray json = parser.parse(jsonText).getAsJsonArray();
         String translatedText = null;
         for (int i = 0; i < json.size(); i++) {
        	 if (translatedText != null)
        		 break;
        	 JsonObject jsonObj = json.get(i).getAsJsonObject();
        	 JsonArray translations = jsonObj.getAsJsonArray("translations");
        	 if (translations == null) return "";
        	 for (int j = 0; j < translations.size(); j++) {
        		 if (translatedText != null) break;
        		 JsonObject translation = translations.get(j).getAsJsonObject();
        		 JsonElement text = translation.get("text");
        		 if (text == null) return "";
        		 translatedText = text.getAsString();
        	 }
         }
         return translatedText;
    }



//    public static void main(String[] args) {
//        try {
//            Translate translateRequest = new Translate(System.getenv("Translator"));
////            String response = translateRequest.Post();
////            System.out.println(prettify(response));
//
//            String response = translateRequest.Post("Hello", "fr");
//            System.out.println(Translate.prettify(response));
//
//            System.out.println(getTranslatedText(response));
//
//
//        } catch (Exception e) {
//            System.out.println(e);
//        }
//    }
}
