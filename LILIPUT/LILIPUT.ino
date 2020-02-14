char caratteri[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.'};
String codiceCaratteri[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".-.-.-"};

#define LED 13

//--------------------------------------------------------------------------------------------------
typedef struct nodo
{
  char carattere;
  struct nodo* next;
}Lista;

Lista* lista = NULL;

Lista* inserisciInCoda(Lista* n, Lista* l)
{
  if (l == NULL)
  {
    return n;
  }
  else
  {
    l->next = inserisciInCoda(n, l->next);
    return l;
  }
}

Lista* creaNodo(char c)
{
  Lista* n = (Lista*)malloc(sizeof(Lista));
  n->carattere = c;
  n->next = NULL;
  return n;
}

void creaLista(String testo)
{
  for (int i = 0; i < testo.length(); i++)
  {
    lista = inserisciInCoda(creaNodo(testo[i]), lista);
  }
}

void stampaLista(Lista* l)
{
  if(l!=NULL)
  {
    Serial.print(l->carattere);
    stampaLista(l->next);
  }
  l->carattere = NULL;
}
//--------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}


bool CharIsOK(char carattere)//controlla se il charater in input è accettabile(se è presente nell'array caratteri
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


void SendChar(char carattere)//riceve
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
      for (int i = 0; i < testo.length(); i++)
      {
        SendChar(testo[i]);
        lista = inserisciInCoda(creaNodo(testo[i]), lista);
      }
      SendChar('.');
      testo += ".";
      lista = inserisciInCoda(creaNodo('.'), lista);
      Serial.print("Hai inviato: ");
      stampaLista(lista);
      Serial.println();
    }
  }
}
