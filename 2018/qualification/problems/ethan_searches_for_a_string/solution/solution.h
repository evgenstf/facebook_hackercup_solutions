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
