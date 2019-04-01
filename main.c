#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX 10000

struct calculatoare{
	char* name;
	char* IP;
	int state;
	int router_index;
	int pc_index;
	struct calculatoare *next;
};

struct routers{
	char* name;
	char* IP;
	int nr_pc;
	struct calculatoare *list;
	int index;
};

typedef struct calculatoare PC;
typedef struct routers Router;

typedef struct {
	int V; 
	int E; 
	int** a; 
}Graph;


Router* readRouters(FILE* f){
	int n, i, nr, state, j;
	char name[15];
	char IP[15];
	
	fscanf(f, "%d", &n);
	Router* array = (Router*)malloc(n*sizeof(Router));
	for(i = 0; i < n; i++){
		array[i].name = (char*)malloc(15*sizeof(char));
		fscanf(f, "%s", name);
		strcpy(array[i].name, name);
		
		array[i].IP = (char*)malloc(15*sizeof(char));
		fscanf(f, "%s", IP);
		strcpy(array[i].IP, IP);
		
		array[i].index = i;

		PC* head = NULL;
		PC* last;
		array[i].list = (PC*)malloc(nr*sizeof(PC));
		for(j = 0; j < nr; j++){

			fscanf(f, "%d", &nr);
			array[i].nr_pc = nr;
		
			fscanf(f, "%s", name);
			fscanf(f, "%s", IP);
			fscanf(f, "%d", &state);
			
			PC* newNode = (PC*)malloc(sizeof(PC));
			newNode->name = (char*)malloc(15*sizeof(char));
			strcpy(newNode->name, name);
			newNode->IP = (char*)malloc(15*sizeof(char));
			strcpy(newNode->IP, IP);
			newNode->state = state;
			newNode->router_index = i;
			newNode->pc_index = j;
			newNode->next = NULL;
			
			if(head == NULL){
				head = newNode;
			}else{
				last = head;
				while(last->next != NULL){
					last = last->next;
				}
				last->next = newNode;
			}
		}
		array[i].list = head;
	}
	
	return array;
}

Graph* buildGraph(FILE *f, int n, Router* array){
	int i, j;
	
	Graph* graf = (Graph*)malloc(sizeof(Graph));
	graf->V = n;
	int muchii;
	fscanf(f, "%d", &muchii);
	graf->E = muchii;

	graf->a = (int**)malloc(graf->V * sizeof(int*));
	
	for(i = 0; i < graf->V; i ++){
		graf->a[i] = (int*)calloc(graf->V, sizeof(int));
	}
	
	for(i = 0; i < graf->V; i++){
		for(j = 0; j < graf->V; j++){
			graf->a[i][j] = 0;
		}
	}
	
	char R1[15];
	char R2[15];
	int cost;
	int start_index;
	int end_index;

	for(i = 0; i < graf->E; i++){
		fscanf(f, "%s%s%d", R1, R2, &cost);
		for(j = 0; j < graf->V; j++){
			if(strcmp(R1, array[j].name) == 0){
				start_index = j;
			}
			if(strcmp(R2, array[j].name) == 0){
				end_index = j;
			}
		}
		
		//matricea de adiacenta e simetrica!!
		graf->a[start_index][end_index] = cost;	
		graf->a[end_index][start_index] = cost;
	}
	
	return graf;
}


void up(char* IP, Router* array, Graph* graf){
	int i;
	PC* node;
	//cautare IP si setare stare la 1
	for(i = 0; i < graf->V; i ++){
		node = array[i].list;
		while(node != NULL){
			if(strcmp(IP, node->IP) == 0){
				node->state = 1;
			}
			node = node->next;
		}
	}		
}

void broke(char* IP, Router* array, Graph* graf){
	int i;
	PC* node;
	//cautare IP si setare stare la 0
	for(i = 0; i < graf->V; i ++){
		node = array[i].list;
		while(node != NULL){
			if(strcmp(IP, node->IP) == 0){
				node->state = 0;
			}
			node = node->next;
		}
	}		
}

void sipn(FILE* f, char *IP, Router* array, Graph* graf){
	//cautare IP si verificare conexiune Router
	int i, j;
	for(i = 0; i < graf->V; i++){
		if(strcmp(IP, array[i].IP) == 0){
			for(j = 0; j < graf->V; j++){
				if(graf->a[i][j] != 0 || graf->a[j][i] != 0){
					fprintf(f, "%s ", array[j].IP);
				}
			}
		}
	}
	fprintf(f, "\n");
}

