#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cctype>
#include <map>
#include <set>
#include <sstream>

using namespace std;

template<typename T1, typename T2, typename T3>
std::ostream& operator<<(std::ostream& os, const std::tuple<T1, T2, T3>& tpl) {
    os << '(' << std::get<0>(tpl) << ", " << std::get<1>(tpl) << ", " << std::get<2>(tpl) << ')';
    return os;
}

void backtrackAll(const vector<vector<int>>& score, const string& seq1, const string& seq2, int i, int j, string align1, string align2, set<tuple<string, string>>& alignments) {
    if (i == 0 && j == 0) 
    {
        alignments.insert(make_tuple(align1, align2));
        return;
    }

    if (i > 0 && j > 0) 
    {
        int scoreDiagonal = seq1[i - 1] == seq2[j - 1] ? 1 : -1;
        if (score[i][j] == score[i - 1][j - 1] + scoreDiagonal) 
        {
            backtrackAll(score, seq1, seq2, i - 1, j - 1, seq1[i - 1] + align1, seq2[j - 1] + align2, alignments);
        }
    }

    if (i > 0) 
    {
        if (score[i][j] == score[i - 1][j] - 2) 
        {
            backtrackAll(score, seq1, seq2, i - 1, j, seq1[i - 1] + align1, "-" + align2, alignments);
        }
    }

    if (j > 0) 
    {
        if (score[i][j] == score[i][j - 1] - 2) 
        {
            backtrackAll(score, seq1, seq2, i, j - 1, "-" + align1, seq2[j - 1] + align2, alignments);
        }
    }
}

tuple<string, string> needlemanWunschAll(const string& seq1, const string& seq2) {
    int m = seq1.size();
    int n = seq2.size();

    vector<vector<int>> score(m + 1, vector<int>(n + 1));

    for (int i = 0; i <= m; ++i) score[i][0] = -2 * i;
    for (int j = 0; j <= n; ++j) score[0][j] = -2 * j;

    for (int i = 1; i <= m; ++i) 
    {
        for (int j = 1; j <= n; ++j) 
        {
            int match = score[i - 1][j - 1] + (seq1[i - 1] == seq2[j - 1] ? 1 : -1);
            int del = score[i - 1][j] - 2;
            int ins = score[i][j - 1] - 2;

            score[i][j] = max({ match, del, ins });
        }
    }

    int min = 10000;

    string rpta;
    set<tuple<string, string>> alignments;

    backtrackAll(score, seq1, seq2, m, n, "", "", alignments);

    for (auto it = alignments.begin(); it != alignments.end(); ++it) {
        const auto& alignment = *it;
        int aux = 0;
        for (int x = 0; x < get<1>(alignment).length(); x++)
        {
            if (x + 1 < get<1>(alignment).length() && get<1>(alignment)[x + 1] != '-')
            {
                aux++;
            }
        }
        if (min > aux)
        {
            rpta = get<1>(alignment);
        }
    }

    string seq1_modificada = seq1;

    for (int x = 0; x < rpta.length() - seq1.length(); x++)
    {
        seq1_modificada = seq1_modificada + "-";
    }

    return make_tuple(seq1_modificada, rpta);

}

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

tuple<string, string, int> needlemanWunsch(const string& seq1, const string& seq2, int flag) {
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
    int cont = 0;
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
            cont++;
        }
    }

    if (flag == 0)
    {
        return make_tuple(align1, align2, score[m][n]);
    }
    else
    {
        string rpta = seq1;
        for (int x = 0; x < cont; x++)
        {
            rpta = rpta + "-";
        }

        return make_tuple(rpta, align2, score[m][n]);
    }

}

void star(string secuencias[], int tamanho, int imprimir)
{
    int** matriz;
    matriz = new int* [tamanho];
    for (int x = 0; x < tamanho; x++)
    {
        matriz[x] = new int[tamanho];
    } 

    for (int x = 0; x < tamanho; x++)
    {
        for (int y = 0; y < tamanho; y++)
        {
            auto [s1, s2, score] = needlemanWunsch(secuencias[x], secuencias[y],0);
            matriz[x][y] = (x == y ? -100 : score);
        }
    }
    
    if (imprimir == 1)
    {
        for (int x = 0; x < tamanho; x++)
        {
            for (int y = 0; y < tamanho; y++)
            {
                cout << matriz[x][y] << "\t";
            }
            cout << endl;
        }
    }
    else
    {
        int maximo = -100;
        int indice_maximo;

        for (int x = 0; x < tamanho; x++)
        {
            int suma = 0;
            for (int y = 0; y < tamanho; y++)
            {
                suma = suma + (x == y ? 0 : matriz[x][y]);
            }
            if (maximo < suma)
            {
                maximo = suma;
                indice_maximo = x;
            }

        }
        cout << "Score maximo es: "<<maximo << endl;
        cout << "El indice de la cadena con el score mayor es: "<<indice_maximo << endl;
        cout << "Imprimiendo los alineamientos: " << endl;

        int longitud_maxima = secuencias[indice_maximo].length();
        vector<string> alineamientos_finales;
        alineamientos_finales.push_back(secuencias[indice_maximo]);

        for (int x = 0; x < tamanho; x++)
        {
            if (x != indice_maximo)
            {
                auto [s1, s2] = needlemanWunschAll(secuencias[indice_maximo], secuencias[x]);
                if (longitud_maxima < s2.length())
                {
                    longitud_maxima = s2.length();
                }
                alineamientos_finales.push_back(s2);
            }  
        }

        for (auto it = alineamientos_finales.begin(); it != alineamientos_finales.end(); ++it)
        {
            string aux = *it;
            while (aux.length() < longitud_maxima)
            {
                aux = aux + "-";
            }
            cout << "Alignment " << aux << endl;
        }
        cout << endl;
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

    star(secuencias, 5,0);

    string AF = "TGCCGGCAGGGATGTGCTTG";
    string AR = "TGCTTGCAGTTTGCTTTCACTGATGGA";
    string BF = "GTTTAGGTTTTTGCTTATGCAGCATCCA";
    string BR = "TCAGGTACCCTGACCTTCTCTGAAC";
    string CF = "GGAAAAGCACAGAACTGGCCAACA";
    string CR = "GTGGGTTGTAAAGGTCCCAAATGGT";
    string DF = "GCCAGTTGGTTGATTTCCACCTCCA";
    string DR = "TGCCTTGGGTCCCTCTGACTGG";
    string EF = "ACCCCCGACATGCAGAAGCTG";
    string ER = "GTGGTGCATTGATGGAAGGAAGCA";
    string FF = "TGACGTGTCTGCTCCACTTCCA";
    string FR = "AGTGAGAGGAGCTCCCAGGGC";

    string txt[12];
    txt[0] = AF;
    txt[1] = AR;
    txt[2] = BF;
    txt[3] = BR;
    txt[4] = CF;
    txt[5] = CR;
    txt[6] = DF;
    txt[7] = DR;
    txt[8] = EF;
    txt[9] = ER;
    txt[10] = FF;
    txt[11] = FR;

    star(txt, 12, 1);
}
