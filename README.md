# Problema-prioritatii-pacientilor-C-POSIX

La o unitate de urgenta care accepta 10 pacienti pe zi, vin aleatoriu trei tipuri de pacienti la momente din zi aleatorii: cu prioritate medicala grava, cu prioritate medicala medie, cu prioritate medicala redusa.
Conducerea spitalului a hotarat ca pacientii sa intre la consultatie in functie de prioritatea nevoii medicale.
Problema se rezolva in real time programming unde 1s = 10 min din viata reala folosind operatii de sincronizare.  

## Definire problema

- Task G - Preluare in cabinet pacient cu prioritate medicala \textbf{Grava}
- Task M - Preluare in cabinet pacient cu prioritate medicala \textbf{Medie}
- Task R - Preluare in cabinet pacient cu prioritate medicala \textbf{Redusa}
- Task addQueue - Populare coada de asteptare cu pacienti in stare Grava , Medie si Redusa la momente aleatorii de timp
- Task monitorizare - Decide ce pacient va intra in cabinet cand acesta se elibereaza
 
## Constrangeri
- Maxim 10 de pacienti in sala de asteptare
- Pacientul cu prioritate grava intra in cabinet daca cabinetul este gol
- Pacientul cu prioritate medie intra in cabinet daca:
  - cabinetul este gol
  - nu exista pacient cu prioritate grava in coada de asteptare
- Pacientul cu prioritate redus intra in cabinet daca:
  - cabinetul este gol
  - nu exista pacient cu prioritate grava in coada de asteptare
  - nu exista pacient cu prioritate medie in coada de asteptare
- Timpul de consultare al pacientului cu prioritate grava este intre 60-120 min
- Timpul de consultare al pacientului cu prioritate medie este intre 30-60 min
- Timpul de consultare al pacientului cu prioritate redusa este intre 10-30 min

## Mai mult..
Toate detaliile despre tema sunt precizate in fisierul pdf.
