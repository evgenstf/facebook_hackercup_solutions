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
