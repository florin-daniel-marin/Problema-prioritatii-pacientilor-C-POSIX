# Problema-prioritatii-pacientilor-C-POSIX
```
Materia: Programarea Aplicațiilor în Timp Real
Aprilie, Sem. 2, 2022
```
# Problema prioritatii pacientilor

![alt text](https://github.com/florin-daniel-marin/Problema-prioritatii-pacientilor-C-POSIX/blob/main/images/Logo_Universitatea_Politehnica_din_Bucure%C8%99ti.svg.png)

## - Problema de sincronizare -

Nume Prenume Grupa Adrese de e-mail

Stoian Alexandru-Gabriel 333AB alexandru.stoian01@stud.acs.upb.ro

Marin Florin Daniel 333AB florindaniel.marin@stud.acs.upb.ro

Stamule Theo 333AB theo.stamule@stud.acs.upb.ro


## Contents

- 1 Introducere. Definire problemă
- 2 Analiza problemei
- 3 Concepte de programare in timp real
- 4 Implementarea soluției
- 5 Codul programului
- 6 Testarea aplicației
- 7 Concluzii
- 8 Bibliografie


## 1 Introducere. Definire problemă


La o unitate de urgenta care accepta 10 pacienti pe zi, vin aleatoriu trei tipuri de pacienti la momente din zi aleatorii: cu prioritate medicala grava, cu prioritate medicala medie, cu prioritate medicala redusa. Conducerea spitalului a hotarat ca pacientii sa intre la consultatie in functie de prioritatea nevoii medicale.

Problema se rezolva in real time programming unde 1s = 10 min din viata reala folosind operatii de sincronizare.

Definire problem ̆a

```
 Task G - Preluare in cabinet pacient cu prioritate medicalaGrava
```
```
 Task M - Preluare in cabinet pacient cu prioritate medicalaMedie
```
```
 Task R - Preluare in cabinet pacient cu prioritate medicalaRedusa
```
```
 Task addQueue - Populare coada de asteptare cu pacienti in stare Grava , Medie si Redusa
la momente aleatorii de timp
```
```
 Task monitorizare - Decide ce pacient va intra in cabinet cand acesta se elibereaza
```
Constrangeri

1. Maxim 10 de pacienti in sala de asteptare
2. Pacientul cu prioritate grava intra in cabinet daca cabinetul este gol
3. Pacientul cu prioritate medie intra in cabinet daca:
```
 -  cabinetul este gol
 - nu exista pacient cu prioritate grava in coada de asteptare
```
4. Pacientul cu prioritate redus intra in cabinet daca:
```
 - cabinetul este gol
 - nu exista pacient cu prioritate grava in coada de asteptare
 - nu exista pacient cu prioritate medie in coada de asteptare
```
5. Timpul de consultare al pacientului cu prioritate grava este intre 60-120 min
6. Timpul de consultare al pacientului cu prioritate medie este intre 30-60 min
7. Timpul de consultare al pacientului cu prioritate redusa este intre 10-30 min

## 2 Analiza problemei

Solutia problemei pe larg

Noi ne-am gandit la implementarea unei solutii care nu inregistreaza datele pacientilor intr-o structura reala, iar coada de asteptare (sala) definita ulterior va fi o coada abstracta, pur teoretica. Toate datele despre coada sunt cunoscute din valoriile unor semafoare. Vom defini cate-un semafor pentru fiecare tip de pacient, un semafor pentru semnalarea locurilor libere si ocupate din coada virtuala. Problema nu include constrangeri despre identificarea pacientilor sau despre intrarea lor in cabinet dupa timpul in care au ajuns in sala, deci cerinta problemei ramane doar accesul corect la cabinet a tipurilor de pacienti dupa prioritatea lor, identitatea
lor nefiind importanta pentru problema data, ci doar tipul de prioritate medicala: grava, medie sau redusa.

Dificultatea problemei sta in atribuirea corecta a resursei critice, care este accesul in cabinet, pacientilor cu prioritatea mare, indiferent de timpul in care au ajuns in coada de asteptare. Prioritizarea pacientilor este, deci, nucleul de interes pentru acest tip de problema, deci o solutie pentru sincronizarea pacientilor trebuie gasita si utilizata. Solutia noastra pentru sincronizare este de a cauta si a decide intr-un timp de maxim 5 minute tipul pacientului care va intra in cabinet, in momentul in care carbinetul se deschide.

```
Solutia problemei este: politetea pacientilor, 
```
prin asteptarea pacientilor cu nevoi medicale mai mari decat ei sa ocupe cabinetul inaintea lor. Pacientii cu prioritate grava intra in cabinet in momentul in care acesta se deschide. Pacientii cu prioritate medie asteapta aprox. 2 minute ca cei dintai sa intre. La expirarea timpului de politete, intra ei in cabinet. In sfarsit, pacientii cu prioritatea redusa, asteapta 4 minute, iar daca altii cu nevoie mai mare decat ei nu au intrat, atunci vor intra ei in cabinet. In program, acest algoritm poarta numele de Monitorizare, iar in descrierea solutiei de mai sus, procedeul poate fi realizat de constiinta fiecarui pacient, sau mai realistic vorbind, de un medic sau asistent, care de fiecare data cand un pacient iese din cabinet, cauta si alege din sala pe cei cu nevoie medicala urgenta, in favoarea celor ce asteapta de mai mult timp la coada.

```
Am identificat o posibila solutie si am realizat o paralela intre situatiile reale posibile si
```
programul gandit de noi spre rezolvarea problemei. Cu siguranta exista metode mai eficiente d.p.d.v al timpului; suntem curiosi si deschisi spre orice metoda mai eficienta prin metode Real Time Programming. In continuare, numim aceasta solutie de sincronizare: ”Politetea pacientilor”.

Problema se aseamana problemei tipice Producator-Consumator, unde producatorul este task-ul addQueue, care adauga in coada de asteptare pacienti, consumatorul este task-ul monitorizare, care decide ce pacient are acces la resursa critica - cabinetul, iar buffer-ul sau queue este coada de asteptare care nu exista prin sine, nefiind creata, si exista prin existenta task-urilor G, M si R. Coada - G, M, R sunt ”shared resource” intre producator-pacienti si consumer-cabinet.Aceasta paralela intre concepte a usurat foarte mult implementarea programului. Problema fiind, dupa parerea noastra, un caz particular al problemei Producator-Consumator, cu con-strangerea prioritatii diferite a thread-urilor.


Consideratii pentru implementare

- problema de tipul RTOS environment
- timpul de 10 minute inseamna o secunda in program
- asemanatoare problemei tipice Producer-Cosumer (producer-pacienti/consumer-cabinet//buffer-
    queue)
- implementam sincronizarea cu semafoare, mutexuri si bariere
- toate taskurile se executa periodic
- 5 task-uri: G, M, R, addQueue, monitorizare; G are prioritatea cea mai mare, iar R cea
    mai redusa
- 5 thread-uri:
    - P - pacienti : addQueue
    - C - cabient : G, M, R
    - M - monitorizare
- resursa comuna: Q - queue (coada de asteptare)

## 3 Concepte de programare in timp real

### Concepte teoretice
Conceptele prezentate la curs pe care le folosim:
```
 sincronizare
```
```
 excludere mutuala
```
```
 planificarea pacientilor pe condițe de timp
```
### Structuri folosite
Utilizam urmatoarele obiecte din teoria programarii in timp real si din categoria de sincronizare
a thread-urilor:

 1. 2 mutexuri: mutexCabinet (echivalent semafor binar) , mutexQueue


 2. 5 semafoare generalizate:
- sG :
    * >0 - in functie de cati G sunt in queue
    * 0 - niciun G in queue
- sM :
    * >0 - in functie de cati M sunt in queue
    * 0 - niciun M in queue
- sR :
    * >0 - in functie de cati R sunt in queue
    * 0 - niciun R in queue
    
- occupiedSeats - numara cate locuri ocupate sunt in sala de asteptare
- emptySeats - numara cate locuri libere sunt ramase in sala de asteptare

 3. 3 semafoare binare:
- Monitor - semaforul activeaza monitorizarea pacientilor dupa eliberarea cabinetului
- barrierM - blocheaza sau permite accesul pacientilor M
- barrierR - blocheaza sau permite accesul pacientilor R

 4. variabile globale:
- isG :
    * 1 - sunt G sunt in sala
    * 0 - niciun G in sala
- isM :
    * 1 - sunt M sunt in sala
    * 0 - niciun M in sala
- isBarrier - bariera aceasta se activeaza cand una din barierele M sau R sunt active

 5. valori initiale:
- sG, sM, sR, occupiedSeats, isG, isM, barrierM, barrierR, isBarrier = 0;
- emptySeats = 10;
- Monitor = 1;


## 4 Implementarea soluției

### Explicare pas cu pas a functiilor programului

1. Task addQueue - Adaug pacienti G, M, R in coada de asteptare

```
creez un numar random de prioritate: 1,2 sau 3
astept loc disponibil in coada de asteptare
blochez accesul la coada de asteptare pentru a adauga un nou pacient
astept un timp random pt a veni un nou pacient
```
2. Task G - Preluare in Cabinet pacient cu prioritate medicala grava:

```
este G in cabinet =¿ Nu!
verific daca sunt pacienti G in coada de asteptare si decrementez dc exista
este G in cabinet =¿ Da!
verific daca cabinetul este liber si se ocupa cu pacientul G
 ——Shared resource——
 consultare pacient G
cabinetul devine liber pentru orice alt pacient
incrementez numarul de scaune libere din coada de asteptare
cabinetul devine liber pentru orice alt pacient =¿ monitorizare (aleg alt pacient)
```
3. Task M - Preluare in Cabinet pacient cu prioritate medicala medie:

```
este M in cabinet =¿ Nu!
verific daca sunt M in coada de asteptare si decrementez dc exista
verific daca nu sunt G in coada de asteptare prin barrierM
este M in cabinet =¿ Da!
verific daca cabinetul este liber si se ocupa cu pacientul M
 ——Shared resource——
 consultare pacient M
cabinetul devine liber pentru orice alt pacient
incrementez numarul de scaune libere din coada de asteptare
cabinetul devine liber pentru orice alt pacient =¿ monitorizare (aleg alt pacient)
```
4. Task R - Preluare in Cabinet pacient cu prioritate medicala redusa:

```
verific daca sunt R in coada de asteptare si decrementez dc exista
pacientul R asteapta 5 minute din politete pentru cei gravi
verific daca nu sunt G sau M in coada de asteptare (barrierR)
verific daca cabinetul este liber si se ocupa cu pacientul R
 ——Shared resource——
 consultare pacient R
cabinetul devine liber pentru orice alt pacient
incrementez numarul de scaune libere din coada de asteptare
cabinetul devine liber pentru orice alt pacient =¿ monitorizare (aleg alt pacient)
```
5. Task Monitorizare - Decide intr-un timp de 3 minute(0.3 secunde) ce pacient va intra in cabinet

```
se declanseaza cand cabinetul devine gol
daca dupa 0.15 secunde nu a intrat nici-un G in salon, adica isG este 0 atunci M poate sa intre
daca dupa inca 0.15 sec nu intra niciun M, atunci intra R
numara cati pacienti au intrat in cabinet
conditia de iesire: au intrat 10 pacienti
```

### Organigrama

![alt text](https://github.com/florin-daniel-marin/Problema-prioritatii-pacientilor-C-POSIX/blob/main/images/Untitled.jpg)
Figure 1: Solut ̧ie implementare - organigrama taskuri  
![alt text](https://github.com/florin-daniel-marin/Problema-prioritatii-pacientilor-C-POSIX/blob/main/images/Untitled%20(1).jpg)
Figure 2: Solut ̧ie implementare - organigrama taskuri


## 5 Codul programului
Codul programului este valabil in fișierul Cod  

## 6 Testarea aplicației
Testari reusite ale codului în fișierul Teste: data_1.txt, ..., data_5.txt


## 7 Erori identificate si functionalitate malitioasa
```
 Deadlocks : nu sunt identificate;
```
```
 Race conditions : sunt prezente in cadrul resursei comune: adica a cozii de asteptare: G,
M, R ; de aici rezulta in unele teste comportament nedefinit;
```
```
 Erori de sincronizare : provin de la functia sleep, de la race conditions, de la incetinirea
CPU, sau problema de arhitectura. In orice caz, apar foarte rar, si incurca randul solutiei
bune, cu maxim 2 sau 3 valori din 10, cate are o executare.
```
```
 Functionalitate malitioasa : din mai mult de 50 de teste facut in 5-10 teste am descoperit
o comportare nedorita, de tipul erorilor de sincronizare discutate mai sus.
```
In continuare sunt prezentate aceste tipuri de erori de sincronizare, care apar rar, impreuna cu printarea valorilor variabilelor bariera pentru debugging.

 * Eroare1 (în fișierul teste: test_erroare_1.txt)

Solutia asteptata: M,G,R,R,R,R,G,G,M,R. Solutia calculata de program: M,G,R,R,R,R,G,M,G,R

 * Eroare1 rezolvata:
Pe un caz asemanator, cu modificarea codului programului, eroarea aseamanatoare cazului anterior, nu a mai aparut.  
(vezi test_erroare_1_rezolvata.txt)

 * Eroare2 (în fișierul teste: test_erroare_2.txt)
Eroare de sincronizare asemanatoare primei erorii, nerezolvate. Solutia buna era R,M,M,G,G,G,G,R,R,R, solutia gasita de program: R,M,M,G,R,G,G,G,R,R.

## Validarea solutiei propuse

Verificare solutie 1:

Va prezentam doua solutii corecte gasite de program, pentru valori de intrare random si ”mai grele” decat majoritatea valorilor de intrare random.
(Vezi verificare_solutie, verificare_solutie_2, verificare_finala din teste)


### Metode verificare solutii

Pentru analiza solutiei, trebuie tinut cont de momentul de timp cand intra pacientii in coada de asteptare si de momentul de timp cand cabinetul se elibereaza. Se poate urmari la executarea programului sau cu ajutorul unui timeline chart de tipul celui de mai jos sau de tipul Gantt chart.

![alt text](https://github.com/florin-daniel-marin/Problema-prioritatii-pacientilor-C-POSIX/blob/main/images/verificare.jpg)
```
Figure 3: Timeline chart Verificare solutie 1
```
```
Solutie asteptata: MMMGGGMMRR
Solutia verifica problema.
```

## 7 Concluzii


Pentru Problema Prioritatii Pacientilor am gasit o solutie de sincronizare, pe care am denumit-o ”Politetea pacientilor”. Solutia este de tipul programarii in timp real, si utilizeaza concepte de RTOS, sincronizare si excludere mutuala. Noi am considerat problema un caz particular a problemei arhetip in sincronizare: ”Producer-Consumer”, prin existenta prioritatii diferite a thread-urilor, care a reprezentat si cea mai mare dificultate a problemei.

Rezultatul programului implementat este de cele mai multe ori corect, cu greseli ce apar rar si sunt incluse in categoria eroriilor de sincronizare. Fiind singurul tip de erori intalnite la testare, pe un numar de aproximativ 50 de executari, acestea pot aparea datorita greselilor de sincronizare a thread-urilor, incetinirii CPU, race conditions sau arhitecturii programului.


Calculand eroarea rezultatului, raportata la numarul de valori, din cele 50 de teste: numarul de valori gresite din 500 de valori, este cuprins intre 20-30. Deci probabilitatea ca o valoare din sirul de valori dintr-o executare sa fie eronata este de 4%, iar numarul de teste
gresite din 50, este 10%.


In concluzie, 90% din teste rezolva corect problema, in timp ce 96% din valorile returnate coincid cu valoriile solutiei problemei.


## 8 Bibliografie

https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem

https://code-vault.net/lesson/18ec1942c2da46840693efe9b520f873

https://www.geeksforgeeks.org/use-posix-semaphores-c/

https://linuxhint.com/https-linuxhint-com-posix-semaphores-with-c-programming/

https://greenteapress.com/wp/semaphores/
