#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

struct Adresat
{
    string imie = "", nazwisko = "", numerTelefonu = "", email = "", adres = "";
    int idAdresata = 0, idUzytkownika = 0;
};

struct Uzytkownik
{
    int idUzytkownika = 0;
    string nazwa = "", haslo = "";
};

void wczytajZarejestrowanychZPliku(vector <Uzytkownik> &uzytkownicy)
{
    fstream plikUzytkownicy;

    Uzytkownik tymczasowyUzytkownik;
    string idUzytkownikaString;

    plikUzytkownicy.open("Uzytkownicy.txt",ios::in);
    if(plikUzytkownicy.good()==false)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
        cout << "Brak pliku z danymi... ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
        cout << endl;
        system("pause");
    }
    else
    {
        while(getline(plikUzytkownicy >> ws,idUzytkownikaString, '|'))
        {
            tymczasowyUzytkownik.idUzytkownika=stoi(idUzytkownikaString);
            getline(plikUzytkownicy,tymczasowyUzytkownik.nazwa, '|');
            getline(plikUzytkownicy,tymczasowyUzytkownik.haslo, '|');

            uzytkownicy.push_back(tymczasowyUzytkownik);
        }
    }
    plikUzytkownicy.close();
}

void zapiszDaneUzytkownikowDoPliku(vector <Uzytkownik> &uzytkownicy)
{
    fstream plikUzytkownicy;

    plikUzytkownicy.open("Uzytkownicy.txt", ios::out);

    for(vector<Uzytkownik>::iterator itr=uzytkownicy.begin(); itr<uzytkownicy.end(); itr++)
    {
        plikUzytkownicy << (*itr).idUzytkownika << "|";
        plikUzytkownicy << (*itr).nazwa << "|";
        plikUzytkownicy << (*itr).haslo << "|" << endl;
    }
    plikUzytkownicy.close();
}

int wczytajAdresatowZPliku(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    fstream plikAdresaci;

    Adresat tymczasowyAdresat;
    string idAdresataString, idUzytkownikaString;

    plikAdresaci.open("Adresaci.txt",ios::in);
    if(plikAdresaci.good()==false)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
        cout << "Nie znaleziono danych adresatow";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
        system("pause");
        tymczasowyAdresat.idAdresata = 0;
    }
    else
    {
        tymczasowyAdresat.idAdresata = 0;
        while(getline(plikAdresaci >> ws,idAdresataString, '|'))
        {
            tymczasowyAdresat.idAdresata=stoi(idAdresataString);
            getline(plikAdresaci,idUzytkownikaString, '|');
            tymczasowyAdresat.idUzytkownika=stoi(idUzytkownikaString);
            getline(plikAdresaci,tymczasowyAdresat.imie, '|');
            getline(plikAdresaci,tymczasowyAdresat.nazwisko, '|');
            getline(plikAdresaci,tymczasowyAdresat.numerTelefonu, '|');
            getline(plikAdresaci,tymczasowyAdresat.email, '|');
            getline(plikAdresaci,tymczasowyAdresat.adres, '|');
            if(idZalogowanegoUzytkownika==tymczasowyAdresat.idUzytkownika)
                adresaci.push_back(tymczasowyAdresat);
        }
    }
    plikAdresaci.close();
    return tymczasowyAdresat.idAdresata;
}

bool sprawdzCzyIstniejeTakiUzytkownik(vector <Uzytkownik> &uzytkownicy,string nazwaUzytkownikaDoSprawdzenia)
{
    for(vector <Uzytkownik>::iterator itr=uzytkownicy.begin(); itr!=uzytkownicy.end(); itr++)
    {
        if((*itr).nazwa==nazwaUzytkownikaDoSprawdzenia)
            return 1;
    }
    return 0;
}

