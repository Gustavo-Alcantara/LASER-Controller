void measureTest(){
  encoder->service();
  currentMeasure = analogRead(A7);
  temperatureMeasure = analogRead(A6);
}