#include <iostream>
#include <algorithm>    // std::max
#include <string>
#include <list>
#include <cctype> // Para usar isalpha()
#include <fstream>
#include <vector> 
#include <tuple>
using namespace std;

int sum_maximo = 100000;

string secuencia_minima1;
string secuencia_minima2;


vector<tuple<string, int>> respuestas;
list<string> rpta_optimas;




// Función para procesar el archivo y obtener las secuencias
void procesarSecuencias(const string& nombreArchivo, string& bacteria, string& sarsCov, string& influenza) {
    ifstream archivo(nombreArchivo);
    string linea;
    string* secuenciaActual = nullptr;

    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
        return;
    }

    while (getline(archivo, linea)) {
        // Determinar a qué secuencia pertenece la línea actual
        if (linea.find("Bacteria") != std::string::npos) {
            secuenciaActual = &bacteria;
        }
        else if (linea.find("Sars-Cov") != std::string::npos) {
            secuenciaActual = &sarsCov;
        }
        else if (linea.find("Influenza") != std::string::npos) {
            secuenciaActual = &influenza;
        }
        else if (secuenciaActual) {
            // Eliminar espacios y números de la línea actual antes de agregarla a la secuencia
            for (char ch : linea) {
                if (std::isalpha(ch)) { // Verifica si el caracter es una letra
                    *secuenciaActual += ch;
                }
            }
        }
    }

    archivo.close();
}




struct Node {
    int data;
    int root;
    Node* up;
    Node* left;
    Node* up_left;
    char letra1;
    char letra2;

    Node()
    {
        data = 0;
        up = NULL;
        left = NULL;
        up_left = NULL;
        root = 0;
        letra1 = ' ';
        letra2 = ' ';
    }
};

int maximo(Node** (&matriz),int indicex, int indicey, int a, int b, int c, char seq1, char seq2)
{
    int rpta;
    int flag = 0;
 

    //asignar el valor maximo
    if (seq1 == seq2)
    {
        flag = 1;
        rpta = max(a + 1, b - 2);
    }
    else
    {
        rpta = max(a - 1, b - 2);
    }

    rpta = max(rpta, c - 2);

    //asignar punteros
    if (rpta == a + 1 && flag == 1)
    {
        matriz[indicex][indicey].up_left = &matriz[indicex-1][indicey-1];
        //cout << "Puntero a..." << matriz[indicex][indicey].up_left->data << endl;
    }
    if (rpta == a - 1 && flag == 0)
    {
        matriz[indicex][indicey].up_left = &matriz[indicex - 1][indicey - 1];
        //cout << "Puntero a..." << matriz[indicex][indicey].up_left->data << endl;
    }
   if (rpta == b - 2)
    {
        matriz[indicex][indicey].up = &matriz[indicex-1][indicey];
        //cout << "Puntero a..." << matriz[indicex][indicey].left->data << endl;
    }
    if (rpta == c - 2)
    {
        matriz[indicex][indicey].left = &matriz[indicex][indicey-1];
        //cout << "Puntero a..." << matriz[indicex][indicey].up->data << endl;
    }
    return rpta;
}


void print(Node** matriz, int m, int n)
{
    for (int x = 0; x < m; x++)
    {
        for (int y = 0; y < n; y++)
        {
            cout << matriz[x][y].data << " \t ";
        }
        cout << endl;
    }
    cout << endl;
}

void print_punteros(Node** &matriz, int m, int n)
{
    for (int x = 0; x < m; x++)
    {
        for (int y = 0; y < n; y++)
        {
            cout << "Punteros de la posicion: " << x << " " << y << endl;
            if (matriz[x][y].up_left != NULL)
            {
                cout << "Up left: " << matriz[x][y].up_left->data << " ";
            }
            if (matriz[x][y].left != NULL)
            {
                cout << "Left: " << matriz[x][y].left->data << " ";
            }
            if (matriz[x][y].up != NULL)
            {
                cout << "Up : " << matriz[x][y].up->data << " ";
            }
            cout << endl;
        }
    }
    cout << endl;
}

int Neddleman_Wunch(Node** (& matriz), int m, int n, string seq1, string seq2)
{
    for (int x = 1; x < m; x++)
    {
        for (int y = 1; y < n; y++)
        {
            matriz[x][y].letra1 = seq1[x-1];
            matriz[x][y].letra2 = seq2[y-1];
        }
    }

    for (int x = 1; x < m; x++)
    {
        matriz[0][x].letra2 = seq2[x - 1];
    }

    for (int x = 1; x < m; x++)
    {
        matriz[x][0].letra1 = seq2[x - 1];
    }



    int cont = 0;
    matriz[0][0].data = cont;
    matriz[0][0].root = 1;

    cont = cont - 2;

    for (int x = 1; x < m; x++) {
        matriz[x][0].data = cont;
        matriz[x][0].up = &matriz[x - 1][0];
        cont = cont - 2;
    }

    cont = -2;

    for (int y = 1; y < n; y++) {
        matriz[0][y].data = cont;
        matriz[0][y].left = &matriz[0][y-1];
        cont = cont - 2;
    }

    for (int x = 1; x < m; x++)
    {
        for (int y = 1; y < n; y++)
        {
            matriz[x][y].data=maximo(matriz, x, y, matriz[x - 1][y - 1].data, matriz[x - 1][y].data, matriz[x][y - 1].data, seq1[x-1], seq2[y-1]);
        }
    }
    
    return matriz[m-1][n-1].data;
}

