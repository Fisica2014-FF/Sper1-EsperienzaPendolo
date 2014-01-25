//============================================================================
// Name        : Misure.cpp
// Author      : Francesco Forcher
// Version     : 3.0
// Description : Programma per analizzare i dati sul pendolo raccolti in laboratorio
//============================================================================

// VERSIONE PER I COMPAGNI DEL GRUPPO!!! NON CONSEGNARE QUESTA!!!!
// Quando scrivo TODO: è perchè con eclipse dopo mi consente di trovare i commenti da togliere più in fretta

/////////////////////////////////////////////////////////////////////////////////////
//Librerie
#include <iostream>//cin e cout
#include <fstream>//FileStream
#include <exception>//Eccezioni
#include <string>
#include <cstdlib>

//#include <algorithm>//Sort?
#include <sstream>//StringStream

/////////////////////////////////////////////////////////////////////////////////////
//le mie classi
#include "mylib/analisiDati.h"//Le mie classi Template per l'analisi dati
#define VERSIONE 3.0

/////////////////////////////////////////////////////////////////////////////////////
//Prototipi
/**legge il file di dati formattato "NomeFile" e analizza i dati ivi contenuti
 * @param: NomeFile, nome del file da aprire
 * @throw: string
 * @return: void
 */
inline void leggiFile(char const * const NomeFile) throw(std::string);

//////////////////////////////////////////////////////////////////////////////////////

int main(int numParam, char* args[]) {
	using namespace std;

	system("clear");
	cout << "\n";
	cout << "Programma per analizzare i dati del pendolo, versione: " << VERSIONE << endl;
	/* Esempio di file di dati:
	 * #FORMATO_D
	 * #NUMDATI_3
	 * 03.1
	 * 2.4
	 * 2.67
	 */
	try {
		//Se il numero di parametri è maggiore o uguale a 1, cioè c'è almeno un argomento, fai un ciclo e analizza tutti i file passati come argomento
		if (numParam > 1){
			for(int i = 1; i < numParam; i++)
				leggiFile(args[i]);
		} else if (numParam == 1){
			//Altrimenti leggi dallo standard input
			cout << "\nNessun argomento utilizzato, leggo da stdin\n\n";
			cout << "Ricordarsi che i dati vanno formattati con le informazioni correttamente. Esempio:\n";
			cout << "#FORMATO_D\n" << "#NUMDATI_3\n" << "03.1\n" << "2.4\n" << "2.67\n\n";
			leggiFile("/dev/fd/0");
		}

	} catch (exception &e) {
		cout << e.what() << endl;
		return -1;
	} catch (string &e) {
		cout << e << endl;
		return -2;
	}
	cout << "\n";
	return 0;
}
/**legge il file di dati formattato "NomeFile" e analizza i dati ivi contenuti
 * @param: NomeFile, nome del file da aprire
 * @throw: string
 * @return: void
 */
inline void leggiFile(char const * const NomeFile) throw(std::string){
	using namespace std;

	fstream FileDati;//FileStream

	cout << "Apro il file di dati: \n\n";
	cout << NomeFile << endl << endl;
	FileDati.open(NomeFile, fstream::in | fstream::out);//Apro il file indicato nell'argomento dato via shell
	if (!FileDati.is_open())
		throw "Errore: file non aperto";

	//Leggi il formato dalla prima riga
	string formato = "";
	{
		// Il file deve iniziare con un riga con il numero dei dati preceduto da #NUMDATI_ e uno spazio, esempio "#NUMDATI_100\n"
		string cancellettoPiuFormato;
		stringstream togliCancelletto;
		getline(FileDati,cancellettoPiuFormato);
		string tst = cancellettoPiuFormato.substr(0,9);
		if (tst != (string)"#FORMATO_")
			throw "Errore: formattazione file errata:\n"+
			(string)"	Atteso #FORMATO_ all'inizio del file";
		//togli i primi 9 caratteri, cioè "#NUMDATI_" (leggi dal decimo in poi, npos significa "fino alla fine della stringa")
		togliCancelletto << cancellettoPiuFormato.substr(9,string::npos);
		togliCancelletto >> formato;
	}

	//Leggi il numero dei dati dalla seconda riga
	long numeroDati=0;
	{
		// Il file deve iniziare con un riga con il numero dei dati preceduto da #NUMDATI_ e uno spazio, esempio "#NUMDATI_100\n"
		string CancellettoPiuNumero;
		stringstream TogliCancelletto;
		getline(FileDati,CancellettoPiuNumero);
		string tst = CancellettoPiuNumero.substr(0,9);
		if (tst != (string)"#NUMDATI_")
			throw "Errore: formattazione file errata:\n"+
			(string)"	Atteso #NUMDATI_n come seconda riga del file";
		//togli i primi 9 caratteri, cioè "#NUMDATI_" (leggi dal decimo in poi, npos significa "fino alla fine della stringa")
		TogliCancelletto << CancellettoPiuNumero.substr(9,string::npos);
		TogliCancelletto >> numeroDati;
	}


	//TODO: Un Vector è un contenitore, una specie di array ridimensionabile automaticamente
	vector<double> dati(numeroDati);// TODO: Array (un Vector in realtà) dei dati.

	//Leggi i dati dal file a seconda del formato indicato all'inizio
	if (formato == "D") {
		// File normale, un double per riga
		// Range-for: PER CIASCUN elemento &dato IN data, fai...
		// Notare come lo abbiamo preso per referenza, così da poterlo modificare
		for (double& dato : dati)
			FileDati >> dato;
	}
	else if (formato == "D,I") {
	// File csv, comma separated values. In questo caso un double e un int separati da una virgola
		int _i;
		char _sep;
		for (double& dato : dati)
			FileDati >> dato >> _sep >> _i;
	}
	else {
		// Lancia un errore se il formato non è uno di questi
		throw "Errore: Formato dati non riconosciuto";
	}


	cout << "Dati letti. Analizzo..." << endl << endl;

	using namespace mions::dataAnalisi;
	//TODO: Costruisci la classe che contiene le stime statistiche dei dati (andatevi a leggere il file analisiDati.h)
	AnalisiSingVarOffline_Lazy<double> AnDat(dati, numeroDati);

	cout << "Media: " << AnDat.getMedia() << endl;
	cout << "Mediana: "<< AnDat.getMediana() << endl;
	cout << "Varianza del campione: " << AnDat.getVarianzaCampione() << endl;
	cout << "Deviazione standard campione: " << AnDat.getDeviazioneStandardCamp() << endl;
	cout << "Varianza della popolazione: " << AnDat.getVarianzaPopolazione() << endl;
	cout << "Deviazione standard popolazione: " << AnDat.getDeviazioneStandardPop() << endl;
	cout << "Errore della media: " << AnDat.getErroreMedia() << endl;
	cout << "Massimo: "<< AnDat.getMax() << endl;
	cout << "Minimo: "<< AnDat.getMin() << endl;

	if (string(NomeFile).find("cronodolo") != string::npos) {
		cout << "RILEVATO FILE CRONODOLO" << endl;
		cout << "Analisi classi di 10 classi di dati" << endl;
		for(int i = 0; i < 10; i++) {
			vector<double> classiDati(dati.begin() + 99 * i, dati.begin() + 99 * (i+1));
			AnalisiSingVarOffline_Lazy<double> AnDatClassi(classiDati, false);
			cout << "Primo valore: " << classiDati[0] << "\n";
			cout << "Ultimo valore: " << classiDati[98] << "\n";
			cout << "Media classe " << i+1 << ": " << AnDatClassi.getMedia() << endl;
		}
	}
}