void si(FILE *f, char *IP, Router* array, Graph* graf){
	//cautare IP Router si afisare pc-uri conectate
	int i;
	PC* node;
	for(i = 0; i < graf->V; i++){
		if(strcmp(IP, array[i].IP) == 0){
			node = array[i].list;
			while(node != NULL){
				if(node->state == 1){
					fprintf(f, "%s ", node->IP);
				}
				node = node->next;
			}
		}
	}
	fprintf(f, "\n");
}
	
void lc(char *IP1, char *IP2, Router* array, Graph* graf){
	int i, start_index, end_index;
	//cautare IP-uri sursa -> destinatie
	for(i = 0; i < graf->V; i++){
		if(strcmp(IP1, array[i].IP) == 0){
			start_index = i;
		}
		if(strcmp(IP2, array[i].IP) == 0){
			end_index = i;
		}
	}
	//stergere conexiune -> matricea de adiacenta e simetrica!!
	graf->a[start_index][end_index] = 0;
	graf->a[end_index][start_index] = 0;
	//actualizare numar muchii
	graf->E--;
}

void add(char *IP1, char *IP2, int d, Router* array, Graph* graf){
	int i, start_index, end_index;
	//cautare IP-uri sursa -> destinatie
	for(i = 0; i < graf->V; i++){
		if(strcmp(IP1, array[i].IP) == 0){
			start_index = i;
		}
		if(strcmp(IP2, array[i].IP) == 0){
			end_index = i;
		}
	}
	
	//adaugare conexiune ->matricea de adiacenta e simetrica!!
	graf->a[start_index][end_index] = d;
	graf->a[end_index][start_index] = d;
	graf->E++;

}

//functie auxiliara pt Dijkstra
int minDistance(int dist[], int sps[], int V){
	int v, min = MAX, min_index;
	for ( v = 0; v < V; v++)
		if (sps[v] == 0 && dist[v] <= min) {
			min = dist[v];
			min_index = v;
	}
	return min_index;
}

// Dijkstra care returneaza vectorul de distante minime 
int* dist_dijkstra(Graph *graf, int src){
	
	int* dist = (int*)malloc(graf->V*sizeof(int)) ;
	int sptSet[graf->V];
	
	int i, count, u, v;
		
	for(i = 0; i < graf->V; i++){
		dist[i] = MAX;
		sptSet[i] = 0;
	}
		
	dist[src] = 0;
		
	for(count = 0; count < graf->V ; count++){
		u = minDistance(dist, sptSet, graf->V);
		sptSet[u] = 1;
			
		for(v = 0; v < graf->V; v++){
			if(!sptSet[v] && graf->a[u][v] && dist[u] + graf->a[u][v] < dist[v]){
				dist[v] = dist[u] + graf->a[u][v];
			}
		}
	}
		
	return dist;
} 


void ping(FILE *f, char* IP1, char* IP2, Router* array, Graph* graf){
	
	int i, start_index, end_index;
	PC* node;
	int* dist; 
	
	//cautare Router sursa si Router destinatie
	for(i = 0; i < graf->V; i++){
		node = array[i].list;
		while(node != NULL){
			if(strcmp(IP1, node->IP) == 0 || strcmp(IP2, node->IP) == 0){
				//daca a fost gasita sursa sau destinatia, se verifica starea pc-ului
				if(node->state == 0){
					//daca pc-ul e inchis, afisam NO
					fprintf(f, "NO\n");
					return;
				}
			}
			//altfel, pastram indicele sursei si indicele destinatiei
			if(strcmp(IP1, node->IP) == 0 && node->state == 1){
				start_index = i;
			}
			if(strcmp(IP2, node->IP) == 0 && node->state == 1){
				end_index = i;
				//construim vectorul de distante minime pentru nodul destinatie
				dist = dist_dijkstra(graf, i);
			}	
			node = node->next;
		}
	}
	
	//daca exista conexiune directa (valoare diferite de 0 in matricea de adiacenta -> care e simetrica!!), afisam OK
	if(graf->a[start_index][end_index] != 0 || graf->a[end_index][start_index] != 0){
		fprintf(f, "OK\n");
		return;
	}
	//altfel verificam daca exista drum de la sursa la destinatie si afisam OK (daca nu exista drum, distanta e infinita -> adica MAX)
	if(dist[end_index] != MAX){
		fprintf(f, "OK\n");
		
	}	
}

//afisare output pt functia trace

