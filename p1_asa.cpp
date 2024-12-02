#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <map>
#include <algorithm>


using namespace std;

// Estrutura para armazenar o valor e a expressão
struct Result {
    int value;        // Valor do resultado
    string expr;      // String com a expressão gerada
    int parametrizacao; // Parametrização

    bool operator<(const Result& other) const {
        if (value != other.value) {
            return value < other.value; // valor
        }
        return parametrizacao < other.parametrizacao; // parametrizacao
    }
};


// Função para calcular a operação usando a tabela
int op(int a, int b, const vector<vector<int>>& table) {
    return table[a - 1][b - 1]; // Ajuste de índices (1-base para 0-base)
}

void filterUnique(vector<Result>& results) {
    map<int, Result> bestResults; // Maps `value` to the best `Result`

    for (const auto& res : results) {
        // If value not seen or parametrizacao is smaller, update
        if (bestResults.find(res.value) == bestResults.end() || 
            res.parametrizacao < bestResults[res.value].parametrizacao) {
            bestResults[res.value] = res;
        }
    }

    // Rebuild the results vector while preserving order
    results.clear();
    for (const auto& res : bestResults) {
        results.push_back(res.second);
    }
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
                    cout << res.expr << "|" << res.value << "|" << res.parametrizacao << " "; // Print each value
                }
                cout << "] ";
            }
        }
        cout << endl; // New row
    }
}



void solve(int n, int m, const vector<vector<int>>& table, const vector<int>& sequence, int desired_result) {

    // Matriz dinâmica para DP (cada célula contém uma lista de resultados possíveis)
    vector<vector<vector<Result>>> dp(m, vector<vector<Result>>(m));


    // Inicializar os elementos unicos da seq
    // Já agora vamos inicializar também a prox diagonal
    for (int i = 0; i < m; i++) {
        dp[i][i].push_back({sequence[i], to_string(sequence[i]), 0});
    }
    for (int i = 0; i < m - 1; i++) {
        int value = op(sequence[i], sequence[i+1], table);
        dp[i][i+1].push_back({(value), dp[i][i][0].expr + dp[i+1][i+1][0].expr, i});
    }

    // vamos por cada diagonal, preenchendo a tabela da esquerda para direita, cima para baixo
    for (int diagonal = 2; diagonal < m; diagonal++) { // basicamente as diagonais sao o tamanho
        for (int i = 0; i < m - diagonal; i++) {       // i percorre a altura da tabela / starting point seq
            int j = i + diagonal;                     // j = final da sequencia
            // calcular ja o tamanho das sublistas para fazer os loops
            int sub_lista1 = dp[i][j-1].size();
            int sub_lista2 = dp[i+1][j].size();
            int value = 0;
            for (int left = 0; left < sub_lista1; left++){
                value = op(dp[i][j-1][left].value, dp[i+diagonal][j][0].value, table); 
                // colocamos todos os valores mas depois tiramos os repetidos noutro sitio para reduzir time complexity
                dp[i][j].push_back({value, dp[i][j-1][left].expr + dp[i+diagonal][j][0].expr, dp[i][j-1][left].parametrizacao});

            }
            for (int below = 0; below < sub_lista2; below++){
                value = op(dp[i][j-diagonal][0].value, dp[i+1][j][below].value, table); 
                // colocamos todos os valores mas depois tiramos os repetidos noutro sitio para reduzir time complexity
                dp[i][j].push_back({value, dp[i][j-diagonal][0].expr + dp[i+1][j][below].expr, dp[i+1][j][below].parametrizacao});
            }
            // dar skim a lista
            filterUnique(dp[i][j]);
        }   
    }
    printMatrix(dp);
    Result result;
    result.parametrizacao = -1;
    for (Result res: dp[0][m-1]){
        if (desired_result == res.value){
            if (result.parametrizacao == -1){
                result.expr = res.expr;
                result.parametrizacao = res.parametrizacao;
            } else {
                if (res.parametrizacao < result.parametrizacao){
                    result.expr = res.expr;
                    result.parametrizacao = res.parametrizacao;
                }
            }
        }
    }
    int i = 0;
    int flag = 0;
    while (i < result.expr.length()){
        if (flag == 1){
            cout << result.expr[i] << ") ";
        } else {
            cout << "(" << result.expr[i] << " ";
            if (result.parametrizacao == i){flag = 1;}
        }
        i++;
    }
    printf("\n");
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