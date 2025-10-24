#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define WDOL 2
#define WLEWO 4
#define WPRAWO 6
#define WGORE 8

typedef struct 
{
    char ** Plansza; //głowna plansza
    char * Komendy; //tablica z komendami, gdzie ! oznacza nowa linie
    int * Dlugosc_Wiersza; //tablica z dlugoscami wierszy; np arr[2] = 6 tlumaczy sie na Wiersz nr 2 ma dlugosc 6
    int dlugosc_Komendy; //dlugosc tablicy Komendy
    int liczba_wierszy; //liczba wierszy tablicy dwuwymiarowej Plansza
    int pozycja_gracza_wiersz;
    int pozycja_gracza_kolumna;
} DaneGry;

struct HistoriaGry
{
    char ** KopiaPlanszy;
    struct HistoriaGry * adres;
};


struct HistoriaGry * zapisz_stan_gry(DaneGry * dane_gry, struct HistoriaGry * poprzedni) //tworzymy kopie stanu planszy i dodajemy na poczatek listy.
{
    struct HistoriaGry * temp = malloc(sizeof(struct HistoriaGry));
    if (temp == NULL)
    {
        exit(1);
    }

    temp->adres = poprzedni;
    poprzedni = temp;

    temp->KopiaPlanszy = malloc((size_t)dane_gry->liczba_wierszy * sizeof(char*));
    if (temp->KopiaPlanszy == NULL)
    {
        free(temp);
        exit(1);
    }

    for (int w = 0; w < dane_gry->liczba_wierszy; w++)
    {
        temp->KopiaPlanszy[w] = malloc((size_t)dane_gry->Dlugosc_Wiersza[w] * sizeof(char));
        if (temp->KopiaPlanszy[w] == NULL)
        {
            for (int i = 0; i < w; i++)
            {
                free(temp->KopiaPlanszy[i]);
            }
            free(temp->KopiaPlanszy);
            free(temp);
            exit(1);
        }
        for (int k = 0; k < dane_gry->Dlugosc_Wiersza[w]; k++)
        {
            temp->KopiaPlanszy[w][k] = dane_gry->Plansza[w][k];
        }
    }
    return poprzedni;
}

void drukuj_plansze(DaneGry *dane_gry)
{
    for (int i = 0; i < dane_gry->liczba_wierszy; i++)
    {
        for (int j = 0; j < dane_gry->Dlugosc_Wiersza[i]; j++)
        {
            printf("%c", dane_gry->Plansza[i][j]);
        }
        printf("\n");
    }
}

