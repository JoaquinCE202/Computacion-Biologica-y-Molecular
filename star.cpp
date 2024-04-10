#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cctype>

using namespace std;

template<typename T1, typename T2, typename T3>
std::ostream& operator<<(std::ostream& os, const std::tuple<T1, T2, T3>& tpl) {
    os << '(' << std::get<0>(tpl) << ", " << std::get<1>(tpl) << ", " << std::get<2>(tpl) << ')';
    return os;
}


// Función para procesar el archivo y obtener las secuencias
void procesarSecuencias(const string& nombreArchivo, string& bacteria, string& sarsCov, string& influenza) {
    ifstream archivo(nombreArchivo);
    string linea;
    string* secuenciaActual = nullptr;

    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    while (getline(archivo, linea)) {
        if (linea.find("Bacteria") != string::npos) {
            secuenciaActual = &bacteria;
        }
        else if (linea.find("Sars-Cov") != string::npos) {
            secuenciaActual = &sarsCov;
        }
        else if (linea.find("Influenza") != string::npos) {
            secuenciaActual = &influenza;
        }
        else if (secuenciaActual) {
            for (char ch : linea) {
                if (isalpha(ch)) {
                    *secuenciaActual += ch;
                }
            }
        }
    }
}

// Función para calcular el mejor alineamiento y el score utilizando DP
tuple<string, string, int> needlemanWunsch(const string& seq1, const string& seq2) {
    int m = seq1.size();
    int n = seq2.size();

    vector<vector<int>> score(m + 1, vector<int>(n + 1));

    // Inicialización
    for (int i = 0; i <= m; i++)
    {
        score[i][0] = -2 * i;
    }

    for (int j = 0; j <= n; j++)
    {
        score[0][j] = -2 * j;
    }

    // Relleno de la matriz
    for (int i = 1; i <= m; i++) 
    {
        for (int j = 1; j <= n; j++) 
        {
            int match;

            if (seq1[i - 1] == seq2[j - 1]) 
            {
                match = score[i - 1][j - 1] + 1;
            }
            else {
                match = score[i - 1][j - 1] - 1;
            }

            int del = score[i - 1][j] - 2;
            int ins = score[i][j - 1] - 2;

            score[i][j] = max({ match, del, ins });
        }
    }
    // Reconstrucción del alineamiento
    string align1 = "";
    string align2 = "";

    int i = m;
    int j = n;
    while (i > 0 || j > 0) 
    {
        if (i > 0 && j > 0 && score[i][j] == score[i - 1][j - 1] + (seq1[i - 1] == seq2[j - 1] ? 1 : -1)) {
            align1 = seq1[i - 1] + align1;
            align2 = seq2[j - 1] + align2;
            i--; j--;
        }
        else if (i > 0 && score[i][j] == score[i - 1][j] - 2) {
            align1 = seq1[i - 1] + align1;
            align2 = "-" + align2;
            i--;
        }
        else {
            align1 = "-" + align1;
            align2 = seq2[j - 1] + align2;
            j--;
        }
    }
    return make_tuple(align1, align2, score[m][n]);
}



void star(string secuencias[], int tamanho)
{
    int matriz[5][5];

    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            auto [s1, s2, score] = needlemanWunsch(secuencias[x], secuencias[y]);
            matriz[x][y] = (x == y ? -100 : score);
        }
    }
    /* 
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            cout << matriz[x][y] << "\t";
        }
        cout << endl;
    }
    */


    int maximo = -100;
    int indice_maximo;
    
    for (int x = 0; x < 5; x++)
    {
        int suma = 0;
        for (int y = 0; y < 5; y++)
        {
            suma = suma + (x == y ? 0 : matriz[x][y]);
        }
        if (maximo < suma)
        {
            maximo = suma;
            indice_maximo = x;
        }
        
    }
    cout << maximo << endl;
    cout << indice_maximo << endl;

    for (int x = 1; x < 5; x++)
    {

        auto [s1, s2, score] = needlemanWunsch(secuencias[0], secuencias[x]);
        cout << "s1: " << s1 << endl;
        cout << "s2: " << s2 << endl;
        cout << "====================" << endl;

    }
}

int main() {

    string s1 = "ATTGCCATT";
    string s2 = "ATGGCCATT";
    string s3 = "ATCCAATTTT";
    string s4 = "ATCTTCTT";
    string s5 = "ACTGACC"; 

    string secuencias[5];
    secuencias[0]=s1;
    secuencias[1]=s2;
    secuencias[2]=s3;
    secuencias[3]=s4;
    secuencias[4]=s5;

    star(secuencias, 5);

    /* 
    string bacteria, sarsCov, influenza;
    procesarSecuencias("C:\\Users\\Admin\\Downloads\\Sequencias1.txt", bacteria, sarsCov, influenza);

    // Imprimir las secuencias para verificar
    cout << "Bacteria:\n" << bacteria << "\n\n";
    cout << "Sars-Cov:\n" << sarsCov << "\n\n";
    cout << "Influenza:\n" << influenza << endl;

    auto [align1, align2, score] = needlemanWunsch(sarsCov, influenza);

    cout << endl;
    cout << "Mejor alineamiento:\n";
    cout << align1 << "\n";
    cout << align2 << "\n";
    cout << "Score: " << score << endl;
    */



    return 0;
}