void printPath(FILE *f, int* parent, int j, Router* array){
    if (parent[j] == -1)
        return;
	
	//afisarea recursiva a drumului
	printPath(f, parent, parent[j], array);
	fprintf(f, "%s ", array[j].IP);
		
}

/*
void printSolution(FILE* f, int* dist, int* parent, Graph* graf, Router* array){
    int src = 0;
    printf("Vertex\t Distance\tPath");
    for (int i = 1; i < graf->V; i++){
        printf( "\n%d -> %d\t %d\t %d ", src, i, dist[i], src);
        printPath(f, parent, i, array);
    }
}
*/
//Dijkstra care returneaza un vector de drum (cu noduri vizitate)
int* path_dijkstra(Graph* graf, int src){
	int dist[graf->V];
	int sptSet[graf->V];
	int* parent = (int*)calloc(graf->V, sizeof(int));
	int i, count, u, v;
		
	for(i = 0; i < graf->V; i++){
		parent[0] = -1;
		dist[i] = MAX;
		sptSet[i] = 0;
	}
		
	dist[src] = 0;
		
	for(count = 0; count < graf->V ; count++){
		u = minDistance(dist, sptSet, graf->V);
		sptSet[u] = 1;
			
		for(v = 0; v < graf->V; v++){
			if(!sptSet[v] && graf->a[u][v] && dist[u] + graf->a[u][v] < dist[v]){
				parent[v] = u;
				dist[v] = dist[u] + graf->a[u][v];
			}
		}
	}
	
	return parent;	
}

void trace(FILE *f, char* IP1, char* IP2, Router* array, Graph* graf){
	
	 
	int* parent; 
	int i, source, destination;
	PC* node;
	
	//cautare Router sursa si Router destinatie
	for(i = 0; i < graf->V; i++){
		node = array[i].list;
		while(node != NULL){
			if(strcmp(node->IP, IP1) == 0){
				source = i;
				//sursa a fost gasita, deci construim vectorul de noduri vizitate
				parent = path_dijkstra(graf, source);
			}
			if(strcmp(node->IP, IP2) == 0){
				//gasire destinatie
				destination = i;
			}
			node = node->next;
		}
	}
	

	int* dist = dist_dijkstra(graf, source);
	if(dist[destination] == MAX){
		fprintf(f, "\n");
		return;
	}
	

	fprintf(f, "%s ", array[source].IP);

	printPath(f, parent, destination, array);
	fprintf(f, "\n");
		
}


void readTasks(FILE* f_in, FILE* f_out, Router* array, Graph* graf){
	int n, d, i;
	char cerinta[10], var1[15], var2[15];
	fscanf(f_in, "%d", &n);
	for(i = 0; i < n; i++){
		fscanf(f_in, "%s", cerinta);
		if(strcmp(cerinta, "ping") == 0){
			fscanf(f_in, "%s%s", var1, var2);
			ping(f_out, var1, var2, array, graf);
		}
		if(strcmp(cerinta, "sipn") == 0){
			fscanf(f_in, "%s", var1);
			sipn(f_out, var1, array, graf);
		}
		if(strcmp(cerinta, "trace") == 0){
			fscanf(f_in, "%s%s", var1, var2);
			trace(f_out, var1, var2, array, graf);
		}
		if(strcmp(cerinta, "lc") == 0){
			fscanf(f_in, "%s%s", var1, var2);
			lc(var1, var2, array, graf);
		}
		if(strcmp(cerinta, "add") == 0){
			fscanf(f_in, "%s%s%d", var1, var2, &d);
			add(var1, var2, d, array, graf);
		}
		if(strcmp(cerinta, "si") == 0){
			fscanf(f_in, "%s", var1);
			si(f_out, var1, array, graf);
		}
		if(strcmp(cerinta, "up") == 0){
			fscanf(f_in, "%s", var1);
			up(var1, array, graf);
		}
		if(strcmp(cerinta, "broke") == 0){
			fscanf(f_in, "%s", var1);
			broke(var1, array, graf);
		}
	}
}

int main (int argc, char *argv[]){
	argc++;
	FILE* f_in = fopen(argv[1], "rt");
	FILE* f_out = fopen(argv[2], "w");
	
	int nr_routers;
	fscanf(f_in, "%d", &nr_routers);
	fclose(f_in);
	
	f_in = fopen(argv[1], "rt");
	Router* array = readRouters(f_in);
	Graph* graf = buildGraph(f_in, nr_routers, array);
	
	readTasks(f_in, f_out, array, graf);

	fclose(f_in);
}
