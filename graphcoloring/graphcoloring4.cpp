#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
using namespace std;
#define ar array
#define sz(x) (int)x.size()
int n, m;
vector<ar<int, 2> > edges;
namespace Dpbitmask
{
    const int mxn = 21;
    const int INF = 1e7 + 7;
    int a[mxn];/// chi so i khong cung mau voi nut nao
    int f[1 << mxn | 1]; ///tap co cac chi so i thi khong cung mau voi nut nao
    int ans[1 << mxn | 1];
    int chromatic_number, allmask;
    int color[mxn];

    void init()
    {
        for (ar<int, 2> edge : edges)
        {
            int u = edge[0];
            int v = edge[1];
            a[u] |= (1 << v);
            a[v] |= (1 << u);
        }
        fill(ans, ans + ((1 << n) | 1), INF);
        for(int i = 0; i < n; i++)
        {
            f[(1 << i)] = a[i];
            ans[(1 << i)] = 1;
        }
    }

    void find_chromatic_number()
    {
        allmask = (1 << n) - 1;
        for(int i = 1; i <= allmask; i++)
        {
            int ok = 1;
            for(int j = i; j > 0; j = (i & (j - 1))){
                if((f[j] & (i ^ j)) || (f[i ^ j] & (j))){
                    f[i] = f[j] | f[i ^ j];
                    ok = 0;
                }
                ans[i] = min(ans[i], ans[j] + ans[i ^ j]);
            }
            if (ok) ans[i] = 1;
        }
        chromatic_number = ans[allmask];
    }

    void tomau(int mask)
    {
        if (ans[mask] == 1)
        {
            for (int i = 0; i < n; i++)
                if ((mask >> i) & 1)
                {
                    color[i] = chromatic_number;
                }
            chromatic_number--;
            return;
        }
        for (int j = mask; j > 0; j = mask & (j - 1))
            if (ans[mask] == ans[j] + ans[mask ^ j])
            {
                tomau(j);
                tomau(mask ^ j);
                break;
            }
    }
    
    void output_coloring(string path)
    {
        ofstream fout(path);
        if (!fout.is_open()) {
            cerr << "Failed to open output file: " << path << endl;
            return;
        }
        fout << ans[allmask] << '\n';
        for (int i = 0; i < n; i++) fout << color[i] << '\n';
        fout.close();
    }

    int solve()
    {
        init();
        find_chromatic_number();
        tomau(allmask);
        return chromatic_number;
    }
}

namespace Dsatur
{
    const int mxn = 1e4 + 7;
    int degree[mxn];
    int color[mxn];
    vector<int> g[mxn];
    set<int> adjcolor[mxn];
    int chromatic_number;
    struct nodeInfo {
    int sat; // Saturation degree of the vertex
    int deg; // Degree in the uncoloured subgraph
    int id; // Index of vertex
    };

    struct maxSat {
        bool operator()(const nodeInfo& lhs,
                        const nodeInfo& rhs) const
        {
            return tie(lhs.sat, lhs.deg, lhs.id) > tie(lhs.sat, lhs.deg, lhs.id);
        }
    };

    set<nodeInfo, maxSat> S;
    set<nodeInfo, maxSat> :: iterator it;
    
    void init()
    {
        for (ar<int, 2> edge: edges)
        {
            int u = edge[0];
            int v = edge[1];
            degree[u]++;
            degree[v]++;
            g[u].push_back(v);
            g[v].push_back(u);
        }

        for (int i = 0; i < n; i++)
        {
            S.insert(nodeInfo{0, degree[i], i});
        }
    }
    void find_coloring()
    {
        while(!S.empty())
        {
            it = S.begin();
            int u = it->id;
            S.erase(it);

            vector<bool> used_color(n + 1);
            for (int v : g[u])
            {
                if (color[v]) used_color[color[v]] = 1;
            }
            for (int i = 1; i <= n; i++) 
                if (!used_color[i])
                    {
                        color[u] = i;
                        break;
                    }
            for (int v : g[u])
            {
                if (color[v]) continue;
                S.erase(nodeInfo{sz(adjcolor[v]), degree[v], v});
                adjcolor[v].insert(color[u]);
                degree[v]--;
                S.insert(nodeInfo{sz(adjcolor[v]), degree[v], v});
            }
        }
        
        for (int i = 0; i < n; i++) chromatic_number = max(chromatic_number, color[i]);
    }
    
    void output_coloring(string path)
    {
        ofstream fout(path);
        if (!fout.is_open()) {
            cerr << "Failed to open output file: " << path << endl;
            return;
        }
        fout << chromatic_number << '\n';
        for (int i = 0; i < n; i++) fout << color[i] << '\n';
        fout.close();
    }
    int solve()
    {
        init();
        find_coloring();
        return chromatic_number;
    }
}

namespace RLF
{
    const int mxn = 1e4 + 1;
    int degree_U[mxn], degree_W[mxn];
    bool is_in_W[mxn];
    int color[mxn];
    vector<int> g[mxn];
    int chromatic_number;
    vector<int> uncolored;