DaneGry * wczytaj_plansze(void) //tworzy głowego struct'a oraz Plansze i zczytuje plansze z pliku wejsciowego. zwraca wskaznik na głownego struct'a.
{
    DaneGry * dane_gry = malloc(sizeof(DaneGry));
    if (dane_gry == NULL)
    {
        return NULL;
    }

    dane_gry->liczba_wierszy = 0; //maksymalna liczba wierszy
    int aktualna_kolumna = 0;
    size_t ile_wierszy_zaalokowanych = 8; // zmienna odpowiedzialna za alokacje wystarczajacej pamieci w wierszach
    size_t ile_kolumn_zaalokowanych = 8; // -- w kolumnach

    dane_gry->Plansza = malloc(ile_wierszy_zaalokowanych * sizeof(char*)); //alokujemy tablice wskaznikow na wskazniki
    if (dane_gry->Plansza == NULL)
    {
        free(dane_gry);
        return NULL;
    }

    dane_gry->Dlugosc_Wiersza = malloc(ile_wierszy_zaalokowanych * sizeof(int));

    if (dane_gry->Dlugosc_Wiersza == NULL) {
        free(dane_gry->Plansza);
        free(dane_gry);
        return NULL;
    }
    
    dane_gry->Plansza[0] = malloc(ile_kolumn_zaalokowanych * sizeof(char)); //alokujemy pierwszy wiersz
    if (dane_gry->Plansza[0] == NULL)
    {
        free(dane_gry->Dlugosc_Wiersza);
        free(dane_gry->Plansza);
        free(dane_gry);
        return NULL;
    }
    
    char znak;
    while ((znak = (char) getchar()) != EOF)
    {
        if (znak == '\n')
        {
            if (dane_gry->liczba_wierszy > 0 && aktualna_kolumna == 0)//doszlismy do konca stanu poczatkowego tablicy
            {
                free(dane_gry->Plansza[dane_gry->liczba_wierszy]); // zwalniamy nieuzyta pamiec
                break;
            }
            else if (dane_gry->liczba_wierszy == 0 && aktualna_kolumna == 0) //jesli znak planszy na 0,0 to nowa linia, to nic nie robimy i idziemy czytac nowy znak.
            {
                continue;
            }
            else // aktualna_kolumna != 0 (nie konczymy zczytywac planszy)
            {
                dane_gry->Dlugosc_Wiersza[dane_gry->liczba_wierszy] = aktualna_kolumna;
                dane_gry->Plansza[dane_gry->liczba_wierszy][aktualna_kolumna] = '\0';
                dane_gry->liczba_wierszy++;

                if (dane_gry->liczba_wierszy >= (int)ile_wierszy_zaalokowanych) //realokacja pamieci w przypadku dojscia do limitu
                {
                    ile_wierszy_zaalokowanych *= 2;
                    char ** temp = realloc(dane_gry->Plansza, ile_wierszy_zaalokowanych * sizeof(char*));
                    if (temp == NULL)
                    {
                        for (int i = 0; i < dane_gry->liczba_wierszy - 1; i++)
                        {
                            free(dane_gry->Plansza[i]);
                        }
                        free(dane_gry->Dlugosc_Wiersza);
                        free(dane_gry->Plansza);
                        free(dane_gry);
                        return NULL;
                    }
                    dane_gry->Plansza = temp;

                    int * temp2 = realloc(dane_gry->Dlugosc_Wiersza, ile_wierszy_zaalokowanych * sizeof(int));
                    if (temp2 == NULL)
                    {
                        for (int i = 0; i < dane_gry->liczba_wierszy - 1; i++)
                        {
                            free(dane_gry->Plansza[i]);
                        }
                        free(dane_gry->Plansza);
                        free(dane_gry);
                        return NULL;
                    }
                    dane_gry->Dlugosc_Wiersza = temp2;
                }

                dane_gry->Plansza[dane_gry->liczba_wierszy] = malloc(ile_kolumn_zaalokowanych * sizeof(char)); //miejsce na nowy wiersz

                if (dane_gry->Plansza[dane_gry->liczba_wierszy] == NULL)
                {
                    for (int i = 0; i < dane_gry->liczba_wierszy - 1; i++)
                    {
                        free(dane_gry->Plansza[i]);
                    }
                    free(dane_gry->Dlugosc_Wiersza);
                    free(dane_gry->Plansza);
                    free(dane_gry);
                    return NULL;
                }
                aktualna_kolumna = 0;
            }
        }
        else
        {
            dane_gry->Plansza[dane_gry->liczba_wierszy][aktualna_kolumna] = znak;
            aktualna_kolumna++;

            if (aktualna_kolumna >= (int)ile_kolumn_zaalokowanych)
            {
                ile_kolumn_zaalokowanych *= 2;
                char * temp = realloc(dane_gry->Plansza[dane_gry->liczba_wierszy], ile_kolumn_zaalokowanych * sizeof(char));
                if (temp == NULL)
                {
                    for (int i = 0; i <= dane_gry->liczba_wierszy; i++)
                    {
                        free(dane_gry->Plansza[i]);
                    }
                    free(dane_gry->Dlugosc_Wiersza);
                    free(dane_gry->Plansza);
                    free(dane_gry);
                    return NULL;
                }
                dane_gry->Plansza[dane_gry->liczba_wierszy] = temp;
            }
        }
    }
    return dane_gry;
}

void wczytaj_komendy(DaneGry *dane_gry) //zczytuje komendy z pliku wejsciowego do tablicy Komendy z glowengo struct'a
{
    dane_gry->dlugosc_Komendy = 0;
    size_t ile_miejsca_zaalokowane = 8;

    dane_gry->Komendy = malloc(ile_miejsca_zaalokowane * sizeof(char));
    
    if (dane_gry->Komendy == NULL)
    {
        exit(1);
    }
    char znak;

    while ((znak = (char) getchar()) != EOF)
    {
        if (znak == '.')
        {
            dane_gry->Komendy[dane_gry->dlugosc_Komendy] = '\0';
            break;
        }
        else if (znak == '\n')
        {
            dane_gry->Komendy[dane_gry->dlugosc_Komendy] = '!';
            dane_gry->dlugosc_Komendy++;
        }
        else
        {
            dane_gry->Komendy[dane_gry->dlugosc_Komendy] = znak;
            dane_gry->dlugosc_Komendy++;
        }

        if ((size_t)dane_gry->dlugosc_Komendy >= ile_miejsca_zaalokowane) //zwiekszamy pamiec
        {
            ile_miejsca_zaalokowane *= 2;
            char * temp = realloc(dane_gry->Komendy, ile_miejsca_zaalokowane * sizeof(char));
            if (temp == NULL)
            {
                free(dane_gry->Komendy);
                exit(1);
            }
            dane_gry->Komendy = temp;
        }  
    }
}

