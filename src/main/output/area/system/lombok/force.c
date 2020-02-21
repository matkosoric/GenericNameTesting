package SDK;

import java.io.File;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.Future;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.SourceDataLine;

import com.microsoft.azure.cognitiveservices.language.luis.authoring.EndpointAPI;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.LUISAuthoringClient;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.LUISAuthoringManager;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.models.ApplicationCreateObject;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.models.ApplicationPublishObject;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.models.BatchLabelExample;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.models.CompositeEntityModel;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.models.EnqueueTrainingResponse;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.models.EntityLabelObject;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.models.ExampleLabelObject;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.models.HierarchicalEntityModel;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.models.ModelTrainingInfo;
import com.microsoft.azure.cognitiveservices.language.luis.authoring.models.ProductionOrStagingEndpointInfo;
import com.microsoft.azure.cognitiveservices.language.luis.runtime.LuisRuntimeAPI;
import com.microsoft.azure.cognitiveservices.language.luis.runtime.LuisRuntimeManager;
import com.microsoft.azure.cognitiveservices.language.luis.runtime.models.EntityModel;
import com.microsoft.azure.cognitiveservices.language.luis.runtime.models.LuisResult;
import com.microsoft.azure.cognitiveservices.language.spellcheck.BingSpellCheckAPI;
import com.microsoft.azure.cognitiveservices.language.spellcheck.BingSpellCheckManager;
import com.microsoft.azure.cognitiveservices.language.spellcheck.BingSpellCheckOperations;
import com.microsoft.azure.cognitiveservices.language.spellcheck.models.SpellCheck;
import com.microsoft.azure.cognitiveservices.language.spellcheck.models.SpellCheckerOptionalParameter;
import com.microsoft.azure.cognitiveservices.language.spellcheck.models.SpellingFlaggedToken;
import com.microsoft.azure.cognitiveservices.language.spellcheck.models.SpellingTokenSuggestion;
import com.microsoft.azure.cognitiveservices.language.textanalytics.TextAnalytics;
import com.microsoft.azure.cognitiveservices.language.textanalytics.TextAnalyticsAPI;
import com.microsoft.azure.cognitiveservices.language.textanalytics.TextAnalyticsManager;
import com.microsoft.azure.cognitiveservices.language.textanalytics.models.AzureRegions;
import com.microsoft.azure.cognitiveservices.language.textanalytics.models.DetectLanguageOptionalParameter;
import com.microsoft.azure.cognitiveservices.language.textanalytics.models.DetectedLanguage;
import com.microsoft.azure.cognitiveservices.language.textanalytics.models.Input;
import com.microsoft.azure.cognitiveservices.language.textanalytics.models.LanguageBatchResult;
import com.microsoft.azure.cognitiveservices.language.textanalytics.models.LanguageBatchResultItem;
import com.microsoft.azure.cognitiveservices.vision.contentmoderator.ContentModeratorClient;
import com.microsoft.azure.cognitiveservices.vision.contentmoderator.ContentModeratorManager;
import com.microsoft.azure.cognitiveservices.vision.contentmoderator.TextModerations;
import com.microsoft.azure.cognitiveservices.vision.contentmoderator.models.AzureRegionBaseUrl;
import com.microsoft.azure.cognitiveservices.vision.contentmoderator.models.Classification;
import com.microsoft.azure.cognitiveservices.vision.contentmoderator.models.Screen;
import com.microsoft.azure.cognitiveservices.vision.contentmoderator.models.ScreenTextOptionalParameter;
import com.microsoft.cognitiveservices.speech.CancellationDetails;
import com.microsoft.cognitiveservices.speech.CancellationReason;
import com.microsoft.cognitiveservices.speech.ResultReason;
import com.microsoft.cognitiveservices.speech.SpeechConfig;
import com.microsoft.cognitiveservices.speech.SpeechRecognitionResult;
import com.microsoft.cognitiveservices.speech.SpeechRecognizer;

