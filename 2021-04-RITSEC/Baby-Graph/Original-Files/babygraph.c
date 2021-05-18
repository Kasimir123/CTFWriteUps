#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAXN 10
#define NQUIZ 5

unsigned int G[MAXN][MAXN];
unsigned int deg[MAXN];
unsigned int V;
unsigned int E;
bool bruh;

void resetGraph() {
    V = 0;
    E = 0;
    for (int i = 0; i < MAXN; i++) {
        memset(G[i], 0, sizeof(G[i]));
    }

    memset(deg, 0, sizeof(deg));
    bruh = true;
}

void generateGraph() {
    resetGraph();
    V = rand() % (MAXN - 2) + 2;
    E = rand() % ((V * (V - 1)) / 2);
    for (int e = 0; e < E; e++) {
        do {
            int u = rand() % V;
            int v = rand() % V;
            if (u != v && !G[u][v] && !G[v][u]) {
                G[u][v] = G[v][u] = 1;
                deg[u]++;
                deg[v]++;
                break;
            }
        } while (true);
    }

    for (int v = 0; v < V; v++) {
        if (deg[v] % 2) {
            bruh = false;
            break;
        }
    }
}

void printGraph() {
    puts("Here is your baby graph");
    printf("V = %u - E = %u\n", V, E);
    for (int v = 0; v < V; v++) {
        for (int u = v + 1; u < V; u++) {
            if (G[u][v]) {
                printf("%u %u\n", v, u);
            }
        }
    }
}

void vuln() {
    char buf[100];

    printf("Here is your prize: %p\n", system);
    fgets(buf, 400, stdin);
}

int main() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    srand(time(NULL));
    char ans;

    for (int i = 0; i < NQUIZ; i++) {
        generateGraph();
        printGraph();

        puts("Now tell me if this baby is Eulerian? (Y/N)");
        scanf("%c%*c", &ans);

        if (ans != 'Y' && ans != 'N') {
            puts("Hey! Don't do that here");
            exit(-1);
        } else {
            bool b_ans = ans == 'Y';
            if (bruh != b_ans) {
                puts("Sorry you are wrong mate.");
                exit(-1);
            }
        }
    }

    vuln();
    return 0;
}