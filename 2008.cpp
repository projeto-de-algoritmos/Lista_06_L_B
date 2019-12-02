/*
Problema 2008
Expondo a Corrupção
Por Walter Erquínigo Pezo PE Peru

O Comitê Central em Nlogônia é formado por muitos membros do congresso. Como o sistema político é dicotômico,
cada membro do congresso pertence a um dos dois partidos: o Partido Sério Mortal e o Partido Festa! Festa!. 
Por tradição, chamam-os de DSP e PPP, respectivamente.

Edward é um jornalista investigativo. Ele descobriu que os congressistas são corruptos e vai mudar de partido,
se for oferecido à ele uma determinada quantidade de Nlogmoney. Cada membro do Congresso tem o seu preço 
específico, mas todos eles têm um preço. Como de costume na política, existem rivalidades entre alguns pares 
de congressistas. Rivais nunca aceitariam estar no mesmo partido.

Edward tem um orçamento e quer usá-lo para fazer alguns congressistas mudarem de partido e assim coletar provas 
irrefutáveis ​​para sua investigação. Ao fazer isso, ele tem que respeitar rivalidades: depois que todos os políticos
envolvidos aceitarem sua proposta, os rivais devem ser colocados em diferentes partidos.

Edward quer causar o máximo impacto. Você pode ajudá-lo a descobrir o número máximo de congressistas que podem
pertencer a DSP se ele usar no máximo todo o seu orçamento para esse objetivo? Da mesma forma, o que é o número
máximo de membros do Congresso que pode pertencer a PPP sob o mesmo restrições?

Entrada
A entrada contém vários casos de teste; cada caso de teste é formatado como segue.
A primeira linha contém quatro inteiros D, P, R e B, que representam, respectivamente, o número de congressistas
que, inicialmente, pertencem a DSP (1 ≤ D ≤ 100), o número de congressistas que, inicialmente, pertencem a
PPP (1 ≤ P ≤ 100), o número de rivalidades entre os membros do congresso (1 ≤ R ≤ 2,000), e o orçamento do 
jornalista expresso em Nlogmoney (1 ≤ B ≤ 104). Os membros do DSP são identificados com números inteiros de 1 a
distintas D, enquanto os membros do PPP são identificados com inteiros distintos de 1 a P. A segunda linha contém D 
inteiros S1, S2, ..., SD, indicando que i membro de DSP mudará de partido se oferecido Si Nlogmoney (1 ≤ Si ≤ 100
para i = 1, 2, ..., D). A terceira linha contém inteiros P T1, T2, ..., TP, indicando que j membros de PPP vão
mudar de partido, se for oferecido Tj Nlogmoney (1 ≤ Tj ≤ 100 para j = 1, 2, ..., P). Cada uma das próximas R 
linhas descreve uma rivalidade com dois inteiros X e Y, onde X representa o membro de DSP e Y o de PPP que são 
rivais (1 ≤ X ≤ D e 1 ≤ Y ≤ P).

Saída
Para cada caso de teste na entrada, a saída deve conter dois números inteiros representando o número máximo de
congressistas que podem pertencer a DSP usando o orçamento dado e, da mesma forma, o número máximo dos congressistas
que pode pertencer a PPP usando o orçamento dado.
*/



#include <iostream>

#define MAXN 205
#define MAXB 10005

#define TRACE(x...) x
#define PRINT(x...) TRACE(printf(x))
#define WATCH(x) TRACE(cout << #x " = " << x << endl;)
#define MAX(a,b) ((a) > (b) ? (a) : (b))

using namespace std;

int D, P, R, B;
int DSP[MAXN], PPP[MAXN];

int adj[MAXN][MAXN], degree[MAXN];
int component[MAXN];
int N;

int memo[MAXN][MAXB];

typedef struct {
    int dsp, ppp, cost;
} t_comp_prop;

t_comp_prop prop[MAXN];

void dfs(int s, int id){
    int t;
    component[s] = id;
    for(int i=0; i<degree[s]; i++){
        t = adj[s][i];
        if(component[t] < 0)
            dfs(t,id);
    }
}

int knapsack(int w, int i, int direction){
    if(w == 0 || i == 0){
        if(direction == 0)
            return D;
        return P;
    }

    if(memo[i-1][w] > 0)
       return memo[i-1][w];
       
    if(prop[i-1].cost > w)
       return memo[i-1][w] = knapsack(w, i-1, direction);
    
    if(direction == 0){
        return memo[i-1][w] = MAX(
            knapsack(w, i-1, direction),
            (prop[i-1].ppp - prop[i-1].dsp) + knapsack(w - prop[i-1].cost, i - 1, direction)
        );
    } else {
        return memo[i-1][w] = MAX(
            knapsack(w, i-1, direction),
            (prop[i-1].dsp - prop[i-1].ppp)  + knapsack(w - prop[i-1].cost, i - 1, direction)
        );
    }
}


int main(){

    int x, y, id, cont = 1;
    //D = numero de congressistas DSP
    //P = numero de congressistas PPP
    //R = numero de rivalidade entre membros
    //B = orçamento 
    while(printf("congressistas DSP, ongressistas PPP, rivalidade, orçamento\n"),scanf("%d %d %d %d", &D, &P, &R, &B) != EOF){
        N = P + D;

        for(int i=0; i<MAXN; i++){
            degree[i] = 0;
            component[i] = -1;
        }
        
        printf("Qtd membros em DSP , minimo suborno\n");

        for(int i=1; i<=D; i++)
            scanf("%d", &DSP[i]);

        printf("Qtd membros em PPP , minimo suborno\n");

        for(int i=D+1; i<=P+D; i++)
            scanf("%d", &PPP[i]);
        printf("Rivalidade\n");
        for(int i=0; i<R; i++){
            scanf("%d %d", &x, &y);
            y = y + D;
            adj[x][degree[x]++] = y;
            adj[y][degree[y]++] = x;
        }

        id = 1;
        for(int i=1; i<=N; i++){
            if(component[i] < 0){
                dfs(i, id);
                id++;
            }
        }

        for(int i=1; i<id; i++){
            int count_dsp = 0, count_ppp = 0, cost = 0;

            for(int j=1; j<=N; j++){
                if(component[j] == i){
                    if(j <= D){
                        count_dsp++;
                        cost += DSP[j];
                    } else {
                        count_ppp++;
                        cost += PPP[j];
                    }
                }
            }
            prop[i].dsp = count_dsp;
            prop[i].ppp = count_ppp;
            prop[i].cost = cost;
        }

        for(int i=0; i<MAXN; i++)
            for(int j=0; j<MAXB; j++)
                memo[i][j] = -1;                   
        printf("%d ", knapsack(B, id, 0));

        for(int i=0; i<MAXN; i++)
            for(int j=0; j<MAXB; j++)
                memo[i][j] = -1;
    
        printf("%d\n", knapsack(B, id, 1));
        printf("Se deseja continuar digit 1, caso contrario digite 0");
        scanf("%d", &cont);
        if (cont == 0)
            break;

    }

}