void znajdz_pozycje_gracza(DaneGry * dane_gry) //zapisuje wiesz i kolumne postaci w glownym struct'cie
{
    for (int i = 0; i < dane_gry->liczba_wierszy; i++)
    {
        for (int j = 0; j < dane_gry->Dlugosc_Wiersza[i]; j++) 
        {
            if (dane_gry->Plansza[i][j] == '@' || dane_gry->Plansza[i][j] == '*')
            {
                dane_gry->pozycja_gracza_wiersz = i;
                dane_gry->pozycja_gracza_kolumna = j;
                return;
            }
        }
    }
}

void znajdz_pozycje_skrzyni(DaneGry *dane_gry, char skrzynia, int *wiersz, int *kolumna)
{
    for (int i = 0; i < dane_gry->liczba_wierszy; i++)
    {
        for (int j = 0; j < dane_gry->Dlugosc_Wiersza[i]; j++)
        {
            if (dane_gry->Plansza[i][j] == (char)tolower(skrzynia) || dane_gry->Plansza[i][j] == (char)toupper(skrzynia))
            {
                *wiersz = i;
                *kolumna = j;
                return;
            }
        }
    }
    *wiersz = -1; //skrzyni nie znaleziono
    *kolumna = -1;
}

int czy_jest_droga(DaneGry * dane_gry, int wiersz, int kolumna, int wiersz_cel, int kolumna_cel, int ** odwiedzone) //funkcja zwraca 1 jesli istnieje droga z miejsca, w ktorym znajduje sie postac do zadanego celu metoda dfs.
{
    if (wiersz == wiersz_cel && kolumna == kolumna_cel)
    {
        return 1;
    }

    if (wiersz < 0 || wiersz >= dane_gry->liczba_wierszy || kolumna < 0 || kolumna >= dane_gry->Dlugosc_Wiersza[wiersz] ||
        odwiedzone[wiersz][kolumna] || (dane_gry->Plansza[wiersz][kolumna] != '-' && dane_gry->Plansza[wiersz][kolumna] != '+' && dane_gry->Plansza[wiersz][kolumna] != '*' && dane_gry->Plansza[wiersz][kolumna] != '@'))
    {
        return 0;
    }
    odwiedzone[wiersz][kolumna] = 1;

    if (czy_jest_droga(dane_gry, wiersz - 1, kolumna, wiersz_cel, kolumna_cel, odwiedzone) ||
        czy_jest_droga(dane_gry, wiersz + 1, kolumna, wiersz_cel, kolumna_cel, odwiedzone) ||
        czy_jest_droga(dane_gry, wiersz, kolumna - 1, wiersz_cel, kolumna_cel, odwiedzone) ||
        czy_jest_droga(dane_gry, wiersz, kolumna + 1, wiersz_cel, kolumna_cel, odwiedzone)) {
        return 1;
    }
    return 0;
}

