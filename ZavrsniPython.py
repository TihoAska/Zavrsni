from array import array
from cmath import sqrt
from datetime import datetime
import numpy as np
import copy
import time
import random

class VectorOperator: 
    def unesi_vektore_iz_fajla(self, broj_vektora, velicina_baze):
        vektori = []
        with open('C:/Users/Tihomir/Documents/Faks/Zavrsni/.vscode/vektoriFile.txt') as file:
            for line in file:
                vektori.append(line.rstrip())
        novi_vektor = []
        novi_vektori = []
        i = 0
        for broj in range(len(vektori)):
            novi_vektor.append(vektori[broj])
            i+=1
            if int(i) == int(velicina_baze):
                novi_vektori.append(novi_vektor.copy())
                novi_vektor.clear()
                i = 0
        return novi_vektori
        
            
    def unesi_vektore(self, broj_vektora, velicina_baze): 
        vektori = []
        vektor = []
        i = 0
        j = 0
        
        for broj in range(int(broj_vektora)):
            print("Unesi " + str(i+1) + ". vektor: ")
            for velicina in range(int(velicina_baze)):
                vrijednost = input("Unesi " + str(j+1) + ". dimenziju vektora: ")
                #vrijednost = random.randint(1, 10)
                vektor.append(vrijednost)
                j=j+1
            vektori.append(vektor.copy())
            vektor.clear()
            i=i+1
            j=0
        return vektori

    def normiraj_vektor(self, vektori):
        duljina = []
        temp = []
        normirani_vektor = []
        
        for i in range(len(vektori)):
            duljina.append(np.linalg.norm(vektori[i]))

        for i in range(len(vektori)):
            for j in range(len(vektori[i])):
                temp.append(float(vektori[i][j])/float(duljina[i]))
            normirani_vektor.append(temp.copy())
            temp.clear()
        return normirani_vektor

    def ispisi_vektore(self, vektori):
        for i in range(len(vektori)):
            print(str(vektori[i]))
            print(str(i+1) + ". vektor: " + str(vektori[i]))

    def gram_schmidtov_postupak_ortogonalizacije(self, vektori): 
        e = []
        ortogonalizirani_vektori = []
        projekcija = []
        temp = []
        aux = []
        for i in range(len(vektori)):
            if i==0:
                e.append(vektori[i])
                ortogonalizirani_vektori.append(e[i])
            else:
                for j in range(i):
                    razlomak = VectorOperator.izracunaj_razlomak(
                        self, vektori[i], e[j])
                    for k in range(len(vektori[i])):
                        projekcija.append(float(razlomak)*float(e[j][k]))
                    temp.append(projekcija.copy())
                    projekcija.clear()
                for j in range(len(vektori[i])):
                    aux.append(vektori[i][j])
                for j in range(i):
                    for k in range(len(vektori[i])):
                        aux[k] = float(aux[k]) - float(temp[j][k])
                e.append(aux.copy())
                ortogonalizirani_vektori.append(e[i])
                temp.clear()
                aux.clear()
        return ortogonalizirani_vektori

    def izracunaj_razlomak(self, vektor, e):
        brojnik = np.dot(np.array(vektor, dtype=float), np.array(e, dtype=float))
        nazivnik = np.dot(np.array(e, dtype=float), np.array(e, dtype=float))
        return brojnik/nazivnik

    def dobij_gramovu_matricu(self, vektori):
        gramova_matrica = []
        redak = []
        for i in range(len(vektori)):
            for j in range(len(vektori)):
                redak.append(np.dot(
                    np.array(
                        vektori[i], dtype=float), 
                        np.array(vektori[j], dtype=float)))
            gramova_matrica.append(redak.copy())
            redak.clear()
        return gramova_matrica

vector_operator = VectorOperator()
broj_vektora = input("Unesi broj vektora: ")
velicina_baze = input("Unesi velicinu baze: ")
vektori = vector_operator.unesi_vektore(broj_vektora, velicina_baze)

gramova_matrica = vector_operator.dobij_gramovu_matricu(vektori)
gramova_determinanta = '{:.2f}'.format(np.linalg.det(np.matrix(gramova_matrica, dtype=float)))
print("Gramova matrica: ")
print(gramova_matrica)
if  float(gramova_determinanta) == 0:
    print("Gramova determinanta: " + gramova_determinanta)
    print ("Vektori su linearno zavisni")
    exit(1)
print("Gramova determinanta: " + gramova_determinanta)
print("Uneseni vektori su linearno nezavisni")

#vektori = vector_operator.unesi_vektore_iz_fajla(broj_vektora, velicina_baze)
start = datetime.now()
ortogonalizirani_vektori = vector_operator.gram_schmidtov_postupak_ortogonalizacije(
        vektori)
print("Ortogonalizirani vektori: ")
vector_operator.ispisi_vektore(ortogonalizirani_vektori)
normirani_vektori = vector_operator.normiraj_vektor(
        ortogonalizirani_vektori)
print("Ortonormirani vektori: ")
vector_operator.ispisi_vektore(normirani_vektori)
end = datetime.now()
razlika = end-start
time_diff = razlika.total_seconds()*1000
print("Vrijeme izvodenja programa: " + str(time_diff) + "[ms]")

