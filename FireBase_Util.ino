bool initFirebase()
{
  config.api_key = FB_APIKEY;
  auth.user.email = FB_USER_EMAIL;
  auth.user.password = FB_USER_PASSW;
  config.token_status_callback = firestoreTokenStatuscallback;
  Firebase.begin(&config, &auth);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  return true;
}

//void create

void firestoreUpdate(float t, float h, double l, int m)
{
  char buf[16];
  //long lval;
  String macadd = WiFi.macAddress();
  ltoa(epochTime(),buf,10);
  String Docpath = macadd + "/" + buf;
  //Serial.println(Docpath);

  FirebaseJson content;
  content.set("fields/mac/stringValue", macadd);
  //content.set("fields/timestamp/timestampValue", buf);
  content.set("fields/temperature/doubleValue", t);
  content.set("fields/moisture/integerValue", m);
  content.set("fields/humidity/doubleValue", h);
  content.set("fields/lux/doubleValue", l);
   
  if (Firebase.Firestore.createDocument(&fbdo, FB_PROJECT_ID,"", Docpath.c_str(), content.raw()))
  {
    Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
  }
  else
  {
    Serial.println(fbdo.errorReason());
  }
}

void firestoreTokenStatuscallback(TokenInfo info)
{
  Serial.printf("Token Info: type = %s, status = %s\n", getTokenType(info), getTokenStatus(info));
}
