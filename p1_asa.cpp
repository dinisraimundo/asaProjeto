#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <set>


using namespace std;

// Estrutura para armazenar o valor e a expressão
struct Result {
    int value;        // Valor do resultado
    string expr;      // String com a expressão gerada
    bool operator<(const Result& other) const { // dar override para conseguir usar set
            return value < other.value;
    }
};

// Função para calcular a operação usando a tabela
int op(int a, int b, const vector<vector<int>>& table) {
    return table[a - 1][b - 1]; // Ajuste de índices (1-base para 0-base)
}

int contains(const vector<Result>& vector, int value){
    int len = vector.size();
    for (int i = 0; i < len; i++){
        if (vector[i].value == value){
            return 1; 
        }
    }
    return 0;
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
                    cout << res.value << " "; // Print each value
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
        dp[i][i].push_back({sequence[i], to_string(sequence[i])});
    }
    for (int i = 0; i < m - 1; i++) {
        int value = op(sequence[i], sequence[i+1], table);
        dp[i][i+1].push_back({(value), to_string(value)});
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
                dp[i][j].push_back({value, to_string(value)});

            }
            for (int below = 0; below < sub_lista2; below++){
                value = op(dp[i][j-diagonal][0].value, dp[i+1][j][below].value, table); 
                // colocamos todos os valores mas depois tiramos os repetidos noutro sitio para reduzir time complexity
                dp[i][j].push_back({value, to_string(value)});
            }
            // dar skim a lista
            set<Result> s(dp[i][j].begin(), dp[i][j].end());
            dp[i][j].assign(s.begin(), s.end());
        }   
    }

    printMatrix(dp);

// no teste de input da print a dois 1's seguidos idk why

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
