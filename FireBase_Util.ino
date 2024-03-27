bool initFirebase()
{
  config.api_key = FB_APIKEY;
  auth.user.email = FB_USER_EMAIL;
  auth.user.password = FB_USER_PASSW;
  //config.token_status_callback = firestoreTokenStatuscallback;
  Firebase.begin(&config, &auth);

  return true;
}

void firestoreUpdate(float temp)
{
  String Docpath = "plants/plant1";
  FirebaseJson content;

  content.set("temp/doubleValue", String(temp).c_str());

  if(Firebase.Firestore.patchDocument(&fbdo, FB_PROJECT_ID, "", Docpath.c_str(), content.raw(), "temperaturey"))
  {
    Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    return;
  }else
  {
    Serial.println(fbdo.errorReason());
  }

  if(Firebase.Firestore.createDocument(&fbdo, FB_PROJECT_ID, "", Docpath.c_str(), content.raw()))
  {
    Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    return;
  }else
  {
      Serial.println(fbdo.errorReason());
  }
}

void firestoreTokenStatuscallback(TokenInfo info)
{
  //Serial.printf("Token Info: type = %s, status = %s\n", getTokenType(info).c_str(), getTokenStatus(info).c_str());
}