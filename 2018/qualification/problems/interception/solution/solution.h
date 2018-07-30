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

  size_t n;

  size_t case_number_;
};

void Solution::read_input() {
    cin >> n;
    int x;
    for (int i = 0; i < n + 1; ++i)
      cin >> x;
}

void Solution::solve_internal() {}

void Solution::print_answer() {
  cout << "Case #" << case_number_ + 1 << ": ";
  if (n % 2) {
    cout << 1 << endl << "0.0";
  } else {
    cout << 0;
  }
  cout << endl;
}