void  rejestracja(vector <Uzytkownik> &uzytkownicy)
{
    Uzytkownik tymczasowyUzytkownik;

    cout << "Podaj nazwe: ";
    cin.ignore();
    getline(cin, tymczasowyUzytkownik.nazwa);
    cout << "Podaj haslo: ";
    getline(cin, tymczasowyUzytkownik.haslo);

    if (uzytkownicy.empty())
        tymczasowyUzytkownik.idUzytkownika=1;
    else
    {
        tymczasowyUzytkownik.idUzytkownika= uzytkownicy.back().idUzytkownika+1;
        while(sprawdzCzyIstniejeTakiUzytkownik(uzytkownicy, tymczasowyUzytkownik.nazwa))
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
            cout << "Taki uzytkownik juz istnieje.";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            getline(cin, tymczasowyUzytkownik.nazwa);
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
    cout<<"Pomyslnie dodano uzytkownika"<<endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    system("pause");
    uzytkownicy.push_back(tymczasowyUzytkownik);
    zapiszDaneUzytkownikowDoPliku(uzytkownicy);
}

int logowanie(vector <Uzytkownik> &uzytkownicy)
{
    string nazwa, haslo;
    cout << endl;
    cout << "Podaj nazwe uzytkownika: ";
    cin.ignore();
    getline(cin, nazwa);

    for(vector<Uzytkownik>::iterator itr=uzytkownicy.begin(); itr!=uzytkownicy.end(); itr++)
    {
        if ((*itr).nazwa==nazwa)
        {
            for(int proby = 0; proby < 3; proby++)
            {
                cout << "Podaj haslo: Pozostalo prob: " << 3-proby << ": ";
                getline(cin, haslo);
                if((*itr).haslo==haslo)
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
                    cout << "Udalo sie zalogowac." << endl;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                    system("pause");
                    return (*itr).idUzytkownika;
                }
            }
            cout << "Podales 3 razy bledne haslo." << endl;
            Sleep(3000);
            return 0;
        }
    }
    cout << "Nie znaleziono uzytkownika" << endl;
    system("pause");
    return 0;
}

void wyszukajPoImieniuLubNazwisku(vector <Adresat> &adresaci, string wyborImieLubNazwisko)
{
    string wyszukiwane;
    int czyIstniejeTakiAdresat = 0;
    if(wyborImieLubNazwisko=="imie")
        cout << endl << "Podaj imie adresata: ";
    else
        cout << endl << "Podaj nazwisko adresata: ";

    cin >> wyszukiwane;

    for (vector<Adresat>::iterator itr=adresaci.begin(); itr!=adresaci.end(); itr++)
    {
        if(((*itr).imie==wyszukiwane && wyborImieLubNazwisko=="imie") || ((*itr).nazwisko==wyszukiwane && wyborImieLubNazwisko=="nazwisko" ))
        {
            cout << endl;
            cout << "Id:                " << (*itr).idAdresata       <<endl;
            cout << "Imie:              " << (*itr).imie             <<endl;
            cout << "Nazwisko:          " << (*itr).nazwisko         <<endl;
            cout << "Numer Telefonu:    " << (*itr).numerTelefonu    <<endl;
            cout << "email:             " << (*itr).email            <<endl;;
            cout << "Adres:             " << (*itr).adres            <<endl<<endl;
            czyIstniejeTakiAdresat=1;
        }
    }
    if(czyIstniejeTakiAdresat==0)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
    cout << endl;
    cout << "Brak adresata. Sprobuj ponownie" << endl;
    cout << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    system("pause");
}