int czy_mozna_pchnac(DaneGry * dane_gry, int wiersz_skrzyni, int kolumna_skrzyni, int kierunek)
{
    int wiersz_cel_postaci = -1, kolumna_cel_postaci = -1;

    if (kierunek == WGORE || kierunek == WDOL) //sprawdzamy "warunek konieczny" na mozliwosc pchniecia skrzyni tzn czy w orientacji w ktorej chcemy pchac skrzynie sąsiednie pola są wolne/dostepne
    {
        if (wiersz_skrzyni - 1 >= 0 && wiersz_skrzyni + 1 < dane_gry->liczba_wierszy && kolumna_skrzyni < dane_gry->Dlugosc_Wiersza[wiersz_skrzyni - 1] && kolumna_skrzyni < dane_gry->Dlugosc_Wiersza[wiersz_skrzyni + 1])
        {
            char docelowyminus1 = dane_gry->Plansza[wiersz_skrzyni - 1][kolumna_skrzyni];
            char docelowyplus1 = dane_gry->Plansza[wiersz_skrzyni + 1][kolumna_skrzyni];

            if ((docelowyminus1 == '-' || docelowyminus1 == '+' || docelowyminus1 == '@' || docelowyminus1 == '*')
            && (docelowyplus1 == '-' || docelowyplus1 == '+' || docelowyplus1 == '@' || docelowyplus1 == '*'))
            {
                switch (kierunek)
                {
                case WGORE:
                    wiersz_cel_postaci = wiersz_skrzyni + 1;
                    kolumna_cel_postaci = kolumna_skrzyni;
                    break;
                case WDOL:
                    wiersz_cel_postaci = wiersz_skrzyni - 1;
                    kolumna_cel_postaci = kolumna_skrzyni;
                    break;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0; 
        }
    }
    else
    {
        if (kolumna_skrzyni - 1 >= 0 && kolumna_skrzyni + 1 < dane_gry->Dlugosc_Wiersza[wiersz_skrzyni])
        {
            char docelowyminus1 = dane_gry->Plansza[wiersz_skrzyni][kolumna_skrzyni - 1];
            char docelowyplus1 = dane_gry->Plansza[wiersz_skrzyni][kolumna_skrzyni + 1];

            if ((docelowyminus1 == '-' || docelowyminus1 == '+' || docelowyminus1 == '@' || docelowyminus1 == '*')
            && (docelowyplus1 == '-' || docelowyplus1 == '+' || docelowyplus1 == '@' || docelowyplus1 == '*'))
            {
                switch (kierunek)
                {
                case WLEWO:
                    wiersz_cel_postaci = wiersz_skrzyni;
                    kolumna_cel_postaci = kolumna_skrzyni + 1;
                    break;
                case WPRAWO:
                    wiersz_cel_postaci = wiersz_skrzyni;
                    kolumna_cel_postaci = kolumna_skrzyni - 1;
                    break;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    // warunek konieczny spelniony, teraz nalezy sprawdzic czy istnieje droga do celu

    int ** odwiedzone = malloc((size_t)dane_gry->liczba_wierszy * sizeof(int*));
    if (odwiedzone == NULL)
    {
        exit(1);
    }

    for (int i = 0; i < dane_gry->liczba_wierszy; i++)
    {
        odwiedzone[i] = malloc((size_t)dane_gry->Dlugosc_Wiersza[i] * sizeof(int));
        if (odwiedzone[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(odwiedzone[j]);
            }
            free(odwiedzone);
            exit(1);
        }
        for (int j = 0; j < dane_gry->Dlugosc_Wiersza[i]; j++)
        {
            odwiedzone[i][j] = 0;
        }
    }
    
    int rezultat = czy_jest_droga(dane_gry, dane_gry->pozycja_gracza_wiersz, dane_gry->pozycja_gracza_kolumna, wiersz_cel_postaci, kolumna_cel_postaci, odwiedzone);

    for (int i = 0; i < dane_gry->liczba_wierszy; i++)
    {
        free(odwiedzone[i]);
    }
    free(odwiedzone);

    return rezultat;
}

void wykonaj_pchniecie(DaneGry * dane_gry, int kierunek, int wiersz_skrzyni, int kolumna_skrzyni, char skrzynia) //funkcja zmieniajaca znaki w odpowiednich komorkach w Planszy tak aby symulowalo pchniecie skrzyni przez gracza
{
    int wiersz_cel_skrzyni, kolumna_cel_skrzyni;
    switch (kierunek) //wspolrzednie docelowe skrzyni po pchnieciu
    {
    case WGORE:
        wiersz_cel_skrzyni = wiersz_skrzyni - 1;
        kolumna_cel_skrzyni = kolumna_skrzyni;
        break;
    case WDOL:
        wiersz_cel_skrzyni = wiersz_skrzyni + 1;
        kolumna_cel_skrzyni = kolumna_skrzyni;
        break;
    case WLEWO:
        wiersz_cel_skrzyni = wiersz_skrzyni;
        kolumna_cel_skrzyni = kolumna_skrzyni - 1;
        break;
    case WPRAWO:
        wiersz_cel_skrzyni = wiersz_skrzyni;
        kolumna_cel_skrzyni = kolumna_skrzyni + 1;
        break;
    default:
        return;
    }

    char znak_pozycja_docelowa_skrzyni = dane_gry->Plansza[wiersz_cel_skrzyni][kolumna_cel_skrzyni];
    char znak_pozycja_docelowa_postaci = dane_gry->Plansza[wiersz_skrzyni][kolumna_skrzyni];
    char znak_aktualna_komorka_postaci = dane_gry->Plansza[dane_gry->pozycja_gracza_wiersz][dane_gry->pozycja_gracza_kolumna];

    if (znak_pozycja_docelowa_skrzyni == '+' || znak_pozycja_docelowa_skrzyni == '*')
    {
        dane_gry->Plansza[wiersz_cel_skrzyni][kolumna_cel_skrzyni] = (char) toupper(skrzynia);
    }
    else
    {
        dane_gry->Plansza[wiersz_cel_skrzyni][kolumna_cel_skrzyni] = (char) tolower(skrzynia);
    }

    if (znak_pozycja_docelowa_postaci >= 'a' && znak_pozycja_docelowa_postaci <= 'z')
    {
        dane_gry->Plansza[wiersz_skrzyni][kolumna_skrzyni] = '@';
    }
    else
    {
        dane_gry->Plansza[wiersz_skrzyni][kolumna_skrzyni] = '*';
    }

    if (znak_aktualna_komorka_postaci == '*' && (wiersz_cel_skrzyni != dane_gry->pozycja_gracza_wiersz || kolumna_cel_skrzyni != dane_gry->pozycja_gracza_kolumna))
    {
        dane_gry->Plansza[dane_gry->pozycja_gracza_wiersz][dane_gry->pozycja_gracza_kolumna] = '+';
    }
    else if (znak_aktualna_komorka_postaci == '@' && (wiersz_cel_skrzyni != dane_gry->pozycja_gracza_wiersz || kolumna_cel_skrzyni != dane_gry->pozycja_gracza_kolumna))
    {
        dane_gry->Plansza[dane_gry->pozycja_gracza_wiersz][dane_gry->pozycja_gracza_kolumna] = '-';
    }

    //po udanym pchnieciu skrzyni gracz zawsze bedzie stal na pozycji skrzyni przed jej pchnieciem
    dane_gry->pozycja_gracza_wiersz = wiersz_skrzyni;
    dane_gry->pozycja_gracza_kolumna = kolumna_skrzyni;

}

void wykonaj_komendy(DaneGry * dane_gry) //funkcja odpowiedzialna za przetwarzanie komend i wykonywania działań
{
    struct HistoriaGry * Historia = NULL;

    for (int i = 0; i < dane_gry->dlugosc_Komendy; i++)
    {
        char komenda = dane_gry->Komendy[i];

        if (komenda == '!')
        {
            drukuj_plansze(dane_gry);
        }
        else if (komenda >= 'a' && komenda <= 'z')
        {
            i++;
            int kierunek = dane_gry->Komendy[i] - '0';
            
            int wiersz_skrzyni;
            int kolumna_skrzyni;
            znajdz_pozycje_skrzyni(dane_gry, komenda, &wiersz_skrzyni, &kolumna_skrzyni);

            if (wiersz_skrzyni >= 0 && kolumna_skrzyni >= 0 && czy_mozna_pchnac(dane_gry, wiersz_skrzyni, kolumna_skrzyni, kierunek))
            {
                Historia = zapisz_stan_gry(dane_gry, Historia);
                wykonaj_pchniecie(dane_gry, kierunek, wiersz_skrzyni, kolumna_skrzyni, komenda);
            }

        }
        else if (komenda == '0')
        {
            if (Historia != NULL) //zmieniamy stan glownej Planszy na ten ostatnio zapisany z listy a nastepnie zwalniamy uzyty element listy
            {
                for (int w = 0; w < dane_gry->liczba_wierszy; w++)
                {
                    for (int k = 0; k < dane_gry->Dlugosc_Wiersza[w]; k++)
                    {
                        dane_gry->Plansza[w][k] = Historia->KopiaPlanszy[w][k];
                    }
                }
                znajdz_pozycje_gracza(dane_gry);
                struct HistoriaGry * temp = Historia;

                Historia = Historia->adres;
                for (int r = 0; r < dane_gry->liczba_wierszy; r++)
                {
                    free(temp->KopiaPlanszy[r]);
                }
                free(temp->KopiaPlanszy);
                free(temp);
            }
        }
    }
    while (Historia != NULL) //zwalniamy nieuzyta historie gry
    {
        struct HistoriaGry * temp = Historia;
        Historia = Historia->adres;
        for (int wiersz = 0; wiersz < dane_gry->liczba_wierszy; wiersz++)
        {
            free(temp->KopiaPlanszy[wiersz]);
        }
        free(temp->KopiaPlanszy);
        free(temp);
    }
}

int main(void)
{
    DaneGry * DANE = wczytaj_plansze();
    if (DANE == NULL)
    {
        return 1;
    }
    znajdz_pozycje_gracza(DANE);
    wczytaj_komendy(DANE);
    drukuj_plansze(DANE);
    wykonaj_komendy(DANE);

    for (int i = 0; i < DANE->liczba_wierszy; i++)
    {
        free(DANE->Plansza[i]);
    }
    free(DANE->Plansza);
    free(DANE->Komendy);
    free(DANE->Dlugosc_Wiersza);
    free(DANE);
    return 0;
}