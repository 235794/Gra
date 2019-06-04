#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iomanip>
#include <limits>

// definice klasy i stuktury

class Gra
{
    enum class Symbole
    {
        nic = '-',
        czlowiek = 'X',
        komputer = 'O'
    };

    struct Ruch
    {
        unsigned x = 0;
        unsigned y = 0;
    };

    static const unsigned n = 3;	// tu mozna zmienic wymiary planszy

    Symbole plansza[n][n];
    unsigned puste;

public:
    Gra() : puste(n * n)
    {
        for (unsigned i = 0; i < n; i++)
        {
            for (unsigned j = 0; j < n; j++)
            {
                plansza[i][j] = Symbole::nic;
            }
        }
    }

// poczatek gry

    void start()
    {
        unsigned turn = 0;
        bool exit = false;

        Rysuj();

        do
        {
            // ruch czkowieka
            if (turn == 0)
            {
                TwojaKolej();

                if (Sprawdz(Symbole::czlowiek))
                {
                    std::cout << "WYGRANA\n";
                    exit = true;
                }
            }
	// ruch komputera
            else
            {

                Ruch przeciwnik = minimax();	// wywolanie algorytmu minimax

                plansza[przeciwnik.x][przeciwnik.y] = Symbole::komputer;
                puste--;

                if (Sprawdz(Symbole::komputer))
                {
                    std::cout << "PRZEGRANA\n";
                    exit = true;
                }
            }

            if (Remis())
            {
                std::cout << "\nREMIS\n";
                exit = true;
            }

            turn ^= 1;
            Rysuj();

        } while (!exit);
    }

private:

// funkcja wyswietla plansze

    void Rysuj()
    {
        for (unsigned i = 0; i < n; i++)
        {
            std::cout << "\n|";
            for (unsigned j = 0; j < n; j++)
            {
                std::cout << std::setw(3) << static_cast<char>(plansza[i][j]) << std::setw(3) << " |";
            }
        }
        std::cout << "\n\n";
    }

// funkcja oglaszajaca remis

    bool Remis()
    {
        return puste == 0;
    }

// funkcje sprawdzajace wygrana

    bool Sprawdz(Symbole Symbole)
    {
        // wygrana w kolumnie lub rzedzie
        for (unsigned i = 0; i < n; ++i)
        {
            bool rzad = true;
            bool kolumna = true;
            for (unsigned j = 0; j < n; ++j)
            {
                rzad &= plansza[i][j] == Symbole;
                kolumna &= plansza[j][i] == Symbole;
            }
            if (kolumna || rzad)
                return true;
        }

        // wygrana po przekatnej
        bool przekatna = true;
        for (unsigned i = 0; i < n; ++i)
            przekatna &= plansza[i][i] == Symbole;

        if (przekatna)
            return true;

        przekatna = true;
        for (unsigned i = 0; i < n; ++i)
            przekatna &= plansza[n - i - 1][i] == Symbole;

        return przekatna;
    }


// algorytm minimax

    Ruch minimax()
    {
        int punkty = std::numeric_limits<int>::max();
        Ruch Ruch;
        int poziom = 0;
	int licznik = 0;	// licznik ogranicza przeszukiwanie i czas trwania ruchu komputera
	

        for (unsigned i = 0; i < n; i++)
        {
            for (unsigned j = 0; j < n; j++)
            {
                if (plansza[i][j] == Symbole::nic)
                {
                    plansza[i][j] = Symbole::komputer;
                    puste--;

                    int temp = maxSearch(poziom, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), licznik);

                    if (temp < punkty)
                    {
                        punkty = temp;
                        Ruch.x = i;
                        Ruch.y = j;
                    }

                    plansza[i][j] = Symbole::nic;
                    puste++;
                }
            }
        }

        return Ruch;
    }

// funkcje wywoluja sie rekurencyjnie do sukcesu lub osiagniecia licznik=15

    int maxSearch(int poziom, int alpha, int beta, int licznik)
    {
        if (Sprawdz(Symbole::czlowiek)) { return 10; }
        else if (Sprawdz(Symbole::komputer)) { return -10; }
        else if (Remis()) { return 0; }

        int punkty = std::numeric_limits<int>::min();
	int licz; 
 
	if (licznik < 15)	// tu mozna zmienic wartosc glebokosci przeszukiwan
	{
	licz++;
        for (unsigned i = 0; i < n; i++)
        {
            for (unsigned j = 0; j < n; j++)
            {
                if (plansza[i][j] == Symbole::nic)
                {
                    plansza[i][j] = Symbole::czlowiek;
                    puste--;

                    punkty = std::max(punkty, minSearch(poziom + 1, alpha, beta, licznik+licz) - poziom);
                    alpha = std::max(alpha, punkty);

                    plansza[i][j] = Symbole::nic;
                    puste++;
         
                    if (beta <= alpha) return alpha;
                }
            }
        }
	}

        return punkty;
    }

    int minSearch(int poziom, int alpha, int beta, int licznik)
    {
        if (Sprawdz(Symbole::czlowiek)) { return 10; }
        else if (Sprawdz(Symbole::komputer)) { return -10; }
        else if (Remis()) { return 0; }

        int punkty = std::numeric_limits<int>::max();

        for (unsigned i = 0; i < n; i++)
        {
            for (unsigned j = 0; j < n; j++)
            {
                if (plansza[i][j] == Symbole::nic)
                {
                    plansza[i][j] = Symbole::komputer;
                    puste--;

                    punkty = std::min(punkty, maxSearch(poziom + 1, alpha, beta, licznik) + poziom);
                    beta = std::min(beta, punkty);

                    plansza[i][j] = Symbole::nic;
                    puste++;

                    if (beta <= alpha) return beta;
                }
            }
        }

        return punkty;
    }


// funkcja odczytujaca ruch czlowieka

    void TwojaKolej()
    {
        bool fail = true;
        unsigned x = -1, y = -1;

        do
        {
            std::cout << "Twoja kolej \n";

            char c;
            std::cin >> c;
            x = c - '0';
            std::cin >> c;
            y = c - '0';

            fail = plansza[x][y] != Symbole::nic;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        } while (fail);

        plansza[x][y] = Symbole::czlowiek;
        puste--;
    }
};




// glowny program, wywoluje funkcje

int main()
{

std::cout << " M.PIETRAS \n PAMSI \n PROJEKT 3 - GRA W KOLKO I KRZYZYK \n ";
std::cout << "RUCH W POSTACI: RZAD I KOLUMNA (00,01,02 itp.) \n";

    Gra runda;
    runda.start();
}
