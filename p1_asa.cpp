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
    int value;        // Valor do resultado     // String com a expressão gerada
    int parametrizacao; // Parametrização
    shared_ptr<Result> left; 
    shared_ptr<Result> down;

};


// Função para calcular a operação usando a tabela
int op(int a, int b, const vector<vector<int>>& table) {
    return table[a - 1][b - 1]; // Ajuste de índices (1-base para 0-base)
}

void filterUnique(vector<Result>& results) {
    map<int, Result> bestResults; // Maps `value` to the best `Result`

    for (Result res : results) {
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
                        cout << res.value << "|" << res.parametrizacao<< " "; 
                    
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

string buildExpression(const shared_ptr<Result>& res) {
    if (!res) return ""; // Base case for null pointer

    if (!res->left && !res->down) { // Base case: leaf node
        return to_string(res->value);
    }

    string leftExpr = buildExpression(res->left);
    string downExpr = buildExpression(res->down);

    // Combine the current value into the expression
    return "(" + leftExpr + " " + downExpr + ")";
}

void traceSolution(const shared_ptr<Result>& res) {
    if (!res) {
        cout << "No solution to trace.\n";
        return;
    }

    // Generate the expression and print it
    string expression = buildExpression(res);
    cout << "Expression: " << expression << endl;
}

void printTrace(const shared_ptr<Result>& res) {
    if (!res) return;

    if (res->left || res->down) {
        cout << res->value << ": ";
        cout << res->left->value << "||" << res->down->value << endl;
        printTrace(res->left);
        printTrace(res->down);
    }
}


void solve(int n, int m, const vector<vector<int>>& table, const vector<int>& sequence, int desired_result) {

    // Matriz dinâmica para DP (cada célula contém uma lista de resultados possíveis)
    vector<vector<vector<Result>>> dp(m, vector<vector<Result>>(m));


    // Inicializar os elementos unicos da seq
    // Já agora vamos inicializar também a prox diagonal
// Initialize base cases
for (int i = 0; i < m; i++) {
    dp[i][i].push_back({sequence[i], 0, nullptr, nullptr});
}

for (int i = 0; i < m - 1; i++) {
    int value = op(sequence[i], sequence[i + 1], table);
    dp[i][i + 1].push_back(
        {value, i, make_shared<Result>(dp[i][i][0]), make_shared<Result>(dp[i + 1][i + 1][0])});
}

// Fill the DP table
for (int diagonal = 2; diagonal < m; diagonal++) {
    for (int i = 0; i < m - diagonal; i++) {
        int j = i + diagonal;
        int sub_lista1 = dp[i][j - 1].size();
        int sub_lista2 = dp[i + 1][j].size();
        for (int left = 0; left < sub_lista1; left++) {
            int value = op(dp[i][j - 1][left].value, dp[i + diagonal][j][0].value, table);
            if (!find_in_vector(dp[i][j], value))
                dp[i][j].push_back(
                    {value, dp[i][j - 1][left].parametrizacao, make_shared<Result>(dp[i][j - 1][left]), make_shared<Result>(dp[i + diagonal][j][0])});
        }
        for (int below = 0; below < sub_lista2; below++) {
            int value = op(dp[i][j - diagonal][0].value, dp[i + 1][j][below].value, table);
            if (!find_in_vector(dp[i][j], value))
                dp[i][j].push_back(
                    {value, dp[i + 1][j][below].parametrizacao , make_shared<Result>(dp[i][j - diagonal][0]), make_shared<Result>(dp[i + 1][j][below])});
        }
    }
}
    int len = dp[0][m - 1].size();
    for (int i = 0; i < len; i++){
        if (dp[0][m - 1][i].value == desired_result){
            printTrace(make_shared<Result>(dp[0][m - 1][i]));
            break;
        }
    }
    printMatrix(dp);
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