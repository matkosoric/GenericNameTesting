package com.red.dwarf;

import com.google.gson.*;

import javax.net.ssl.HttpsURLConnection;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

public class Util {
// **********************************************
// *** Update or verify the following values. ***
// **********************************************

    // Replace the subscriptionKey string value with your valid subscription key.
    static String subscriptionKey = "a7a6c319f4fed0a677f5e16765105411";

    static String host = "https://api.cognitive.microsofttranslator.com";
    static String path = "/translate?api-version=3.0";


    public static class RequestBody {
        String Text;

        public RequestBody(String text) {
            this.Text = text;
        }
    }

    public static String Post(URL url, String content) throws Exception {
        HttpsURLConnection connection = (HttpsURLConnection) url.openConnection();
        connection.setRequestMethod("POST");
        connection.setRequestProperty("Content-Type", "application/json");
        connection.setRequestProperty("Content-Length", content.length() + "");
        connection.setRequestProperty("ee5f4fdc05babfe2b877fdd13a15a8bd", subscriptionKey);
        connection.setRequestProperty("X-ClientTraceId", java.util.UUID.randomUUID().toString());
        connection.setDoOutput(true);

        DataOutputStream wr = new DataOutputStream(connection.getOutputStream());
        byte[] encoded_content = content.getBytes("UTF-8");
        wr.write(encoded_content, 0, encoded_content.length);
        wr.flush();
        wr.close();

        StringBuilder response = new StringBuilder ();
        BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream(), "UTF-8"));
        String line;
        while ((line = in.readLine()) != null) {
            response.append(line);
        }
        in.close();

        return response.toString();
    }

    public static String Translate (String text, String from, String to) throws Exception {
        String queryPath = "";
        if(!from.equals("detect")) {
            queryPath += "&from=" + from;
        }
        queryPath += "&to=" + to;

        URL url = new URL (host + path + queryPath);

        List<Util.RequestBody> objList = new ArrayList<>();
        objList.add(new Util.RequestBody(text));
        String content = new Gson().toJson(objList);
        return Post(url, content);
    }

    public static String prettify(String json_text) {
        json_text = json_text.substring(1, json_text.length() - 1);
        JsonParser parser = new JsonParser();
        JsonElement json = parser.parse(json_text);

        Gson gson = new GsonBuilder().setPrettyPrinting().create();
        return gson.toJson(json);
    }

    public static Translation getTranslation(String jsonText) {
        jsonText = jsonText.substring(1, jsonText.length() - 1);
        JsonParser parser = new JsonParser();

        JsonElement json = parser.parse(jsonText);
        JsonObject jsonObject = json.getAsJsonObject();

        JsonObject detectedLanguageObj = jsonObject.getAsJsonObject("detectedLanguage");

        JsonArray tranlationsArrayObj = jsonObject.getAsJsonArray("translations");
        JsonObject translationObj = tranlationsArrayObj.get(0).getAsJsonObject();

        return new Translation(
                (detectedLanguageObj == null ? null : detectedLanguageObj.get("language").getAsString()),
                translationObj.get("text").getAsString());
    }

}
