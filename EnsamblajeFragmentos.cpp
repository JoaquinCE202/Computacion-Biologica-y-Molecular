#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;
int minimo = 1000;
vector<bool> inversos_finales;
string consenso_final;
int longitud_deseada = 55;

int minimo_hamilton = 1000;
string consenso_final_hamilton;
int t = 0;

vector<int> camino_final;
vector<int> camino_final_acumulado;


int encontrar_mejor_overlap(const string& cadena1, const string& cadena2)
{
    int max_overlap = 0;
    int limite = min(cadena1.size(), cadena2.size());

    for (int i = 1; i <= limite; i++)
    {
        string inicio_overlap = cadena1.substr(cadena1.size() - i);
        string fin_overlap = cadena2.substr(0, i);

        if (inicio_overlap == fin_overlap)
        {
            max_overlap = i;
        }
    }
    return max_overlap;
}

void encontrar_cadena_consenso(string consenso, const vector<string>& cadenas, vector<bool>& usadas, vector<bool>& inversos, int usadas_count)
{
    if (usadas_count == cadenas.size())
    {
        //cout << "Secuencia de consenso: " << consenso << endl;
        int n1 = (consenso.length() - longitud_deseada);
        if (abs(minimo- longitud_deseada) > abs(n1))
        {
            minimo = consenso.length();
            inversos_finales = inversos;
            consenso_final = consenso;
        }
        return;
    }

    for (int i = 0; i < cadenas.size(); ++i)
    {
        if (!usadas[i])
        {
            int overlap = encontrar_mejor_overlap(consenso, cadenas[i]);

            string aux1, aux2;
            aux1 = cadenas[i];
            aux2 = cadenas[i];

            for (int x = 0; x < aux1.length(); x++)
            {
                if (aux2[aux2.length() -1 - x] == 'A')
                {
                    aux1[x] = 'T';
                }
                else if (aux2[aux2.length() - 1 - x] == 'C')
                {
                    aux1[x] = 'G';
                }
                else if (aux2[aux2.length() - 1 - x] == 'G')
                {
                    aux1[x] = 'C';
                }
                else if (aux2[aux2.length() - 1 - x] == 'T')
                {
                    aux1[x] = 'A';
                }
                else
                {
                    aux1[x] = aux2[aux2.length() - 1 - x];
                }
            }

            int overlap2 = encontrar_mejor_overlap(consenso,aux1);
            usadas[i] = true;
            if (overlap2 > overlap)
            {
                inversos[i] = true;
                encontrar_cadena_consenso(consenso + aux1.substr(overlap2), cadenas, usadas, inversos, usadas_count + 1);
                inversos[i] = false;

            }
            else
            {
               encontrar_cadena_consenso(consenso + cadenas[i].substr(overlap), cadenas, usadas,inversos, usadas_count + 1);

            }
            usadas[i] = false; 
        }
    }
}

void recursion_hamilton(const vector<string>& cadenas, vector<bool>& usadas,int** caminos, int inicio,int cont, int n, string acumulado)
{
    if (cont == n)
    {
        //cout << acumulado.length();
;        int n1 = (acumulado.length() - longitud_deseada);
        if (abs(minimo_hamilton - longitud_deseada) > abs(n1))
        {
            minimo_hamilton = acumulado.length();
            consenso_final_hamilton = acumulado;
            camino_final_acumulado = camino_final;
        }
        return;
    }

    for (int x = 0; x < n; x++)
    {
        if (caminos[inicio][x] > t && inicio != x && usadas[x] == false)
        {
            cont = cont + 1;
            string aux = acumulado;
            usadas[x] = true;
            acumulado = acumulado + cadenas[x].substr(caminos[inicio][x]);
            camino_final.push_back(x);

            recursion_hamilton(cadenas,usadas,caminos, x, cont, n, acumulado);
            cont = cont - 1;
            acumulado = aux;
            usadas[x] = false;
            camino_final.pop_back();
        }
    }
}


