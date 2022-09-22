#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <fstream>
using std::ofstream;
using namespace std;
using namespace std::chrono;

vector<vector<double>> UnesiVektore(int brojVektora, int velicinaBaze);
void IspisiVektore(vector<vector<double>> vektori);
double IzracunajRazlomak(vector<double> v1, vector<double> v2);
vector<vector<double>> OrtogonalizacijaVektora(vector<vector<double>> vektori);
double DuljinaVektora(vector<double> vektor);
vector<vector<double>> NormirajVektor(vector<vector<double>> vektori);
double GramovaDeterminanta(vector<vector<double>> vektori);
double projekcijaniProdukt(vector<double> vektor1, vector<double> vektor2);
double IzracunajDeterminantu (vector<vector<double>> matrica);
void UnesiVektoreUFile(vector<vector<double>> vektori);

void UnesiVektoreUFile(vector<vector<double>> vektori){
    ofstream file;
    file.open("C:/Users/Tihomir/Documents/Faks/Zavrsni/.vscode/vektoriFile.txt");
    if(!file){
        cout<<"File nije pronaden!";
        exit(1);
    }
    for (int i = 0; i < vektori.size(); i++)
    {
        for (int j = 0; j < vektori[i].size(); j++)
        {
            file << vektori[i][j] << endl;
        }
    }
    file.close();
}

int main()
{
    int brojVektora, velicinaBaze;
    cout<<"Koliko vektora zelite?: ";
    cin>>brojVektora;
    cout<<"Unesi velicinu baze: ";
    cin>>velicinaBaze;

    vector<vector<double>> vektori = UnesiVektore(brojVektora, velicinaBaze);
    if(GramovaDeterminanta(vektori) == 0){
        cout<<"Vektori su linearno zavisni!";
        return -1;
    }
    cout<<"Uneseni vektori su linearno nezavisni!"<<'\n';
    
    //UnesiVektoreUFile(vektori);
    IspisiVektore(vektori);
    auto start = high_resolution_clock::now();
    vector<vector<double>> ortogonaliziraniVektori = OrtogonalizacijaVektora(
        vektori);
    cout<<"Ortogonalizirani vektori: \n";
    IspisiVektore(ortogonaliziraniVektori);
    vector<vector<double>> normiraniVektori = NormirajVektor(
        ortogonaliziraniVektori);
    cout<<"Ortonormirani vektori: \n";
    IspisiVektore(normiraniVektori);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop-start);
    cout<<"Vrijeme izvodenja programa: "<<duration.count()<<"[ms]"<<endl;
    return 0;
}


vector<vector<double>> UnesiVektore(int brojVektora, int velicinaBaze)
{
    vector<vector<double>> vektori;
    srand((unsigned) time(NULL));
    int vrijednost;
    for(int i=0; i<brojVektora; i++)
    {
        cout<<"Unesi dimenzije "<<i+1<<". vektora: \n";
        vector<double> vektor;
        for (int j = 0; j < velicinaBaze; j++)
        {
            cout<<"Vrijednost "<<j+1<<". dimenzije: ";
            //int random = 1 + (rand() % 9);
            cin>>vrijednost;
            vektor.push_back(vrijednost);
        }
        vektori.push_back(vektor);
    }
    return vektori;
}

void IspisiVektore(vector<vector<double>> vektori){
    for (int i = 0; i < vektori.size(); i++)
    {
        //cout<<i+1<<". vektor: \n";
        for (int j = 0; j < vektori[i].size(); j++)
        {
            cout<<vektori[i][j]<<" ";
        }
        cout<<"\n";
    }   
}