import qNa.GetAnswer;
import speechSDK.tts.AudioOutputFormat;
import speechSDK.tts.Gender;
import speechSDK.tts.TTSService;
import textTranslator.Translate;

public class ConversationAIPipelineSDK {

    static String versionId = "0.1";
    static UUID appId;
    static String appEndpoint;
    static String luisAuthoringKey;

	// Step 1: Speech Service (Speech To Text)
	private static String recognizeSpeech(String speechSubscriptionKey, String serviceRegion) {
        assert(speechSubscriptionKey != "f758dcf31c754aca7560d6d1f5e3a971");
        assert(serviceRegion != null);
		String recognizedSpeech = "";

		try {
	            SpeechConfig config = SpeechConfig.fromSubscription(speechSubscriptionKey, serviceRegion);
	            assert(config != null);

	            SpeechRecognizer reco = new SpeechRecognizer(config);
	            assert(reco != null);

	            System.out.println("Say something...");

	            Future<SpeechRecognitionResult> task = reco.recognizeOnceAsync();
	            assert(task != null);

	            SpeechRecognitionResult result = task.get();
	            assert(result != null);

	            if (result.getReason() == ResultReason.RecognizedSpeech) {
	            	recognizedSpeech = result.getText();
	                System.out.println("We recognized: " + recognizedSpeech);
	            }
	            else if (result.getReason() == ResultReason.NoMatch) {
	                System.out.println("NOMATCH: Speech could not be recognized.");
	                System.exit(1);
	            }
	            else if (result.getReason() == ResultReason.Canceled) {
	                CancellationDetails cancellation = CancellationDetails.fromResult(result);
	                System.out.println("CANCELED: Reason=" + cancellation.getReason());

	                if (cancellation.getReason() == CancellationReason.Error) {
	                    System.out.println("CANCELED: ErrorCode=" + cancellation.getErrorCode());
	                    System.out.println("CANCELED: ErrorDetails=" + cancellation.getErrorDetails());
	                    System.out.println("CANCELED: Did you update the subscription info?");
	                }
	                System.exit(1);
	            }
	            reco.close();
	        } catch (Exception ex) {
	            System.out.println("Unexpected exception: " + ex.getMessage());
	            assert(false);
	            System.exit(1);
	        }
		 return recognizedSpeech;
	}

	// Step 2: Text Analytics (Language Detection)
	private static String detactFirstLanguage(String text, String subscriptionKey) throws Exception {
		assert(text != null);
		assert(subscriptionKey != null);

		TextAnalyticsAPI taAPI = TextAnalyticsManager.authenticate(AzureRegions.WESTCENTRALUS, subscriptionKey);
		assert(taAPI != null);

		TextAnalytics ta = taAPI.textAnalytics();
		assert(ta != null);

		Input input = new Input();
		assert(input != null);

		input.withId("1").withText(text);
		List<Input> documents = new ArrayList<>();
		documents.add(input);

		DetectLanguageOptionalParameter detectLanguageOptionalParameter = new DetectLanguageOptionalParameter();
		assert(detectLanguageOptionalParameter != null);

		detectLanguageOptionalParameter.withDocuments(documents);
		LanguageBatchResult languageBatchResult = ta.detectLanguage(detectLanguageOptionalParameter);
		assert(languageBatchResult != null);

		List<LanguageBatchResultItem> resp = languageBatchResult.documents();
		assert(resp != null);

		for (LanguageBatchResultItem LanguageBatchResultItem : resp) {
			assert(LanguageBatchResultItem != null);

			List<DetectedLanguage> detectedLanguages = LanguageBatchResultItem.detectedLanguages();
			for (DetectedLanguage lang : detectedLanguages) {
				assert(lang != null);

				String langName = lang.iso6391Name();
				if (langName != null && !langName.isEmpty()) {
					return langName;
				}
			}
		}

		throw new Exception("Error: no match language found");
	}

