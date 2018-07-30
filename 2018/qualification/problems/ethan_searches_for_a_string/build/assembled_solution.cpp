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

  string build_ans(const string& str, int offset) {
    string res;
    for (int i = 0; i < offset; ++i) {
      res.push_back(str[i]);
    }
    for (auto c : str)
      res.push_back(c);
    return res;
  }

  bool check(const string& a, const string& b) {
    int i = 1;
    int j = 1;
  lol:
    if (i > a.size()) {
      //clog << "check: true\n";
      return true;
    }
    if (j > b.size()) {
      //clog << "check: true\n";
      return false;
    }
    if (a[i - 1] == b[j - 1]) {
      ++i;
      ++j;
      goto lol;
    }
    if (i == 1) {
      ++j;
      goto lol;
    }
    i = 1;
    goto lol;
  }

  string str;
  string answer;
  bool fl;
  size_t case_number_;
};

void Solution::read_input() {
  cin >> str;
}

void Solution::solve_internal() {
  fl = false;
  for (int i = 1; i < str.size(); ++i) {
    auto ans = build_ans(str, i);
    //clog << ans << endl;
    if (!check(str, ans) && ans.find(str) != string::npos) {
      answer = ans;
      fl = true;
      break;
    }
  }
}

void Solution::print_answer() {
  cout << "Case #" << case_number_ + 1 << ": ";
  if (fl) {
    cout << answer << endl;
  } else {
    cout << "Impossible" << endl;
  }
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

