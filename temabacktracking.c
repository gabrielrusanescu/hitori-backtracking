/*
Grupa: 161
Problema: 11
Nume: Rusănescu Gabriel
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct{
    int n;
    int** matrice;
    int** is_black;
    int** bestis_black;
    int gasit;
} hitori;

void dfs(int l, int c, int n, int** is_black, int** vizitat){
    if(l<0 || c<0 || l>=n || c>=n) return;
    if(is_black[l][c] || vizitat[l][c]) return;
    vizitat[l][c]=1;
    dfs(l+1, c, n, is_black, vizitat);
    dfs(l-1, c, n, is_black, vizitat);
    dfs(l, c+1, n, is_black, vizitat);
    dfs(l, c-1, n, is_black, vizitat);
}

int verificconectivitate(int n, int** is_black){
    int** vizitat=(int**)malloc(n * sizeof(int*));
    for(int i=0; i<n; i++) vizitat[i]=(int*)calloc(n, sizeof(int));
    int startlinie=-1, startcol=-1;
    int cntalbe=0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(!is_black[i][j]){
                if(startlinie==-1){ startlinie=i; startcol=j; }
                cntalbe++;
            }
        }
    }
    if(cntalbe==0){
        for(int i=0; i<n; i++) free(vizitat[i]);
        free(vizitat);
        return 0;
    }
    
    dfs(startlinie, startcol, n, is_black, vizitat);
    int vizitat_cnt=0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(vizitat[i][j]) vizitat_cnt++;
        }
    }
    
    for(int i=0; i<n; i++) free(vizitat[i]);
    free(vizitat);
    return vizitat_cnt==cntalbe;
}

int faraduplicate(int n, int** matrice, int** is_black){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if (is_black[i][j]) continue;
            for(int k=j+1; k<n; k++){
                if(!is_black[i][k] && matrice[i][j]==matrice[i][k]) return 0;
            }
            for(int k=i+1; k<n; k++){
                if(!is_black[k][j] && matrice[i][j]==matrice[k][j]) return 0;
            }
        }
    }
    return 1;
}

void backtrack(int l, int c, hitori* var){
    if(var->gasit) return;
    if(l==var->n){
        if(faraduplicate(var->n, var->matrice, var->is_black) && verificconectivitate(var->n, var->is_black)){  
            var->gasit=1;
            for(int i=0; i < var->n; i++)
                for(int j=0; j < var->n; j++)
                    var->bestis_black[i][j] = var->is_black[i][j];
        }
        return;
    }
    
    int nextl = l + (c+1) / var->n;
    int nextc = (c+1) % var->n;
    var->is_black[l][c]=0;
    backtrack(nextl, nextc, var);

    int poateblack=1;
    if(l>0 && var->is_black[l-1][c]==1) poateblack=0;
    if(c>0 && var->is_black[l][c-1]==1) poateblack=0;
    if(!var->gasit && poateblack){
        var->is_black[l][c]=1;
        backtrack(nextl, nextc, var);
        var->is_black[l][c]=0;
    }
}

int solve_hitori(const char* in_filename, const char* out_filename){
    FILE* fin=fopen(in_filename, "r");
    hitori var;
    var.gasit=0;
    fscanf(fin, "%d", &var.n);
    var.matrice=(int**)malloc(var.n * sizeof(int*));
    var.is_black=(int**)malloc(var.n * sizeof(int*));
    var.bestis_black=(int**)malloc(var.n * sizeof(int*));
    for(int i=0; i<var.n; i++){
        var.matrice[i]=(int*)malloc(var.n * sizeof(int));
        var.is_black[i]=(int*)calloc(var.n, sizeof(int));
        var.bestis_black[i]=(int*)calloc(var.n, sizeof(int));
        for(int j=0; j<var.n; j++){
            fscanf(fin, "%d", &var.matrice[i][j]);
        }
    }
    fclose(fin);
    backtrack(0, 0, &var);
    FILE* fout=fopen(out_filename, "w");
    int mutari=0;
    if(var.gasit){
        for(int i=0; i<var.n; i++){
            for(int j=0; j<var.n; j++){
                if(var.bestis_black[i][j]){
                    fprintf(fout, "B ");
                    mutari++;
                }else{
                    fprintf(fout, "%d ", var.matrice[i][j]);
                }
            }
            fprintf(fout, "\n");
        }
        fprintf(fout, "Numar mutari (celule negre): %d\n", mutari);
    } else {
        fprintf(fout, "Nu exista solutie.\n");
        mutari=-1;
    }
    fclose(fout);
    for(int i=0; i<var.n; i++){
        free(var.matrice[i]);
        free(var.is_black[i]);
        free(var.bestis_black[i]);
    }
    free(var.matrice);
    free(var.is_black);
    free(var.bestis_black);
    return mutari;
}

void creeazatestfile(const char* filename, int n, int* valori) {
    FILE* f=fopen(filename, "w");
    fprintf(f, "%d\n", n);
    for(int i=0; i<n*n; i++){
        fprintf(f, "%d ", valori[i]);
        if((i+1)%n==0) fprintf(f, "\n");
    }
    fclose(f);
}

int main() {
    //Test1: Matrice 2x2 fara duplicate: Nicio mutare necesara
    int test1[]={1, 2, 2, 1};
    
    //Test2: Matrice 3x3 cu un singur set de duplicate (trebuie eliminat un 1)
    int test2[]={1, 2, 3, 1, 4, 5, 6, 7, 8};
    
    //Test3: Matrice 3x3 unde un numar este izolat
    int test3[]={1, 7, 3, 5, 6, 5, 4, 7, 5, 7, 7, 3, 2, 6, 2, 6, 4, 5, 1, 5, 5, 3, 1, 6, 2, 6, 4, 6, 7, 6, 1, 5, 4, 5, 2, 3, 4, 2, 1, 7, 6, 6, 6, 3, 5, 6, 2, 3, 1};
    creeazatestfile("input1.txt", 2, test1);
    creeazatestfile("input2.txt", 3, test2);
    creeazatestfile("input3.txt", 7, test3);

    int result1=solve_hitori("input1.txt", "output1.txt");
    assert(result1==0);
    printf("Test 1 trecut cu succes, 0 mutari confirmate.\n");

    int result2=solve_hitori("input2.txt", "output2.txt");
    assert(result2==1);
    printf("Test 2 trecut cu succes, 1 mutare confirmata.\n");

    int result3=solve_hitori("input3.txt", "output3.txt");
    assert(result3>=2);
    printf("Test 3 trecut cu succes, %d mutari confirmate\n", result3);
    printf("\nToate cele 3 verificari assert au trecut conform regulamentului! \n");
    return 0;
}