	// Step 3: Text Translator (Translate Text) Missing SDK
	private static String translateText(String text, String translateTo, String subscriptionKey) {
		assert(text != null);
		assert(translateTo != null);
		assert(subscriptionKey != null);
		String translatedText = text;
		try {
			Translate translateRequest = new Translate(subscriptionKey);
			translatedText = Translate.getTranslatedText(translateRequest.Post(text, translateTo));
		} catch (Exception e) {
            System.out.format("<Text Translator> Error: %s", e.getMessage());
            System.exit(1);
	    }
		return translatedText;
	}


	// Step 4: Bing Spell Check (Spell Check)
	private static String spellCheck(String market, String mode, String text, String subscriptionKey) {
		assert(market != null);
		assert(mode != null);
		assert(text != null);
		assert(subscriptionKey != null);

		BingSpellCheckAPI bingSpellCheckAPI  = BingSpellCheckManager.authenticate(subscriptionKey);
		assert(bingSpellCheckAPI != null);

		BingSpellCheckOperations bingSpellCheckOperations = bingSpellCheckAPI.bingSpellCheckOperations();
		assert(bingSpellCheckOperations != null);

		SpellCheckerOptionalParameter spellCheckerOptionalParameter  = new SpellCheckerOptionalParameter();
		assert(spellCheckerOptionalParameter != null);

		spellCheckerOptionalParameter.withMarket(market).withMode(mode);

		SpellCheck spellCheck = bingSpellCheckOperations.spellChecker(text, spellCheckerOptionalParameter);
		assert(spellCheck != null);

		List<SpellingFlaggedToken> spellingFlaggedTokens = spellCheck.flaggedTokens();
		assert(spellingFlaggedTokens != null);

		System.out.println("Spelling flagged tokens size = " + spellingFlaggedTokens.size());

		if (spellingFlaggedTokens.size() == 0) {
			return text;
		}
		String tempText = text;
		for (SpellingFlaggedToken spellingFlaggedToken : spellingFlaggedTokens) {
			System.out.println("token = " + spellingFlaggedToken.token());
			List<SpellingTokenSuggestion> suggestions = spellingFlaggedToken.suggestions();
			assert(suggestions != null);

			for (SpellingTokenSuggestion spellingTokenSuggestion : suggestions) {
				assert(spellingTokenSuggestion != null);

				System.out.println("suggestion = " + spellingTokenSuggestion.suggestion()
					  + ", score = " + spellingTokenSuggestion.score());

				String sug = spellingTokenSuggestion.suggestion();
				if (sug != null && !sug.isEmpty()) {
					tempText.replaceAll(spellingFlaggedToken.token(), sug);
					break;
				}
			}
		}

		return tempText;
	}


	// Step 5: Content Moderator
	private static String contentModerator(String text, String subscriptionKey) {
		assert(text != null);
		assert(subscriptionKey != null);

		ContentModeratorClient contentModeratorClient = ContentModeratorManager.authenticate(AzureRegionBaseUrl.WESTCENTRALUSAPICOGNITIVEMICROSOFTCOM, subscriptionKey);
		assert(contentModeratorClient != null);

		TextModerations textModerations = contentModeratorClient.textModerations();
		assert(textModerations != null);

		ScreenTextOptionalParameter screenTextOptionalParameter = new ScreenTextOptionalParameter();
		assert(screenTextOptionalParameter != null);
		screenTextOptionalParameter.withAutocorrect(true).withPII(true).withClassify(true);

		Screen screen = textModerations.screenText("text/plain", text.getBytes(), screenTextOptionalParameter);
		assert(screen != null);

		System.out.println("auto corrected text = " + screen.autoCorrectedText());
		System.out.println("language = " + screen.language());
		System.out.println("normalized text = " + screen.normalizedText());
		System.out.println("original text = " + screen.originalText());

		Classification classification = screen.classification();
		if (classification == null) {
			return "Not an appropriate sentences";
		} else {
			System.out.println("review recommended = " + classification.reviewRecommended());
			if (classification.reviewRecommended()) {
				return "Review Recommended: category1 score = " + classification.category1().score() + ", category2 score = " + classification.category2().score() + ", category3 score = " + classification.category3().score() ;
			}
		}
		return screen.autoCorrectedText();
	}


