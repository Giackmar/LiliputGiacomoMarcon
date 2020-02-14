char caratteri[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.'};
String codiceCaratteri[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".-.-.-"};
int LED = 13;



//----------------------METODI LISTA INIZIO--------------------------------------------------------------------------------------

typedef struct nodo { //creo il tipo Lista [carattere-->char & next-->puntatore a nodo]
  char carattere;
  struct nodo* next;
} Lista;

Lista* lista = NULL; //creo una variabile lista di tipo Lista

Lista* inserisciInCoda(Lista* n, Lista* l) { //restituisce la lista l data in input, dopo aver aggiunto il nodo n alla fine della lista
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

Lista* creaNodo(char c) { //restituisce un nodo con variabile carattere = c (dato in input)
  Lista* n = (Lista*)malloc(sizeof(Lista));
  n->carattere = c;
  n->next = NULL;
  return n;
}

void stampaLista(Lista* l) { //data in input una lista stampa il contenuto su seriale
  if (l != NULL)
  {
    Serial.print(l->carattere);
    stampaLista(l->next);
  }
  l->carattere = NULL;
}

//----------------------METODI LISTA FINE----------------------------------------------------------------------------------------



//------------------METODI TRADUZIONE CHAR TO MORSE INIZIO-----------------------------------------------------------------------

bool charIsOK(char carattere) { //restituisce true se il charater in input è accettabile (se è presente nell'array caratteri)
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

String charToMorse(char carattere) { //restituisce il codice morse del carattere dato in input
  int i = 0;
  while (carattere != caratteri[i])
  {
    i++;
  }
  return codiceCaratteri[i];
}

String controlloTesto(String input) { //ritorno una stringa con caratteri "accettabili" (A,B,C,D,..) tutti maiuscoli
  input.toUpperCase();
  String testo = "";
  bool noPunto = true;

  //estraggo da input solo i caratteri accettabili e precendenti il punto
  for (int i = 0; i < input.length(); i++)
  {
    if (charIsOK(input[i]) && noPunto)
    {
      testo += input[i];
    }
    if (input[i] == '.')
    {
      noPunto = false;
    }
  }
  return testo;
}

//------------------METODI TRADUZIONE CHAR TO MORSE FINE--------------------------------------------------------------------------



//----------------------------METODI INVIO INIZIO---------------------------------------------------------------------------------

void trasmissione(String input) { //ottengo il testo che posso inviare e quindi lo invio
  String testo = controlloTesto(input);
  invioTesto(testo);
}

void invioTesto(String testoDaInviare) { //spedisco in codice morse il testo ricevuto in input
  if (testoDaInviare.length() > 0)
  {
    for (int i = 0; i < testoDaInviare.length(); i++) //per ogni carattere da inviare:
    {
      SendChar(testoDaInviare[i]);                                         //lo invio
      lista = inserisciInCoda(creaNodo(testoDaInviare[i]), lista);         //lo inserisco nella lista
    }

    //una volta spedito tutto il testo spedisco il punto(END-CHAR) e lo aggiungo alla lista
    SendChar('.');
    lista = inserisciInCoda(creaNodo('.'), lista);

    //stampo su seriale la lista
    Serial.print("Hai inviato: ");
    stampaLista(lista);
    Serial.println();
  }
}

void SendChar(char carattere) { //invia il codice morse del carattere ricevuto in input con trasmissione luminosa
  String codeMorse = charToMorse(carattere);
  for (int i = 0; i < codeMorse.length(); i++)
  {
    if (codeMorse[i] == '.') //punto --> 0.5s ON
    {
      digitalWrite(LED, HIGH);
      delay(500);
    }
    else if (codeMorse[i] == '-') //linea --> 1.0s ON
    {
      digitalWrite(LED, HIGH);
      delay(1000);
    }

    digitalWrite(LED, LOW);
    delay(200);//attendo
  }

  //fine-carattere --> 2.0s ON
  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);
  delay(200);//attendo
}

//----------------------------METODI INVIO FINE-----------------------------------------------------------------------------------



void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  Serial.println("Digitare la frase da trasmettere e quindi inviare");
}

void loop() {
  if (Serial.available() > 0) //attendo che venga dato un input da seriale
  {
    trasmissione(Serial.readString());
  }
}
