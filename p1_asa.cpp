#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <map>
#include <algorithm>


using namespace std;

// Estrutura para cada celula da matriz
struct Result {
    int value;        
    int left_celula;
    int left_index;
    int down_celula;
    int down_index;
};


// Função para calcular a operação usando a tabela
int op(int a, int b, const vector<vector<int>>& table) {
    return table[a - 1][b - 1]; // Ajuste de índices (1-base para 0-base)
}




void printTrace(const Result& res, const vector<vector<vector<Result>>>& dp, int i, int j) {
    if (res.left_celula == -1) {
        cout << res.value;
        return;
    }
    cout << "(";
    printTrace(dp[i][res.left_celula][res.left_index], dp, i, res.left_celula);
    cout << " ";
    printTrace(dp[res.down_celula][j][res.down_index], dp, res.down_celula, j);
    cout << ")";
}


bool find_in_vector(vector<Result>& results, int value){
    int len = results.size();
    for (int i = 0; i < len; i++){
        if (results[i].value  == value){
            return true;
        }
    }
    return false;
}

int get_value(const vector<vector<vector<Result>>>& dp, int i, int j, Result res,const vector<vector<int>>& table){
    int value_left = dp[i][res.left_celula-1][res.left_index].value;
    int value_down = dp[i-res.down_celula+1][j][res.down_index].value;
    return op(value_left, value_down, table);
}



void solve(int n, int m, const vector<vector<int>>& table, const vector<int>& sequence, int desired_result) {

    vector<vector<vector<Result>>> dp(m, vector<vector<Result>>(m));

    for (int i = 0; i < m; i++) {
        dp[i][i].push_back({sequence[i], -1, -1,-1,-1});
    }

    for (int i = 0; i < m - 1; i++) {
        int value = op(sequence[i], sequence[i + 1], table);
        dp[i][i + 1].push_back(
            {value, i, 0, i+1,0});
    }
for (int diagonal = 2; diagonal < m; diagonal++) { // O(m)
    for (int i = 0; i < m - diagonal; i++) {  // O(m^2)
        int j = i + diagonal;
        int count = 0;             
        for (int c = j-1; c >= i; c--) {  // O(m^3)

            int sub_lista1 = dp[i][c].size(); 
            int sub_lista2 = dp[c + 1][j].size(); 
            

            if (count >= n){break;}
            
            for (int left = 0; left < sub_lista1; left++) { // O()
                if (count >= n){break;}
                for (int down = 0; down < sub_lista2; down++) { // O()
                
                    int value = op(dp[i][c][left].value, dp[c + 1][j][down].value, table);
                    if (count >= n){break;}
    
                    if(!find_in_vector(dp[i][j], value)){ // O()
                        dp[i][j].push_back({
                            value, 
                            c, left,
                            c+1, down});
                            count++;
                     }
                    
                }
            }
            
            
            }
            
        }
        
        
    }

    int len = dp[0][m - 1].size();
    for (int i = 0; i < len; i++){
        if (dp[0][m - 1][i].value == desired_result){
            cout << 1 << endl;
            printTrace(dp[0][m-1][i], dp, 0, m-1);
            printf("\n");
            return;
        }
    }


    cout << 0 << endl;
}





int main() {

    int n, m, desired_result;

    // Leitura do tamanho do conjunto e da sequência
    cin >> n >> m;

    // Leitura da tabela
    vector<vector<int>> table(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> table[i][j];
        }
    }

    // Leitura da sequência
    vector<int> sequence(m);
    for (int i = 0; i < m; i++) {
        cin >> sequence[i];
    }

    // Leitura do resultado desejado
    cin >> desired_result;

    // Resolver o problema


    solve(n, m, table, sequence, desired_result);

    return 0;
    
}
