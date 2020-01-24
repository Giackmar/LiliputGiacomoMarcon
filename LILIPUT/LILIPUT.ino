char caratteri[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.'};
String codiceCaratteri[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..","--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".-.-.-"};

#define LED 13

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

bool CharIsOK(char carattere)//controlla se il charater in input è accettabile
{
  bool ok = false;
  for (int i = 0; i < 26; i++)
  {
    if (caratteri[i] == carattere)
    {
      ok = true;
    }
  }
  return ok;
}

String CharToMorse(char carattere)//dato il charater mi restituisce il corrispondente in codice morse
{
  int i = 0;
  while (carattere != caratteri[i])
  {
    i++;
  }
  return codiceCaratteri[i];
}

void SendChar(char carattere)
{
  String codeMorse = CharToMorse(carattere);
  for (int i = 0; i < codeMorse.length(); i++)
  {
    if (codeMorse[i] == '.')
    {
      digitalWrite(LED, HIGH);
      delay(500);
    }
    else if (codeMorse[i] == '-')
    {
      digitalWrite(LED, HIGH);
      delay(1000);
    }

    digitalWrite(LED, LOW);
    delay(200);//attendo
  }

  //termino charater
  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);
  delay(200);//attendo
}

void loop() {
  if (Serial.available() > 0)
  {
    String input = Serial.readString();
    input.toUpperCase();
    String testo = "";
    bool notDot = true;
    for (int i = 0; i < input.length(); i++)
    {
      if (CharIsOK(input[i]) && notDot)
      {
        testo += input[i];
      }
      if (input[i] == '.')
      {
        notDot = false;
      }
    }
    if (testo.length() > 0)
    {
      Serial.print("Hai inviato: ");
      for (int i = 0; i < testo.length(); i++)
      {
        SendChar(testo[i]);
        Serial.print(testo[i]);
      }
      SendChar('.');
      Serial.print(".");
    }
    Serial.println();
  }
}