void dodajAdresata(vector <Adresat> &adresaci,int idUzytkownika, int &idOstatniegoAdresata)
{
    Adresat tymczasowyAdresat;

    idOstatniegoAdresata++;
    tymczasowyAdresat.idAdresata= idOstatniegoAdresata;
    tymczasowyAdresat.idUzytkownika=idUzytkownika;

    cout << "Dodaj Adresata" << endl;
    cout << "Podaj imie: ";
    cin.ignore();
    getline(cin, tymczasowyAdresat.imie);
    cout << "Podaj nazwisko: ";
    getline(cin,tymczasowyAdresat.nazwisko);
    cout << "Podaj numer telefonu: ";
    getline(cin, tymczasowyAdresat.numerTelefonu);
    cout << "Podaj email: ";
    getline(cin,tymczasowyAdresat.email);
    cout << "Podaj Adres: ";
    getline(cin,tymczasowyAdresat.adres);

    adresaci.push_back(tymczasowyAdresat);
    fstream plikAdresaci;
    plikAdresaci.open("Adresaci.txt", ios::out| ios::app);
    plikAdresaci << tymczasowyAdresat.idAdresata << "|" << tymczasowyAdresat.idUzytkownika << "|" << tymczasowyAdresat.imie << "|"
                << tymczasowyAdresat.nazwisko  << "|" << tymczasowyAdresat.numerTelefonu << "|" << tymczasowyAdresat.email << "|"
                << tymczasowyAdresat.adres     << "|" << endl;
    plikAdresaci.close();
    cout << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
    cout << "Pomyslnie dodano adresata" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
}

void wyswietlWszystkichAdresatow(vector <Adresat> &adresaci)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout << endl;
    cout << "Lista wszystkich adresatow w ksiazce:" << endl;
    cout << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);


    for(vector<Adresat>::iterator  itr=adresaci.begin(); itr!=adresaci.end(); itr++)
    {
        cout<< "Id: "               << (*itr).idAdresata       <<endl;
        cout<< "Imie: "             << (*itr).imie             <<endl;
        cout<< "Nazwisko: "         << (*itr).nazwisko         <<endl;
        cout<< "Numer Telefonu: "   << (*itr).numerTelefonu    <<endl;
        cout<< "email: "            << (*itr).email            <<endl;
        cout<< "Adres: "            << (*itr).adres            <<endl<<endl;
    }
}

void usunAdresata(vector <Adresat> &adresaci)
{
    int idDoUsuniecia = 0;
    Adresat tymczasowyAdresat;
    string idAdresataString,idUzytkownikaString;

    wyswietlWszystkichAdresatow(adresaci);
    if (!adresaci.empty())
    {
        fstream plikAdresaci, plikTymczasowy;

        cout << "Podaj id adresata, ktory ma zostac usuniety" << endl;
        cin >> idDoUsuniecia;

        for(vector<Adresat>::iterator itr=adresaci.begin(); itr<adresaci.end(); itr++)
        {
            if((*itr).idAdresata==idDoUsuniecia)
                adresaci.erase(itr);
        }

        plikAdresaci.open("Adresaci.txt", ios::in);
        plikTymczasowy.open("Tymczasowy.txt", ios::out);

        while(getline(plikAdresaci >> ws,idAdresataString, '|'))
        {
            tymczasowyAdresat.idAdresata=stoi(idAdresataString);

            getline(plikAdresaci,idUzytkownikaString, '|');
            tymczasowyAdresat.idUzytkownika=stoi(idUzytkownikaString);

            getline(plikAdresaci,tymczasowyAdresat.imie, '|');
            getline(plikAdresaci,tymczasowyAdresat.nazwisko, '|');
            getline(plikAdresaci,tymczasowyAdresat.numerTelefonu, '|');
            getline(plikAdresaci,tymczasowyAdresat.email, '|');
            getline(plikAdresaci,tymczasowyAdresat.adres, '|');

            if (tymczasowyAdresat.idAdresata!=idDoUsuniecia)
            {
                plikTymczasowy<<tymczasowyAdresat.idAdresata<<"|"<<tymczasowyAdresat.idUzytkownika<<"|"<<tymczasowyAdresat.imie <<"|"
                              <<tymczasowyAdresat.nazwisko  <<"|"<<tymczasowyAdresat.numerTelefonu<<"|"<<tymczasowyAdresat.email<<"|"
                              <<tymczasowyAdresat.adres     <<"|"<<endl;
            }
        }
        plikAdresaci.close();
        plikTymczasowy.close();

        remove("Adresaci.txt");
        rename("Tymczasowy.txt", "Adresaci.txt");

        cout<<"Pomyslnie Ukonczono Usuwanie"<<endl;
        system("pause");
    }
}