void camino_hamiltonieano(int n, vector<string>& cadenas)
{
    vector<string>& cadenas_finales = cadenas;
    int** caminos;
    caminos = new int*[n];
    for (int x = 0; x < n; x++)
    {
        caminos[x] = new int[n];
    }
    
    /* 
    for (int x = 0; x < n; x++)
    {
        for (int y = 0; y < n; y++)
        {
            cout << caminos[x][y] << " ";
        }
        cout << endl;
    }
    */
    int contador = 0;
    for (vector<bool>::iterator it_inversos = inversos_finales.begin(); it_inversos != inversos_finales.end(); ++it_inversos)
    {
        if (*it_inversos == true)
        {
            string aux_hamilton = cadenas_finales[contador];
            for (int x = 0; x < cadenas_finales[contador].length(); x++)
            {
                if (aux_hamilton[aux_hamilton.length() - 1 - x] == 'A')
                {
                    cadenas_finales[contador][x] = 'T';
                }
                else if (aux_hamilton[aux_hamilton.length() - 1 - x] == 'C')
                {
                    cadenas_finales[contador][x] = 'G';
                }
                else if (aux_hamilton[aux_hamilton.length() - 1 - x] == 'G')
                {
                    cadenas_finales[contador][x] = 'C';
                }
                else if (aux_hamilton[aux_hamilton.length() - 1 - x] == 'T')
                {
                    cadenas_finales[contador][x] = 'A';
                }
                else
                {
                    cadenas_finales[contador][x] = aux_hamilton[aux_hamilton.length() - 1 - x];
                }
            }
        }
        contador++;
    }

    for (int x = 0; x < n; x++)
    {
        for (int y = 0; y < n; y++)
        {
            caminos[x][y] = encontrar_mejor_overlap(cadenas_finales[x], cadenas_finales[y]);
        }
    }

    vector<bool> usadas(cadenas.size(), false);
    string acumulado="";

    for (int x = 0; x < n; x++)
    {
        string aux = acumulado;
        acumulado = acumulado + cadenas[x];
        usadas[x] = true;
        camino_final.push_back(x);
        recursion_hamilton(cadenas_finales, usadas, caminos, x, 1, cadenas.size(), acumulado);
        acumulado = aux;
        usadas[x] = false;
        camino_final.pop_back();
    }

    for (int x = 0; x < n; x++)
    {
        cout << x << " -> ";
        for (int y = 0; y < n; y++)
        {
            if (x != y && caminos[x][y] != 0)
            {
                cout << y << ", ";
            }
        }
        cout << endl;
    }

}

int main() {

    vector<string> cadenas = { "ATCCGTTGAAGCCGCGGGC", "TTAACTCGAGG", "TTAAGTACTGCCCG", "ATCTGTGTCGGG", "CGACTCCCGACACA", "CACAGATCCGTTGAAGCCGCGGG","CTCGAGTTAAGTA", "CGCGGGCAGTACTT"};
    //vector<string> cadenas = { "ATC", "TTA", "TTAA", "ATCT"};
    vector<bool> usadas(cadenas.size(), false);
    vector<bool> inversos(cadenas.size(), false);

    for (int i = 0; i < cadenas.size(); ++i) 
    {
        usadas[i] = true;
        encontrar_cadena_consenso(cadenas[i], cadenas, usadas,inversos, 1);
        usadas[i] = false;
    }

    cout << "Consenso final: "<<consenso_final << endl;
    cout << "Minimo: " << minimo << endl;
    cout << "inversos finales: ";
    for (vector<bool>::iterator it = inversos_finales.begin(); it != inversos_finales.end(); ++it)
        cout << ' ' << *it;
    cout << endl;

    camino_hamiltonieano(cadenas.size(), cadenas);
    cout << "Consenso final de hamilton: "<<consenso_final_hamilton << endl;
    cout << "Minimo final: "<<minimo_hamilton << endl;
    cout << "Camino recorrido: " << endl;

    for (vector<int>::iterator it = camino_final_acumulado.begin(); it != camino_final_acumulado.end(); ++it)
        cout << ' ' << *it;
    cout << endl;

    return 0;
}
