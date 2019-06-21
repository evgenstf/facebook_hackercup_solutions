#include <iostream>
#include <sstream>
#include <atomic>
#include <memory>
#include <cassert>
#include <thread>
#include <vector>
#include <mutex>

constexpr size_t kMaxThreadsCount = 10;

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

  const double eps = 1e-4;

  template<typename T>
  void log_array(const vector<T>& array, const string& name) {
    return;
    int cnt = 10;
    clog << name << ": ";
    for (auto x : array) {
      clog << x << ' ';
      if (--cnt < 0)
        break;
    }
    clog << endl;
  }
  struct parkourist {
    int start, finish;
    ll up_limit, down_limit;
  };

  auto generate_heights(ll h1, ll h2, ll w, ll x, ll y, ll z, int n) {
    vector<ll> result{h1, h2};
    for (int i = 2; i < n; ++i) {
      result.push_back((w * result[i - 2] + x * result[i - 1] + y) % z);
    }
    for (auto& h : result) {
      h *= 2ll;
    }
    return result;
  }

  vector<parkourist> dt;
  vector<ll> heights;
  vector<ll> up_limit_to_next;
  vector<ll> down_limit_to_next;

  void init_limits() {
    //clog << "limits:\n";
    up_limit_to_next.assign(200005, 1000000000);
    down_limit_to_next.assign(200005, 1000000000);
    for (const auto& human : dt) {
      for (int i = human.start; i < human.finish; ++i) {
        up_limit_to_next[i] = min(up_limit_to_next[i], human.up_limit);
        down_limit_to_next[i] = min(down_limit_to_next[i], human.down_limit);
      }
    }
    log_array(up_limit_to_next, "up_limit_to_next");
    log_array(down_limit_to_next, "down_limit_to_next");
  }

  bool check(ll timer) {
    //clog << "check timer: " << timer << endl;
    vector<ll> top;
    vector<ll> bottom;
    for (auto h : heights) {
      top.push_back(h + timer);
      bottom.push_back(max(0ll, h - timer));
    }
    log_array(top, "top");
    log_array(bottom, "bottom");

    ll lowest = bottom.front();
    ll hirest = top.front();
    //clog << "hirest: " << hirest << " lowest: " << lowest << endl;
    for (int i = 0; i < top.size() - 1; ++i) {
      ll next_hirest = hirest + up_limit_to_next[i];
      ll next_lowest = lowest - down_limit_to_next[i];
      next_hirest = min(next_hirest, top[i + 1]);
      next_lowest = max(next_lowest, bottom[i + 1]);
      if (next_hirest + eps < next_lowest) {
        //clog << "result: false\n\n";
        return false;
      }
      lowest = next_lowest;
      hirest = next_hirest;
      //clog << "hirest: " << hirest << " lowest: " << lowest << endl;
    }
    //clog << "result: true\n\n";
    return true;
  }

  int n, m;
  long long h1, h2, w, x, y, z;
  double answer;
  size_t case_number_;
};

void Solution::read_input() {
    cin >> n >> m;
    cin >> h1 >> h2 >> w >> x >> y >> z;
    heights = generate_heights(h1, h2, w, x, y, z, n);
    log_array(heights, "heights");

    dt.clear();
    dt.resize(m);
    for (auto& x : dt) {
      cin >> x.start >> x.finish >> x.up_limit >> x.down_limit;
      --x.start, --x.finish;
      x.up_limit *= 2;
      x.down_limit *= 2;
      if (x.start > x.finish) {
        swap(x.start, x.finish);
        swap(x.up_limit, x.down_limit);
      }
    }
    init_limits();
}


void Solution::solve_internal() {
    ll l = -1;
    ll r = 1000000000;
    while (l < r - 1) {
      ll m = (l + r) / 2;
      if (check(m)) {
        r = m;
      } else {
        l = m;
      }
    }
    answer = ((double)r) / 2.0;
}

void Solution::print_answer() {
  cout.precision(10);
  cout << "Case #" << case_number_ + 1 << ": " << fixed << answer << endl;
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

