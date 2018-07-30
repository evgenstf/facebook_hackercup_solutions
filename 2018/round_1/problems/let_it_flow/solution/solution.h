using namespace std;

const long long mod = 1000000007;

class Solution : public AsyncSolution {
public:
  explicit Solution(size_t case_number, atomic<size_t>& free_threads_count):
      AsyncSolution(case_number, free_threads_count),
      case_number_(case_number) {
    read_input();
  }

  void print_answer();

protected:
  void solve_internal() override;

private:
  void read_input();

  int n;
  vector<vector<char>> field;
  long long answer;

  size_t case_number_;
};

void Solution::read_input() {
  cin >> n;
  field.assign(n + 1, vector<char>(3));
  for (int i = 0; i < 3; ++i) {
    for (int j = 1; j < n + 1; ++j) {
      cin >> field[j][i];
    }
  }
}

void Solution::solve_internal() {
  vector<vector<long long> > dp(n + 1, vector<long long>(3, 0));
  dp[0][0] = 1;
  for (int i = 1; i < n + 1; ++i) {
    if (field[i][0] == '.' && field[i][1] == '.') {
      dp[i][1] += dp[i - 1][0];
    }
    if (field[i][1] == '.' && field[i][0] == '.') {
      dp[i][0] += dp[i - 1][1];
    }
    if (field[i][1] == '.' && field[i][2] == '.') {
      dp[i][2] += dp[i - 1][1];
    }
    if (field[i][2] == '.' && field[i][1] == '.') {
      dp[i][1] += dp[i - 1][2];
    }
    dp[i][0] %= mod;
    dp[i][1] %= mod;
    dp[i][2] %= mod;
  }
  answer = dp[n][2];
}

void Solution::print_answer() {
  cout << "Case #" << case_number_ + 1 << ": " << answer << endl;
}
