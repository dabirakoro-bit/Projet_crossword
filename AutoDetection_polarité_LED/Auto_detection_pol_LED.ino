const byte LED_TEST_1 = 2;   // Broche D2
const byte LED_TEST_2 = 3;   // Broche D3
const byte MESURE = A0;      // Mesure analogique

const int LIMITE_RESISTANCE = 800;       // cette valeur dépend de la valeur de la résistance de mesure 
// dans notre cas on avait une résistance de 1Kohm la valeur était à 464 donc on a pris 800 comme seuil au dessus des 400 

const int LIMITE_LED = 800; // la valeur dépend de la tension aux borne de la led quand elle est passante il était à 564 (2.2v)
void setup()
{
  Serial.begin(115200);

  pinMode(LED_TEST_1, OUTPUT);
  pinMode(LED_TEST_2, OUTPUT);

  delay(1000);

  Serial.println("===== Detection automatique polarite LED =====");

  int valeur1;
  int valeur2;


  // ===================================
  // Test 1 : D2 = HIGH, D3 = LOW
  // ===================================

  digitalWrite(LED_TEST_1, HIGH);
  digitalWrite(LED_TEST_2, LOW);

  delay(20);

  valeur1 = analogRead(MESURE);

  Serial.print("Test 1 (D2 HIGH -> D3 LOW) : ");
  Serial.println(valeur1);


  // Petite pause
  delay(100);


  // ===================================
  // Test 2 : D2 = LOW, D3 = HIGH
  // ===================================

  digitalWrite(LED_TEST_1, LOW);
  digitalWrite(LED_TEST_2, HIGH);

  delay(20);

  valeur2 = analogRead(MESURE);

  Serial.print("Test 2 (D3 HIGH -> D2 LOW) : ");
  Serial.println(valeur2);



  // ===================================
  // Decision
  // ===================================

  Serial.println("-----------------------------");


  if (valeur1 > 1000) // le mille c'est pour les 5 v qui est à 1023 
  {  
      if (valeur2 < LIMITE_RESISTANCE && valeur2 != 0 )
      {
        Serial.println("LED detectee !");
        Serial.println("Anode : broche D3");
        Serial.println("Cathode : broche D2");

         // On coupe les sorties apres le test
        digitalWrite(LED_TEST_1, LOW);
        digitalWrite(LED_TEST_2, LOW);
        return ; }
  }
    
 if (valeur1 < LIMITE_LED)
  {  
      if (valeur2 < 10) // pour verifier que l'on est à zero 
      {
        Serial.println("LED detectee !");
        Serial.println("Anode : broche D2");
        Serial.println("Cathode : broche D3");

         // On coupe les sorties apres le test
        digitalWrite(LED_TEST_1, LOW);
        digitalWrite(LED_TEST_2, LOW);
        return ; }
  }

  
    Serial.println("Aucune LED detectee ou mesure insuffisante.");
    digitalWrite(LED_TEST_1, LOW);
    digitalWrite(LED_TEST_2, LOW);

}


void loop()
{
  // Rien ici : test effectue une seule fois
}