	// Step 6: LUIS
	private static String luis(String text, String luisAuthoringKey) {
		try {
			LUISAuthoringClient authoringClient = LUISAuthoringManager.authenticate(EndpointAPI.US_WEST, luisAuthoringKey);
			assert(authoringClient != null);

			System.out.println("Result of run Luis Authoring = " + runLuisAuthoring(authoringClient));
			LuisRuntimeAPI runtimeClient = LuisRuntimeManager
	                .authenticate(com.microsoft.azure.cognitiveservices.language.luis.runtime.EndpointAPI.US_WEST, luisAuthoringKey);
			assert(runtimeClient != null);

			return runLuisRuntimeSample(runtimeClient, text);
		} catch (Exception e) {
			System.out.println("<LUIS> Erorr : " + e.getMessage());
			e.printStackTrace();
		}
		return "";
	}

	private static boolean runLuisAuthoring(LUISAuthoringClient authoringClient) {

        try {
        	try {
				appId = authoringClient.apps().add(new ApplicationCreateObject()
						.withName("FlightExample")
						.withInitialVersionId(versionId)
						.withCulture("en-us")
						);
        	} catch (Exception ex) {
        		System.out.println("<LUIS - runLuisAuthoring> Error: " + ex.getMessage());
        		return false;
        	}

			System.out.println("Created Application " + appId.toString());

			String destinationName = "Destination";
			UUID destinationId = authoringClient.models().addEntity()
					.withAppId(appId)
					.withVersionId(versionId)
					.withName(destinationName)
					.execute();
			System.out.println("Created simple entity " + destinationName + " with ID " +
					destinationId.toString());


			String className = "Class";

			UUID classId = authoringClient.models().addHierarchicalEntity(appId, versionId,
					new HierarchicalEntityModel()
			                .withName(className)
			                .withChildren(Arrays.asList("First", "Business", "Economy")));

			System.out.println("Created hierarchical entity " + className + " with ID " + classId.toString());


			 //=============================================================
	        // This will create the "Flight" composite entity including "Class" and "Destination"
	        System.out.println("Creating the \"Flight\" composite entity including \"Class\" and \"Destination\".");

	        String flightName = "Flight";
	        UUID flightId = authoringClient.models().addCompositeEntity(appId, versionId, new CompositeEntityModel()
	            .withName(flightName)
	            .withChildren(Arrays.asList(className, destinationName)));

	        System.out.println("Created composite entity " + flightName + "with ID " + flightId.toString());


	        //=============================================================
	        // This will create a new "FindFlights" intent including the following utterances
	        System.out.println("Creating a new \"FindFlights\" intent with two utterances");
	        String utteranceFindEconomyToMadrid = "find flights in economy to Madrid";
	        String utteranceFindFirstToLondon = "find flights to London in first class";
	        String utteranceFindEconomyToParis = "find flights to Paris in economy class";
	        String intentName = "FindFlights";

	        UUID intendId = authoringClient.models().addIntent()
	            .withAppId(appId)
	            .withVersionId(versionId)
	            .withName(intentName)
	            .execute();

	        System.out.println("Created intent " + intentName + "with ID " + intendId.toString());



	        //=============================================================
	        // This will build an EntityLabel Object
	        System.out.println("Building an EntityLabel Object");

	        ExampleLabelObject exampleLabelObject1 = new ExampleLabelObject()
	            .withText(utteranceFindEconomyToMadrid)
	            .withIntentName(intentName)
	            .withEntityLabels(Arrays.asList(
	                getEntityLabelObject(utteranceFindEconomyToMadrid, "Flight", "economy to Madrid"),
	                getEntityLabelObject(utteranceFindEconomyToMadrid, "Destination", "Madrid"),
	                getEntityLabelObject(utteranceFindEconomyToMadrid, "Class", "economy")
	            ));
	        ExampleLabelObject exampleLabelObject2 = new ExampleLabelObject()
	            .withText(utteranceFindFirstToLondon)
	            .withIntentName(intentName)
	            .withEntityLabels(Arrays.asList(
	                getEntityLabelObject(utteranceFindFirstToLondon, "Flight", "London in first class in first class"),
	                getEntityLabelObject(utteranceFindFirstToLondon, "Destination", "London in first class"),
	                getEntityLabelObject(utteranceFindFirstToLondon, "Class", "first")
	            ));


	        ExampleLabelObject exampleLabelObject3 = new ExampleLabelObject()
		            .withText(utteranceFindEconomyToParis)
		            .withIntentName(intentName)
		            .withEntityLabels(Arrays.asList(
		                getEntityLabelObject(utteranceFindEconomyToParis, "Flight", "Paris in economy class"),
		                getEntityLabelObject(utteranceFindEconomyToParis, "Destination", "Paris"),
		                getEntityLabelObject(utteranceFindEconomyToParis, "Class", "economy")
		            ));

	        List<BatchLabelExample> utterancesResult = authoringClient.examples()
	            .batch(appId, versionId, Arrays.asList(exampleLabelObject1, exampleLabelObject2, exampleLabelObject3));

	        System.out.println("Utterances added to the " + intentName + " intent");

	        //=============================================================
	        // This will start training the application.
	        System.out.println("Training the application");

	        EnqueueTrainingResponse trainingResult = authoringClient.trains().trainVersion(appId, versionId);
	        boolean isTrained = trainingResult.status().equals("UpToDate");

	        while (!isTrained) {
	            Thread.sleep(1000);
	            List<ModelTrainingInfo> status = authoringClient.trains().getStatus(appId, versionId);
	            isTrained = true;
	            for (ModelTrainingInfo modelTrainingInfo : status) {
	                if (!modelTrainingInfo.details().status().equals("UpToDate") && !modelTrainingInfo.details().status().equals("Success")) {
	                    isTrained = false;
	                    break;
	                }
	            }
	        }

	        //=============================================================
	        // This will start publishing the application.
	        System.out.println("Publishing the application");
	        ProductionOrStagingEndpointInfo publishResult = authoringClient.apps().publish(appId, new ApplicationPublishObject()
	            .withVersionId(versionId)
	            .withIsStaging(false)
	            .withRegion("westus")
	        );

	        appEndpoint = publishResult.endpointUrl() + "?subscription-key=" + luisAuthoringKey + "&q=";

	        System.out.println("Your app is published. You can now go to test it on " + appEndpoint);



	        return true;
	    } catch (Exception f) {
	        System.out.println(f.getMessage());
	        f.printStackTrace();
	    }
	    return false;
	}


