package PlagiarismDetection;

/*
 *  CSIT214/814 GROUP ALPHA
 */

import java.io.*;
import java.net.*;
import java.util.*;
import javax.net.ssl.HttpsURLConnection;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonElement;
import com.google.gson.JsonParser;

public class MicrosoftTextTranslate {
    final private static String SUBSCRIPTIONKEY = "e64e85eaef5582b7e9b692340b179014";
    final private static String HOST = "https://api.cognitive.microsofttranslator.com";
    final private static String PATH = "/translate?api-version=3.0";
    public static String params;
    public String translatedText = "";

    public static class RequestBody {
        String Text;
        public RequestBody(String text) {
            this.Text = text;
        }
    }

    /*
     *  This function was not written by the team. Source: https://docs.microsoft.com/en-us/azure/cognitive-services/translator/quickstart-java-translate
     */
    public String Post (URL url, String content) throws Exception {
        HttpsURLConnection connection = (HttpsURLConnection) url.openConnection();
        connection.setRequestMethod("POST");
        connection.setRequestProperty("Content-Type", "application/json");
        connection.setRequestProperty("Content-Length", content.length() + "");
        connection.setRequestProperty("0978ca5e2350c1994e244bf75f3ab6cc", SUBSCRIPTIONKEY);
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

    /*
     *  Most of this was not written by the team, only changed the return type to void.
     *  Source: https://docs.microsoft.com/en-us/azure/cognitive-services/translator/quickstart-java-translate
     *
     *  Sets internal class attribute called translatedText to be the translation of "text" parameter.
     */
    public void translateAString (String text) throws Exception {
        URL url = new URL (HOST + PATH + params);

        List<RequestBody> objList = new ArrayList<>();
        objList.add(new RequestBody(text));
        String content = new Gson().toJson(objList);
        translatedText = Post(url, content);
    }

    /*
     *  This function was not written by the team.
     *  Source: https://docs.microsoft.com/en-us/azure/cognitive-services/translator/quickstart-java-translate
     */
    public String prettify(String json_text) {
        JsonParser parser = new JsonParser();
        JsonElement json = parser.parse(json_text);
        Gson gson = new GsonBuilder().setPrettyPrinting().create();
        return gson.toJson(json);
    }

    /*
     *  Returns this part of json_text:
     *      "text":"[EVERYTHING IN HERE]","to":
     */
    public String getTextFromJson (String json_text) {
        return json_text.substring(json_text.indexOf("\"text\":") + 8, json_text.indexOf("\",\"to\":"));
    }

    /*
     *  Set the params global variable to be used in the next text translation.
     *  Unfortunately, some of these languages can't be used as of right now, because
     *  they're characters can't be handled by the matching algorithm.
     */
    public void setParams (String str) {
        switch (str) {
//            case "Arabic":
//                params = "&to=ar";
//                break;
//            case "Chinese Simplified":
//                params = "&to=zh-Hans";
//                break;
//            case "Chinese Traditional":
//                params = "&to=zh-Hant";
//                break;
            case "English":
                params = "&to=en";
                break;
            case "French":
                params = "&to=fr";
                break;
            case "German":
                params = "&to=de";
                break;
//            case "Hindi":
//                params = "&to=hi";
//                break;
            case "Italian":
                params = "&to=it";
                break;
//            case "Russian":
//                params = "&to=ru";
//                break;
            case "Spanish":
                params = "&to=es";
                break;
            default:
                break;
        }
    }

    /*
     *  All supported languages can be found here: https://api.cognitive.microsofttranslator.com/languages?api-version=3.0
     */
    public static void getAllSupportedLanguages () {
        // do a GET request of the above URL
        // todo
    }
}
