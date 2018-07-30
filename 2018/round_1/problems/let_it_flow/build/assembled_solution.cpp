#include <iostream>
#include <sstream>
#include <atomic>
#include <memory>
#include <cassert>
#include <thread>
#include <vector>
#include <mutex>

constexpr size_t kMaxThreadsCount = 19;

namespace logger {
  std::mutex logs_mutex;
};

#define log(log_message, method_name, class_name)\
{\
  std::lock_guard<std::mutex> lock(logger::logs_mutex);\
  std::clog << '[' << class_name << "::" << method_name << "]: " << log_message << std::endl;\
}

class AsyncSolution {
public:
  AsyncSolution(size_t case_number, std::atomic<size_t>& free_threads_count):
      case_number_(case_number),
      free_threads_count_(free_threads_count) {}

  void start_solving_thread() {
    assert(free_threads_count_ > 0);
    --free_threads_count_;
    solving_thread_ = std::make_unique<std::thread>([this]{this->solve();});
  }

  void join() {
    solving_thread_->join();
  }

protected:
  virtual void solve_internal() = 0;

private:
  void solve() {
    log("start solution " << case_number_, "solve", "AsyncSolution");
    solve_internal();
    log("finish solution " << case_number_, "solve", "AsyncSolution");
    ++free_threads_count_;
  }

  size_t case_number_;
  std::atomic<size_t>& free_threads_count_;
  std::unique_ptr<std::thread> solving_thread_;
};

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
int main() {
  atomic<size_t> free_threads_count = kMaxThreadsCount;

  size_t case_count;
  cin >> case_count;
  vector<Solution> solutions;
  for (size_t i = 0; i < case_count; ++i) {
    solutions.emplace_back(i, free_threads_count);
  }
  log("solutions initialized", "main", "");

  for (auto& solution : solutions) {
    solution.start_solving_thread();
    while (free_threads_count == 0) {}
  }
  log("solutions started", "main", "");

  for (auto& solution : solutions) {
    solution.join();
    solution.print_answer();
  }
  log("solutions finished", "main", "");
}