void edytujAdresata(vector <Adresat> &adresaci)
{
    int wybor = 0, idDoZmiany;
    string idAdresataString, idUzytkownikaString;
    vector<Adresat>::iterator itr;
    Adresat tymczasowyAdresat, adresatDoZamiany;

    if (adresaci.empty())
        return;

    wyswietlWszystkichAdresatow(adresaci);
    cout << "Podaj numer Id adresata, ktorego dane maja zostac zmienione" << endl;
    cin >> idDoZmiany;

    for(vector<Adresat>::iterator itr=adresaci.begin(); itr<adresaci.end(); itr++)
    {
        while(wybor!=6 &&(*itr).idAdresata==idDoZmiany)
        {
            cout << "Wybierz Dane do zmiany:" << endl;
            cout << "1 - imie" << endl << "2 - nazwisko" << endl << "3 - numer telefonu" << endl << "4 - email" << endl << "5 - adres" << endl << "6 - powrot do menu" << endl;
            cin >> wybor;
            cin.ignore();

            switch (wybor)
            {
            case 1:
                cout << "Podaj nowe imie: ";
                cin >> (*itr).imie;
                break;
            case 2:
                cout << "Podaj nowe nazwisko: ";
                cin >> (*itr).nazwisko;
                break;
            case 3:
                cout << "Podaj nowy numer telefonu: ";
                cin >> (*itr).numerTelefonu;
                break;
            case 4:
                cout << "Podaj nowy email: ";
                cin >> (*itr).email;
                break;
            case 5:
                cout << "Podaj nowy adres: ";
                cin >> (*itr).adres;
                break;
            case 6:
                break;
            }
            adresatDoZamiany=(*itr);
        }
    }

    fstream plikAdresaci, plikTymczasowy;

    plikAdresaci.open("Adresaci.txt", ios::in);
    plikTymczasowy.open("Tymczasowy.txt", ios::out);

    while(getline(plikAdresaci >> ws,idAdresataString, '|'))
    {
        tymczasowyAdresat.idAdresata=stoi(idAdresataString);

        getline(plikAdresaci,idUzytkownikaString, '|');
        tymczasowyAdresat.idUzytkownika=stoi(idUzytkownikaString);

        getline(plikAdresaci,tymczasowyAdresat.imie, '|');
        getline(plikAdresaci,tymczasowyAdresat.nazwisko, '|');
        getline(plikAdresaci,tymczasowyAdresat.numerTelefonu, '|');
        getline(plikAdresaci,tymczasowyAdresat.email, '|');
        getline(plikAdresaci,tymczasowyAdresat.adres, '|');

        if (tymczasowyAdresat.idAdresata==idDoZmiany)
        {
            plikTymczasowy<<adresatDoZamiany.idAdresata<<"|"<<adresatDoZamiany.idUzytkownika<<"|"<<adresatDoZamiany.imie <<"|"
                          <<adresatDoZamiany.nazwisko  <<"|"<<adresatDoZamiany.numerTelefonu<<"|"<<adresatDoZamiany.email<<"|"
                          <<adresatDoZamiany.adres     <<"|"<<endl;
        }
        else
        {
            plikTymczasowy<<tymczasowyAdresat.idAdresata<<"|"<<tymczasowyAdresat.idUzytkownika<<"|"<<tymczasowyAdresat.imie <<"|"
                          <<tymczasowyAdresat.nazwisko  <<"|"<<tymczasowyAdresat.numerTelefonu<<"|"<<tymczasowyAdresat.email<<"|"
                          <<tymczasowyAdresat.adres     <<"|"<<endl;
        }
    }
    plikAdresaci.close();
    plikTymczasowy.close();

    remove("Adresaci.txt");
    rename("Tymczasowy.txt", "Adresaci.txt");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
    cout << "Dane zosta³y zaktualizowane" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    system("pause");
}

