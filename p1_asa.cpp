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
    int parametrizacao;
    int left_celula;
    int left_index;
    int down_celula;
    int down_index;
    bool operator==(const Result& other) const {
        return value == other.value;
    }
};

// Função para calcular a operação usando a tabela
int op(int a, int b, const vector<vector<int>>& table) {
    return table[a - 1][b - 1]; // Ajuste de índices (1-base para 0-base)
}

void printTrace(const Result& res, const vector<vector<vector<Result>>>& dp, int i, int j) {
    if (res.parametrizacao == -1) {
        cout << res.value;
        return;
    }
    cout << "(";
    printTrace(dp[i][res.left_celula][res.left_index], dp, i, res.left_celula);
    cout << " ";
    printTrace(dp[res.down_celula][j][res.down_index], dp, res.down_celula, j);
    cout << ")";
}




void printMatrix(const vector<vector<vector<Result>>>& dp) {
    int m = dp.size(); // Number of rows/columns in the matrix

    cout << "Matrix (diagonal to top, properly aligned):" << endl;

    for (int i = 0; i < m; i++) { // Iterate over rows
        cout << string(i * 4, ' '); // Add spaces for indentation (4 spaces per row)

        for (int j = i; j < m; j++) { // Iterate from the diagonal (i == j) to the top (j > i)
            if (dp[i][j].empty()) {
                cout << "[] "; // Empty vector
            } else {
                cout << "[";
                for (const auto& res : dp[i][j]) {
                        cout << res.value << "->" << res.parametrizacao<< " " << res.left_celula << ":"<<res.left_index<< " "<< res.down_celula << ":"<<res.down_index<< " |||"; 
                    
                }
                cout << "] ";
            }
        }
        cout << endl; // New row
    }
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



void solve(int n, int m, const vector<vector<int>>& table, const vector<int>& sequence, int desired_result) {

    vector<vector<vector<Result>>> dp(m, vector<vector<Result>>(m));

    for (int i = 0; i < m; i++) {
        dp[i][i].push_back({sequence[i], -1, -1, -1,-1,-1});
    }

    for (int i = 0; i < m - 1; i++) {
        int value = op(sequence[i], sequence[i + 1], table);
        dp[i][i + 1].push_back(
            {value, i, i, 0, i+1,0});
    }
// Primeiros 2 for's sao essenciais sendo que definem cada célula a
for (int diagonal = 2; diagonal < m; diagonal++) { 
    for (int i = 0; i < m - diagonal; i++) { 
        int j = i + diagonal;
        int count = 0;
        //cout << "Para a célula: " << "[" << i << "]["<< j << "]" << endl;               
        for (int c = j-1; c >= i; c--) {  

            int sub_lista1 = dp[i][c].size();       
            int sub_lista2 = dp[c + 1][j].size(); 
            
             
            //cout << "Utilizamos as células: " << "[" << i << "]["<< c << "]" << endl;
            //cout << "Utilizamos as células: " << "[" << c+1 << "]["<< j << "]" << endl;
            

            
            for (int left = 0; left < sub_lista1; left++) {
                for (int down = 0; down < sub_lista2; down++) {
                
                    int value = op(dp[i][c][left].value, dp[c + 1][j][down].value, table);
                     
                    //cout <<"Combinamos com os seguintes valores: " << dp[i][c][left].value << "+" << dp[c + 1][j][down].value << "="<< value << endl;
                    if(find(dp[i][j].begin(), dp[i][j].end(), value) != dp[i][j].end()){
                        //cout << "Foi colocado: " <<value <<endl;
                        dp[i][j].push_back({
                            value, 
                            dp[i][c][left].parametrizacao + dp[c + 1][j][down].parametrizacao + 1,
                            c, left,
                            c+1, down});
                            
                
                     }
                    
                     //else  {cout << "Nao foi colocado: " << value<<endl;}
                }
            }
                //cout << endl;
            
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
