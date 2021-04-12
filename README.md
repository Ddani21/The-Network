# The-Network


Structuri:

1. struct calculatoare. Memoreaza numele, ip, starea pc-ului. "Pc index" si "router index" memoreaza al catelea pc e conectat la router, respectiv la ce router e conectat.

2. struct routers. Memoreaza numele si ip-ul router-ului, numarul de pc-uri conectate la router.
Am ales sa le stochez intr-o lista numarul de pc-uri.

Structura calculatoare am redefinit-o PC, iar routers Router.

3.structGraph. Aceasta structura memoreaza numarul de noduri, numarul de muchii si matricea adiacenta a
structurii grafului.

Functii:

1.Router* readRouters(file *f)
Este o functie ce intoarce adresa unui vector de rutere creat.
Citeste numarul de rutere, introduc elementele intr-un vector de routere, inclusiv listele de 
pc-uri.

2.Graph* buildGraph(FILE*F, int n, Router* array)
Este o functie ce intoarce adresa unui graf creat.
Citesc numarul de legaturi.
Primind ca parametri numarul de elemente si vectorul de routere, creez matricea adiacenta si
initializez elementele cu 0.
Compar in acelasi timp numele router-ului de unde incepe legatura cu router-ele
din vectorul de router-e, dar si pe cel cu care se finalizeaza legatura, si
retin indicii. Cu indicii retinuti, adaug in matricea de adiacenta constul.
Matricea fiind simetrica (intrucat conexiunea routere-lor este reciproca),
fac si pentru celalalt element al matricei.

3.void up(char* IP, Router* array, Graph* graf)
Functia aceasta porneste un calculator.
Parcurg vectorul de routere, intru in fiecare lista de calculatoare si compar 
ip-ul. Odata gasit modific starea acestuia in 1.

4.void up(char* IP, Router* array, Graph* graf)
Functia aceasta opreste un calculator.
Parcurg vectorul de routere, intru in fiecare lista de calculatoare si compar 
ip-ul. Odata gasit modific starea acestuia in 0.

5.void sipn(FILE* f, char *IP, Router* array, Graph* graf)
Functia aceasta cauta IP-ul