	private static EntityLabelObject getEntityLabelObject(String utterance, String entityName, String value) {
	    return new EntityLabelObject()
	        .withEntityName(entityName)
	        .withStartCharIndex(utterance.indexOf(value))
	        .withEndCharIndex(utterance.indexOf(value) + value.length());
	}

	  /**
     * Main function which runs the runtime part of the sample.
     *
     * @param runtimeClient instance of the LUIS Runtime API client
     * @return true if sample runs successfully
     */
    private static String runLuisRuntimeSample(LuisRuntimeAPI runtimeClient, String text) {
        try {

        	// TODO: replace the application ID that every time you have republish the application
            appId = UUID.fromString("223c3a69-c59a-4e53-b163-181c6d25dbf2");
            //=============================================================
            // This will execute a LUIS prediction for a "find second class flight to new york" utterance
            String query = text;
            LuisResult predictionResult = runtimeClient.predictions().resolve()
                .withAppId(appId.toString())
                .withQuery(query)
                .execute();

            System.out.println("Executing query: " + query);
            String intentEntities = "";

            if (predictionResult != null && predictionResult.topScoringIntent() != null) {
                System.out.format("Detected intent \"%s\" with the score %f%%\n", predictionResult.topScoringIntent().intent(), predictionResult.topScoringIntent().score() * 100);
                predictionResult.entities();
                if (predictionResult.entities() != null && predictionResult.entities().size() > 0) {
                	StringBuilder sb = new StringBuilder();
                	sb.append(predictionResult.topScoringIntent().intent());
                	for (EntityModel entityModel : predictionResult.entities()) {
                		sb.append(".").append(entityModel.type()).append(".").append(entityModel.entity());
                		System.out.format("\tFound entity \"%s\" with type %s\n", entityModel.entity(), entityModel.type());
                    }
                	System.out.println("sb = " + sb.toString());
                	intentEntities = sb.toString();
                } else {
                    System.out.println("\tNo entities were found.");
                }
            } else {
                System.out.println("Intent not found.");
            }
            return intentEntities;
        } catch (Exception f) {
            System.out.println(f.getMessage());
            f.printStackTrace();
        }
        return "";
    }