void camino_corto(Node bottom, int sum, string seq1, string seq2, int inicio)
{
    int rpta = 0;

    string aux = "";
    if (bottom.root == 1)
    {
        if (sum < sum_maximo)
        {
            secuencia_minima1 = seq1;
            secuencia_minima2 = seq2;
            sum_maximo = sum;
        }

        //cout << "Seq1: "<<seq1 << endl;
        //cout << "Seq2: "<<seq2 << endl;
        //cout << "----------------------"<<endl; 

        respuestas.push_back(make_tuple(seq1, sum));
        respuestas.push_back(make_tuple(seq2, sum));
        
        return;
    }
    if (bottom.up != NULL)
    {
        inicio = 0;
        aux = "";
        aux = aux + bottom.letra1;
        seq1.insert(0, aux);
        aux = "-";

        if (inicio == 1)
        {
            sum++;
        }

        seq2.insert(0, aux);
        inicio = 1;

        camino_corto(*bottom.up,sum ,seq1,seq2,inicio);

        aux = "";
        seq1.replace(0,1, "");
        seq2.replace(0, 1, "");
        sum--;
    }
    if (bottom.left != NULL)
    {
        inicio = 0;
        aux = "";
        aux = "-";

        if (inicio == 1)
        {
            sum++;
        }

        seq1.insert(0, aux);
        aux = "";
        aux = aux + bottom.letra2;
        seq2.insert(0, aux);
        inicio = 1;

        camino_corto(*bottom.left, sum ,seq1,seq2,inicio);

        aux = "";
        seq1.replace(0, 1, "");
        seq2.replace(0, 1, "");
        sum--;
    }
    if (bottom.up_left != NULL)
    {
        inicio = 0;
        aux = "";
        aux = aux + bottom.letra1;
        seq1.insert(0, aux);
        aux = "";
        aux = aux + bottom.letra2;
        seq2.insert(0, aux);
        inicio = 1;

        camino_corto(*bottom.up_left, sum ,seq1,seq2,inicio);

        aux = "";
        seq1.replace(0, 1, "");
        seq2.replace(0, 1, "");
        sum--;
    }
    return;
}


void matriz_puntos(int m, int n, string seq1, string seq2)
{
    cout << endl;
    cout<<"Imprimiendo matriz de puntos de las secuencias: "<<seq1<<" y "<<seq2<<endl;
    char** puntos;
    puntos = new char*[m-1];
    for (int x = 0; x < m-1; x++)
    {
        puntos[x] = new char[n-1];
    }

    for (int x = 0; x < m-1; x++)
    {
        for (int y = 0; y < n-1; y++)
        {
            if (seq1[x] == seq2[y])
            {
                puntos[x][y] = '*';
            }
            else
            {
                puntos[x][y] = ' ';
            }
            cout << puntos[x][y]<<" ";
            
        }
        cout << endl;
    }
}


int main() {
    Node** matriz;
    string seq1 = "AAAC";
    string seq2 = "AGC";
    int m = seq1.length()+1;
    int n = seq2.length()+1;
    string rpta1 = "";
    string rpta2 = "";


    /* 
    matriz = new Node * [m];
    for (int x = 0; x < m; x++)
    {
        matriz[x] = new Node[n];
    }
    
    cout << "Score Obtenido: " << Neddleman_Wunch(matriz, m, n, seq1, seq2) << endl;

    print(matriz, m, n);

    sum_maximo = 100000;
    camino_corto(matriz[m - 1][n - 1],0,rpta1,rpta2,0);

    cout << "Secuencia minima 1: " << secuencia_minima1 << endl;
    cout << "Secuencia minima 2: " << secuencia_minima2 << endl;

    matriz_puntos(m, n, seq1, seq2);
    */


    string bacteria, sarsCov, influenza;
    procesarSecuencias("C:\\Users\\Admin\\Downloads\\Sequencias1.txt", bacteria, sarsCov, influenza);

    // Imprimir las secuencias para verificar
    cout << "Bacteria:\n" << bacteria << "\n\n";
    cout << "Sars-Cov:\n" << sarsCov << "\n\n";
    cout << "Influenza:\n" << influenza << std::endl;

    m = sarsCov.length() + 1;
    n = influenza.length() + 1;

    matriz = new Node * [m];
    for (int x = 0; x < m; x++)
    {
        matriz[x] = new Node[n];
    }
    cout << endl;
    cout << "Score Obtenido: " << Neddleman_Wunch(matriz, m, n, sarsCov, influenza) << endl;

    //print(matriz, m, n);

    sum_maximo = 100000000;
    camino_corto(matriz[m - 1][n - 1], 0, rpta1, rpta2,0);

    cout << "Secuencia minima 1: " << secuencia_minima1 << endl;
    cout << "Secuencia minima 2: " << secuencia_minima2 << endl;
    //matriz_puntos(m, n, bacteria, sarsCov);


   
}
