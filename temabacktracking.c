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
    int** grid;
    int** is_black;
    int** best_is_black;
    int found;
} HitoriContext;

void dfs(int r, int c, int n, int** is_black, int** visited){
    if(r<0 || c<0 || r>=n || c>=n) return;
    if(is_black[r][c] || visited[r][c]) return;
    visited[r][c]=1;
    dfs(r+1, c, n, is_black, visited);
    dfs(r-1, c, n, is_black, visited);
    dfs(r, c+1, n, is_black, visited);
    dfs(r, c-1, n, is_black, visited);
}

int check_connectivity(int n, int** is_black){
    int** visited=(int**)malloc(n * sizeof(int*));
    for(int i=0; i<n; i++) visited[i]=(int*)calloc(n, sizeof(int));
    int start_r=-1, start_c=-1;
    int white_count=0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(!is_black[i][j]){
                if(start_r==-1){ start_r=i; start_c=j; }
                white_count++;
            }
        }
    }
    if(white_count==0){
        for(int i=0; i<n; i++) free(visited[i]);
        free(visited);
        return 0;
    }
    
    dfs(start_r, start_c, n, is_black, visited);
    int visited_count=0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(visited[i][j]) visited_count++;
        }
    }
    
    for(int i=0; i<n; i++) free(visited[i]);
    free(visited);
    return visited_count==white_count;
}

int check_no_duplicates(int n, int** grid, int** is_black){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if (is_black[i][j]) continue;
            for(int k=j+1; k<n; k++){
                if(!is_black[i][k] && grid[i][j]==grid[i][k]) return 0;
            }
            for(int k=i+1; k<n; k++){
                if(!is_black[k][j] && grid[i][j]==grid[k][j]) return 0;
            }
        }
    }
    return 1;
}

void backtrack(int r, int c, HitoriContext* ctx){
    if(ctx->found) return;
    if(r==ctx->n){
        if(check_no_duplicates(ctx->n, ctx->grid, ctx->is_black) && check_connectivity(ctx->n, ctx->is_black)){  
            ctx->found=1;
            for(int i=0; i < ctx->n; i++)
                for(int j=0; j < ctx->n; j++)
                    ctx->best_is_black[i][j] = ctx->is_black[i][j];
        }
        return;
    }
    
    int next_r = r + (c+1) / ctx->n;
    int next_c = (c+1) % ctx->n;
    ctx->is_black[r][c]=0;
    backtrack(next_r, next_c, ctx);

    int can_be_black=1;
    if(r>0 && ctx->is_black[r-1][c]==1) can_be_black=0;
    if(c>0 && ctx->is_black[r][c-1]==1) can_be_black=0;
    if(!ctx->found && can_be_black){
        ctx->is_black[r][c]=1;
        backtrack(next_r, next_c, ctx);
        ctx->is_black[r][c]=0;
    }
}

int solve_hitori(const char* in_filename, const char* out_filename){
    FILE* fin=fopen(in_filename, "r");
    if(!fin) return -1;
    HitoriContext ctx;
    ctx.found=0;
    fscanf(fin, "%d", &ctx.n);
    ctx.grid=(int**)malloc(ctx.n * sizeof(int*));
    ctx.is_black=(int**)malloc(ctx.n * sizeof(int*));
    ctx.best_is_black=(int**)malloc(ctx.n * sizeof(int*));
    for(int i=0; i<ctx.n; i++){
        ctx.grid[i]=(int*)malloc(ctx.n * sizeof(int));
        ctx.is_black[i]=(int*)calloc(ctx.n, sizeof(int));
        ctx.best_is_black[i]=(int*)calloc(ctx.n, sizeof(int));
        for(int j=0; j<ctx.n; j++){
            fscanf(fin, "%d", &ctx.grid[i][j]);
        }
    }
    fclose(fin);
    backtrack(0, 0, &ctx);
    FILE* fout=fopen(out_filename, "w");
    int moves=0;
    if(ctx.found){
        for(int i=0; i<ctx.n; i++){
            for(int j=0; j<ctx.n; j++){
                if(ctx.best_is_black[i][j]){
                    fprintf(fout, "B ");
                    moves++;
                }else{
                    fprintf(fout, "%d ", ctx.grid[i][j]);
                }
            }
            fprintf(fout, "\n");
        }
        fprintf(fout, "Numar mutari (celule negre): %d\n", moves);
    } else {
        fprintf(fout, "Nu exista solutie.\n");
        moves = -1;
    }
    fclose(fout);
    for(int i=0; i<ctx.n; i++){
        free(ctx.grid[i]);
        free(ctx.is_black[i]);
        free(ctx.best_is_black[i]);
    }
    free(ctx.grid);
    free(ctx.is_black);
    free(ctx.best_is_black);
    return moves;
}

void create_test_file(const char* filename, int n, int* values) {
    FILE* f=fopen(filename, "w");
    fprintf(f, "%d\n", n);
    for(int i=0; i<n*n; i++){
        fprintf(f, "%d ", values[i]);
        if((i+1) % n == 0) fprintf(f, "\n");
    }
    fclose(f);
}

int main() {
    //Test1: Matrice 2x2 fara duplicate : Nicio mutare necesara
    int test1_data[]={1, 2, 2, 1};
    
    //Test2: Matrice 3x3 cu un singur set de duplicate (trebuie eliminat un 1)
    int test2_data[]={1, 2, 3, 1, 4, 5, 6, 7, 8};
    
    //Test3: Matrice 3x3 unde un numar este izolat
    int test3_data[]={1, 1, 2, 2, 3, 3, 4, 5, 6};
    create_test_file("input1.txt", 2, test1_data);
    create_test_file("input2.txt", 3, test2_data);
    create_test_file("input3.txt", 3, test3_data);

    int result1=solve_hitori("input1.txt", "output1.txt");
    assert(result1==0);
    printf("Test 1 trecut cu succes, 0 mutari confirmate.\n");

    int result2=solve_hitori("input2.txt", "output2.txt");
    assert(result2==1);
    printf("Test 2 trecut cu succes, 1 mutare confirmata.\n");

    int result3=solve_hitori("input3.txt", "output3.txt");
    assert(result3>=2);
    printf("Test 3 trecut cu succes, %d mutari confirmate (minim 2).\n", result3);
    printf("\nToate cele 3 verificari assert au trecut conform regulamentului! \n");
    return 0;
}
