using namespace std;


class Solution : public AsyncSolution {
public:

  vector<int> pre_order_by_colors;
  vector<int> post_order_by_colors;

  void fill_pre_order_by_colors(int v, const vector<int>& colors, const vector<vector<int>>& G) {
    pre_order_by_colors.push_back(colors[v]);
    for (auto to : G[v])
      fill_pre_order_by_colors(to, colors, G);
  }

  void fill_post_order_by_colors(int v, const vector<int>& colors, const vector<vector<int>>& G) {
    for (auto to : G[v])
      fill_post_order_by_colors(to, colors, G);
    post_order_by_colors.push_back(colors[v]);
  }

  void check(const vector<int>& colors, const vector<vector<int>>& G) {
    pre_order_by_colors.clear();
    post_order_by_colors.clear();
    fill_pre_order_by_colors(0, colors, G);
    fill_post_order_by_colors(0, colors, G);
    if (pre_order_by_colors != post_order_by_colors) {
      clog << "FAIL\n";
      log_order(pre_order_by_colors);
      log_order(post_order_by_colors);
      exit(0);
    }
  }

  explicit Solution(size_t case_number, atomic<size_t>& free_threads_count):
      AsyncSolution(case_number, free_threads_count),
      case_number_(case_number) {
    read_input();
  }

  void print_answer();

protected:
  void solve_internal() override;

private:
  void log_order(const vector<int>& order) {
    clog << "order: ";
    for (auto v : order)
      clog << v << ' ';
    clog << endl;
  }


  void read_input();

  void fill_pre_order(int v, const vector<vector<int>>& G) {
    pre_order.push_back(v);
    for (auto to : G[v])
      fill_pre_order(to, G);
  }

  void fill_post_order(int v, const vector<vector<int>>& G) {
    for (auto to : G[v])
      fill_post_order(to, G);
    post_order.push_back(v);
  }

  void fill_colors(int v, int color, vector<int>& colors, const vector<int>& permutation) {
    colors[v] = color;
    int to = permutation[v];
    if (!colors[to]) {
      fill_colors(to, color, colors, permutation);
    }
  }


  int n, k;
  int next_color = 1;
  vector<vector<int>> G;
  vector<int> pre_order;
  vector<int> post_order;
  vector<int> colors;

  size_t case_number_;
};

void Solution::read_input() {
  cin >> n >> k;
  G.resize(n);
  for (int i = 0; i < n; ++i) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    if (a != -1) {
      G[i].push_back(a);
    }
    if (b != -1) {
      G[i].push_back(b);
    }
  }
  pre_order.clear();
  post_order.clear();
  fill_pre_order(0, G);
  fill_post_order(0, G);
}

void Solution::solve_internal() {
    vector<int> permutation(n);
    for (int i = 0; i < n; ++i) {
      permutation[pre_order[i]] = post_order[i];
    }
    colors.resize(n);
    for (int i = 0; i < n; ++i) {
      if (!colors[i]) {
        //clog << "fill color from node: " << i << endl;
        fill_colors(i, min(next_color, k), colors, permutation);
        //clog << "finished" << endl;
        ++next_color;
      }
    }
    check(colors, G);
}

void Solution::print_answer() {
  cout << "Case #" << case_number_ + 1 << ": ";
  if (next_color <= k) {
    cout << "Impossible\n";
    return;
  }
  for (auto color : colors) {
    cout << color << ' ';
  }
  cout << endl;
}