vector<vector<double>> OrtogonalizacijaVektora(
    vector<vector<double>> vektori){
    double razlomak;
    vector<double> projekcija;
    vector<double> aux;
    vector<vector<double>> e;
    vector<vector<double>> temp;
    vector<vector<double>> ortogonaliziraniVektori;

    for (int i = 0; i < vektori.size(); i++)
    {
        if (i == 0){
            e.push_back(vektori[i]);
            ortogonaliziraniVektori.push_back(e[i]);
        } 
        else{
            for (int j = 0; j < i; j++){
                razlomak = IzracunajRazlomak(vektori[i], e[j]);
                for (int k = 0; k < vektori[i].size(); k++)
                {
                    projekcija.push_back(razlomak*e[j][k]);
                }
                temp.push_back(projekcija);
                projekcija.clear();
            }
            for (int j = 0; j < vektori[i].size(); j++)
            {
                aux.push_back(vektori[i][j]);
            }
            for (int j = 0; j < i; j++)
            {
                for (int k = 0; k < vektori[i].size(); k++)
                {
                    aux[k] = aux[k] - temp[j][k];
                }
            }
            e.push_back(aux);
            ortogonaliziraniVektori.push_back(e[i]);
            temp.clear();
            aux.clear();
        }
    }
    return ortogonaliziraniVektori;
}

double IzracunajRazlomak(vector<double> vektor, vector<double> e){
    double brojnik = projekcijaniProdukt(vektor, e);
    double nazivnik = projekcijaniProdukt(e, e);
    return (brojnik/nazivnik);
}

vector<vector<double>> NormirajVektor(
    vector<vector<double>> vektori){
    vector<double> duljina; 
    vector<double> temp; 
    vector<vector<double>> normiraniVektori;
    for (int i = 0; i < vektori.size(); i++)
    {
        duljina.push_back(DuljinaVektora(vektori[i]));
    }
    for (int i = 0; i < vektori.size(); i++)
    {
        for (int j = 0; j < vektori[i].size(); j++)
        {
            temp.push_back(vektori[i][j]/duljina[i]);    
        }
        normiraniVektori.push_back(temp);
        temp.clear();
    }
    return normiraniVektori;
}

double DuljinaVektora(vector<double> vektor){
    double duljina = projekcijaniProdukt(vektor, vektor);
    return sqrt(duljina);
}

double GramovaDeterminanta(vector<vector<double>> vektori){
    vector<vector<double>> GramovaMatrica;
    vector<double> redak;
    for (int i = 0; i < vektori.size(); i++)
    {
        for (int j = 0; j < vektori.size(); j++)
        {
            redak.push_back(projekcijaniProdukt(vektori[i], vektori[j]));
        }
        GramovaMatrica.push_back(redak);
        redak.clear();
    }
    cout<<"Gramova matrica: "<<endl;
    IspisiVektore(GramovaMatrica);
    cout<<"Gramova determinanta: "<<IzracunajDeterminantu(GramovaMatrica)<<endl;
    return IzracunajDeterminantu(GramovaMatrica);
}

double projekcijaniProdukt(
    vector<double> vektor1, vector<double> vektor2)
{
    double zbroj = 0, umnozak = 1;
     
    for (int i = 0; i < vektor1.size(); i++)
    {
        umnozak = vektor1[i] * vektor2[i];
        zbroj += umnozak;
        umnozak = 1;
    } 
    return zbroj;   
}

double IzracunajDeterminantu (vector<vector<double>> matrica){
    double determinanta = 0;

    if (matrica.size() == 1){
        return matrica[0][0];
    } else if (matrica.size() == 2){
        return ((matrica[0][0] * matrica[1][1]) - (matrica[0][1] * matrica[1][0]));
    } else{
        for (int k = 0; k < matrica[0].size(); k++){
            vector<vector<double>> temp;
            for (int i = 1; i < matrica.size(); i++){
                vector<double> redak;
                for (int j = 0; j < matrica[i].size(); j++){
                    if (j != k){
                        redak.push_back(matrica[i][j]);
                    }
                }
                if(redak.size() > 0){
                    temp.push_back(redak);
                }
            }
            determinanta += matrica[0][k] * pow(-1,k) * IzracunajDeterminantu(temp);
        }
        return determinanta;
    }
}
