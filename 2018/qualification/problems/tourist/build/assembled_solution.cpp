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

#include <algorithm>

#define ll long long

using namespace std;

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

  ll n, k, v;
  vector<string> dt;

  size_t case_number_;
};

void Solution::read_input() {
    cin >> n >> k >> v;
    dt.resize(n);
    for (auto& str : dt) {
      cin >> str;
    }
}

void Solution::solve_internal() {}

void Solution::print_answer() {
  ll offset = k * (v - 1);
  vector<pair<ll, string>> ans;
  for (ll i = 0; i < k; ++i) {
    ll index = (i + offset) % dt.size();
    ans.push_back({index, dt[index]});
  }
  sort(ans.begin(), ans.end());

  cout << "Case #" << case_number_ + 1 << ": ";
  for (ll i = 0; i < ans.size(); ++i) {
    cout << ans[i].second;
    if (i != ans.size() - 1) {
      cout << ' ';
    }
  }
  cout << endl;
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