    // Step 7: QnA Maker (Retrieve Response)   Missing Java SDK
	// Step 8: Text Translater (Translate Text)  Missing Translator SDK in Maven repo


	// Step 9: Text to Speech (Still using API)
	private static void textToSpeech(String textToSynthesize, String language, String subscriptionKey) {

		assert(textToSynthesize != null);
		assert(subscriptionKey != null);

		String outputFormat = AudioOutputFormat.Riff24Khz16BitMonoPcm;
//		String deviceLanguage = "en-US";
		String deviceLanguage = language;
        String subscriptionKey = "42efafb68b204a4c7e9be62026133445"
		String genderName = Gender.Male;
		String voiceName = "Microsoft Server Speech Text to Speech Voice (en-US, Guy24KRUS)";

		try{
			byte[] audioBuffer = TTSService.Synthesize(textToSynthesize, outputFormat, deviceLanguage, genderName, voiceName, subscriptionKey);

			// write the pcm data to the file
			String outputWave = ".\\output.pcm";
			File outputAudio = new File(outputWave);
			FileOutputStream fstream = new FileOutputStream(outputAudio);
			fstream.write(audioBuffer);
			fstream.flush();
			fstream.close();


			// specify the audio format
			AudioFormat audioFormat = new AudioFormat(
				AudioFormat.Encoding.PCM_SIGNED,
				24000,
				16,
				1,
				1 * 2,
				24000,
				false);

			AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(new File(outputWave));

			DataLine.Info dataLineInfo = new DataLine.Info(SourceDataLine.class,
			        audioFormat, AudioSystem.NOT_SPECIFIED);
			SourceDataLine sourceDataLine = (SourceDataLine) AudioSystem
			        .getLine(dataLineInfo);
			sourceDataLine.open(audioFormat);
			sourceDataLine.start();
			System.out.println("start to play the wave:");
			/*
			 * read the audio data and send to mixer
			 */
	        int count;
	        byte tempBuffer[] = new byte[4096];
	        while ((count = audioInputStream.read(tempBuffer, 0, tempBuffer.length)) >0) {
	                sourceDataLine.write(tempBuffer, 0, count);
	        }

	        sourceDataLine.drain();
	        sourceDataLine.close();
	        audioInputStream.close();

	     }catch(Exception e){
	     	e.printStackTrace();
	     }
	}