    void init()
    {
        for (ar<int, 2> edge: edges)
        {
            int u = edge[0];
            int v = edge[1];
            degree_U[u]++;
            degree_U[v]++;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        for (int i = 0; i < n; i++) uncolored.push_back(i);
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle (uncolored.begin(), uncolored.end(), std::default_random_engine(seed));
    }

    void move_to_W(int u, int p)
    {
        is_in_W[u] = 1;
        for (int v : g[u])
            if (!color[v] && v != p)
            {
                degree_W[v]++;
                degree_U[v]--;
            }
    }

    void move_to_Cv(int u)
    {
        for (int v : g[u])
        if (!color[v])
        {
            degree_U[v]--;
            if (!is_in_W[v]) move_to_W(v, u);
        }
    }

    vector<int> select_starting_vertices(const int M)
    {
        vector<int> ans;
        int mx = 0;
        vector<vector<int> > vertices_of_deg(sz(uncolored) + 1);
        for (int v : uncolored) 
        {
            vertices_of_deg[sz(g[v])].push_back(v);
            mx = max(mx, sz(g[v]));
        }
        
        for (int i = mx; i >= 0; i--)
            for (int u : vertices_of_deg[i])
                {
                    ans.push_back(u);
                    if (sz(ans) >= M) return ans;
                }
        return ans;
    }

    vector<int> Cv(int u, int color_num) // construct new Color
    {
        // Init degree 
        for (int v : uncolored) 
        {
            degree_U[v] = sz(g[u]);
            degree_W[v] = 0;
            is_in_W[v] = 0;
        }

        vector<int> NN; // non neighbors of u aka potential vertices for Cv

        // Init NN
        move_to_Cv(u);
        for (int v : uncolored)
            if (!is_in_W[v]) NN.push_back(v);

        vector<int> ans;
        color[u] = color_num;
        ans.push_back(u);

        while(!NN.empty()) // construct Cv
        {   
            int mx = 0, deg = n, vertex = NN[0];

            for (int v : NN)
            {   
                if (degree_W[v] > mx || degree_W[v] == mx && degree_U[v] < deg)
                {
                    mx = degree_W[v];
                    deg = degree_U[v];
                    vertex = v;
                }
            }

            if (mx == 0)
            for (int v : NN) 
                if (degree_U[v] > degree_U[vertex]) vertex = v;
            
            color[vertex] = color_num;
            ans.push_back(vertex);
            move_to_Cv(vertex);

            for (int i = sz(NN) - 1; i >= 0; i--)
                if (is_in_W[NN[i]] || color[NN[i]]) swap(NN[i], NN.back()), NN.pop_back();
        }
        
        int cnt = 0;
        for (int v : ans)
        {
            color[v] = 0;
            cnt += sz(g[v]);
        }

        ans.push_back(cnt); //cnt = number of edges reduce
        return ans;
    }
    
    void find_coloring(const int M)
    {
        while(!uncolored.empty())
        {
            vector<int> starting_vertices = select_starting_vertices(M);
            ++chromatic_number;
            vector<int> actualCv;
            for (int u : starting_vertices)
            {
                vector<int> temp = Cv(u, chromatic_number);
                if (actualCv.empty() || actualCv.back() < temp.back()) actualCv = temp;
            }
            actualCv.pop_back();
            for (int u : actualCv) color[u] = chromatic_number;

            for (int i = sz(uncolored) - 1; i >= 0; i--)
                {
                    int v = uncolored[i];
                    if (color[v]) swap(uncolored[i], uncolored.back()), uncolored.pop_back();
                }
            for (int u : uncolored)
            {
                for (int i = sz(g[u]) - 1; i >= 0; i--)
                {
                    if (color[g[u][i]]) 
                    {
                        swap(g[u][i], g[u].back());
                        g[u].pop_back();
                    }
                }
            }
        }
    }

    void output_coloring(string path)
    {
        ofstream fout(path);
        if (!fout.is_open()) {
            cerr << "Failed to open output file: " << path << endl;
            return;
        }
        fout << chromatic_number << '\n';
        for (int i = 0; i < n; i++) fout << color[i] << '\n';
        fout.close();
    }
    
    int solve(const int M)
    {
        init();
        find_coloring(M);
        return chromatic_number;
    }
}
void input_graph(string path) {
    ifstream fin(path);
    if (!fin.is_open()) {
        cerr << "Failed to open input file: " << path << endl;
        return;
    }
    fin >> n >> m;
    for(int i = 0; i < m; i++)
    {
        int u, v;
        fin >> u >> v;
        u--;
        v--;
        edges.push_back({u, v});
    }
    fin.close();
}

int main(int argc, char const *argv[]) 
{
    ios::sync_with_stdio(0);    
    cin.tie(0); cout.tie(0);

    if (argc < 3) {
        cout<<"Usage: "<<argv[0]<< " input.txt    output_dir"<<endl;
        return 1;
    } 

    input_graph(argv[1]);
    int ans1;
    if (n <= 20) {
        ans1 = Dpbitmask::solve();
        Dpbitmask::output_coloring(argv[2]);
    } else if (n > 20 && n <= 2000) {
        ans1 = RLF::solve(n);
        RLF::output_coloring(argv[2]);
    } else {
        ans1 = RLF::solve(10);
        RLF::output_coloring(argv[2]);
    }
    cout << "Input done: " << ans1 << endl;
    return 0;
}