void zmianaHasla(vector <Uzytkownik> &uzytkownicy, int idZalogowanegoUzytkownika)
{
    string haslo;

    cout << "Podaj nowe haslo: ";
    cin.ignore();
    getline(cin, haslo);

    for(vector<Uzytkownik>::iterator itr=uzytkownicy.begin(); itr<uzytkownicy.end(); itr++)
        if ((*itr).idUzytkownika==idZalogowanegoUzytkownika)
            (*itr).haslo=haslo;

    zapiszDaneUzytkownikowDoPliku(uzytkownicy);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
    cout << "Haslo zostalo zmienione" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
}

int main()
{
    vector<Adresat> adresaci;
    vector<Uzytkownik> uzytkownicy;
    int wybranaOpcja, idOstatniegoAdresata;
    int idZalogowanegoUzytkownika = 0;
    char wybor;
    wczytajZarejestrowanychZPliku(uzytkownicy);

    while(1)
    {
        if(idZalogowanegoUzytkownika == 0 )
        {
            system("cls");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
            cout << " Witaj w ksiazce adresowej" << endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            cout << endl;
            cout << "1. Rejestracja"      << endl;
            cout << "2. Logowanie"        << endl;
            cout << "9. Zakoncz program"  << endl;
            cout << endl;
            cout << "TWOJ WYBOR: ";

            cin >> wybranaOpcja;

            if ( wybranaOpcja == 1)
                rejestracja(uzytkownicy);
            else if (wybranaOpcja == 2)
            {
                idZalogowanegoUzytkownika=logowanie(uzytkownicy);
                idOstatniegoAdresata=wczytajAdresatowZPliku(adresaci, idZalogowanegoUzytkownika);
            }
            else if (wybranaOpcja == 9)
                exit(0);
        }
        else
        {
            system("cls");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
            cout << " Witaj w ksiazce adresowej" << endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            cout << endl;

            cout << "1. Dodaj adresata"                 << endl;
            cout << "2. Wyszukaj po imieniu"            << endl;
            cout << "3. Wyszukaj po nazwisku"           << endl;
            cout << "4. Wyswietl wszystkich adresatow"  << endl;
            cout << "5. Usun adresata"                  << endl;
            cout << "6. Edytuj adresata"                << endl;
            cout << "7. Zmiana hasla"                   << endl;
            cout << "8. Wylogowanie"                    << endl;
            cout << endl;
            cout << "TWOJ WYBOR: ";

            cin >> wybor;

            switch(wybor)
            {
            case '1':
            {
                dodajAdresata(adresaci, idZalogowanegoUzytkownika, idOstatniegoAdresata);
                break;
            }
            case '2':
            {
                wyszukajPoImieniuLubNazwisku(adresaci, "imie");
                break;
            }
            case '3':
            {
                wyszukajPoImieniuLubNazwisku(adresaci, "nazwisko");
                break;
            }
            case '4':
            {
                wyswietlWszystkichAdresatow(adresaci);
                break;
            }
            case '5':
            {
                usunAdresata(adresaci);
                break;
            }
            case '6':
            {
                edytujAdresata(adresaci);
                break;
            }
            case '7':
            {
                zmianaHasla(uzytkownicy,idZalogowanegoUzytkownika);
                break;
            }
            case '8':
            {
                idZalogowanegoUzytkownika=0;
                adresaci.clear();
                break;
            }
            case '9':
            {
                exit(0);
            }
            default:
                cout << "Nie ma takiej opcji w menu. Wybierz jeszcze raz.\n";
                break;
            }
            system("pause");
        }
    }
    return 0;
}