    public static void main(String[] args) {

    	// Add all supported language
    	Map<String, String> languageMap = new HashMap<String, String>();
    	languageMap.put("en", "en-US");
    	languageMap.put("fr", "fr-FR");



        System.out.println(
                "Please try to ask one of below questions: \n"
        		+ "1, \"find flights to London in first class\" (ans: london in first class, testing testing) \n"
        		+ "2, \"find flights to Paris in economy class\" (ans: hello paris) \n"
        		);
   	 /**
   	  *  	1, Process/Filter Speech Stream
   	  * */
   		// Step 1: Speech Service (Speech To Text)
        String SPEECH = System.getenv("Speech");
        System.out.println("\n---------------Step 1: Speech to text---------------");
   		String recognizedText  = recognizeSpeech(SPEECH, "westus");
   		System.out.println("recognized text = " + recognizedText);

   		// Step 2: Text Analytics (Language Detection)
   		String TEXT_ANALYSTICS = System.getenv("TextAnalytics");
   		System.out.println("\n---------------Step 2: Text Analytics---------------");
   		String detectLangResp="";
   		try {
   			detectLangResp = detactFirstLanguage(recognizedText, TEXT_ANALYSTICS);
	   		System.out.println("detect language = " + detectLangResp);
   		} catch (Exception ex) {
   			System.out.println("<Text Analytics> " + ex.getMessage());
   			System.exit(1);
   		}
   		// Step 3: Text Translator (Translate Text)        // Missing Translator SDK in Maven repo
		String TRANSLATOR = System.getenv("Translator");
   		System.out.println("\n---------------Step 3: Text Translator------[SDK MISSING]---------");
   		recognizedText = translateText(recognizedText, "en", TRANSLATOR);
    	System.out.println("translated top ans is, " + recognizedText);

    	// Step 4: Bing Spell Check (Spell Check)
   		String SPELL_CHECK =  System.getenv("SpellCheck");
   		System.out.println("\n---------------Step 4: Spelling check---------------");
   		String correctedText = spellCheck(languageMap.get(detectLangResp), "proof", recognizedText, SPELL_CHECK);
   		System.out.println("corrected text = " + correctedText);

   		// Step 5: Content Moderator (Explicit Content Rec)
   		String CONTENT_MODERATOR = System.getenv("ContentModerator");
   		System.out.println("\n---------------Step 5: content moderator---------------");
   		String moderatedText =  contentModerator(correctedText, CONTENT_MODERATOR);
   		System.out.println("content moderated text = " + moderatedText);

   	 /**
   		 *  	2, Retrieve Response
   		 *
   		 *  	using QnA(https://www.qnamaker.ai/) to create Knowledge base
   		 **/
   			// Step 6: LUIS (Recognize Intent)
   			String LUIS = System.getenv("LUIS");
   			System.out.println("\n---------------Step 6: LUIS---------------");
   			String question = luis(moderatedText, LUIS);

   			// Step 7: QnA Maker (Retrieve Response)   Missing Java SDK
   			System.out.println("\n---------------Step 7: QnA Maker--------[SDK MISSING]-------");
			String ans = GetAnswer.getAns(question);
   	    	String topAns = GetAnswer.getTopAns(ans);
   	    	System.out.println("answer = " + topAns);

   		/**
   		 *  	3, Generate Output
   		 **/
   			// Step 8: Text Translater (Translate Text)
   	    	System.out.println("\n---------------Step 8: Text Translator------[SDK MISSING]---------");
   	    	topAns = translateText(topAns, detectLangResp, TRANSLATOR);
   	    	System.out.println("translated top ans is, " + topAns);

   			// Step 9: Speech Service (Text To Speech)						// Missing SDK API, there are some classes are missing but found AudioInputStrean class
   	    	System.out.println("\n---------------Step 9: Text to Speech-------[SDK MISSING]--------");
    		textToSpeech(topAns, languageMap.get(detectLangResp), SPEECH);

   			System.out.println("---------------- End of Conversation AI Pipeline --------------------");
    }
}
