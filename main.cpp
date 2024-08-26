#include <bits/stdc++.h>
using namespace std;
const int N = 6;

struct Block {
	bool is_horizontal, is_red;
	int id, pos, start, length;

	Block(){
		this->id = 0;
		this->is_red = false;
	}

	Block(int _id, bool _is_horizontal, bool _is_red, int _pos, int _start, int _length) {
		this->id = _id;
		this->is_horizontal = _is_horizontal;
		this->is_red = _is_red;
		this->pos = _pos;
		this->start = _start;
		this->length = _length;
	}

	Block& operator=(const Block& rhs) {
		this->id = rhs.id;
		this->is_horizontal = rhs.is_horizontal;
		this->is_red = rhs.is_red;
		this->pos = rhs.pos;
		this->start = rhs.start;
		this->length = rhs.length;
	};
};

struct State {
	Block grid[N][N];
	int num_blocks;

	State() {
	}

	State(vector<Block> blocks) {
		int num_blocks = blocks.size() - 1;
		for (Block block : blocks) {
			if (block.is_horizontal) {
				for (int i = 0; i < block.length; ++i) {
					grid[block.pos][block.start + i] = block;
				}
			} else {
				for (int i = 0; i < block.length; ++i) {
					grid[block.start + i][block.pos] = block;
				}
			}
		}
	}

	vector<State> get_neighbors() {
		vector<bool> visited(num_blocks + 1, false);
		vector<State> neighbors;
		
		// Left
		for (int row = 0; row < N; ++row) {
			for (int col = 0; col < N; ++col) {
				Block block = grid[row][col];
				if (block.id == 0 || visited[block.id] || !block.is_horizontal)
					continue;
				State new_state = *this;
				for (int step = 1; block.start - step >= 0; ++step) {
					int new_start = block.start - step;
					if (grid[row][new_start].id != 0)
						break;
					Block new_block = block;
					new_block.start = new_start;
					new_state.grid[row][new_start + block.length] = Block();
					for (int i = 0; i < block.length; ++i)
						new_state.grid[row][new_start + i] = new_block;
					State neighbor = new_state;
					neighbors.push_back(neighbor);
				}
				visited[block.id] = true;
			}
		}

		// Right
		visited.assign(num_blocks + 1, false);
		for (int row = 0; row < N; ++row) {
			for (int col = 0; col < N; ++col) {
				Block block = grid[row][col];
				if (block.id == 0 || visited[block.id] || !block.is_horizontal)
					continue;
				State new_state = *this;
				for (int step = 1; block.start + step + block.length - 1 < N; ++step) {
					int new_start = block.start + step;
					int new_end = new_start + block.length - 1;
					if (grid[row][new_end].id != 0)
						break;
					Block new_block = block;
					new_block.start = new_start;
					new_state.grid[row][new_start - 1] = Block();
					for (int i = 0; i < block.length; ++i)
						new_state.grid[row][new_start + i] = new_block;
					State neighbor = new_state;
					neighbors.push_back(neighbor);
				}
				visited[block.id] = true;
			}
		}

		// Up
		visited.assign(num_blocks + 1, false);
		for (int row = 0; row < N; ++row) {
			for (int col = 0; col < N; ++col) {
				Block block = grid[row][col];
				if (block.id == 0 || visited[block.id] || block.is_horizontal)
					continue;
				State new_state = *this;
				for (int step = 1; block.start - step >= 0; ++step) {
					int new_start = block.start - step;
					if (grid[new_start][col].id != 0)
						break;
					Block new_block = block;
					new_block.start = new_start;
					new_state.grid[new_start + block.length][col] = Block();
					for (int i = 0; i < block.length; ++i)
						new_state.grid[new_start + i][col] = new_block;
					State neighbor = new_state;
					neighbors.push_back(neighbor);
				}
				visited[block.id] = true;
			}
		}

		// Down
		visited.assign(num_blocks + 1, false);
		for (int row = 0; row < N; ++row) {
			for (int col = 0; col < N; ++col) {
				Block block = grid[row][col];
				if (block.id == 0 || visited[block.id] || block.is_horizontal)
					continue;
				State new_state = *this;
				for (int step = 1; block.start + step + block.length - 1 < N; ++step) {
					int new_start = block.start + step;
					int new_end = block.start + step + block.length - 1;
					if (grid[new_end][col].id != 0)
						break;
					Block new_block = block;
					new_block.start = new_start;
					new_state.grid[new_start - 1][col] = Block();
					for (int i = 0; i < block.length; ++i)
						new_state.grid[new_start + i][col] = new_block;
					State neighbor = new_state;
					neighbors.push_back(neighbor);
				}
				visited[block.id] = true;
			}
		}

		return neighbors;
	}

	State& operator=(const State& rhs) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				this->grid[i][j] = rhs.grid[i][j];
			}
		}
	};

	bool is_final() {
		for (int r = 0; r < N; ++r)
			if (grid[r][N - 1].is_red)
				return true;
		return false;
	}

	string hash_code() {
		string h = "";
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				h = h + "_" + to_string(grid[i][j].id);
			}
		}
		return h;
	}

	void print() {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				cout << grid[i][j].id << "\t";
			}
			cout << endl;
		}
		cout << "==============================" << endl;
	}

	void print_diff(State &state) {
		set<int> diff_block_id;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				if (state.grid[i][j].id != grid[i][j].id) {
					diff_block_id.insert(grid[i][j].id != 0 ? grid[i][j].id : state.grid[i][j].id);
				}
			}
		}

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				if (diff_block_id.find(grid[i][j].id) == diff_block_id.end()) {
					cout << grid[i][j].id << "\t";
				} else {
					cout << "[" << grid[i][j].id << "]\t"; 
				}
			}
			cout << endl;
		}
		cout << "==============================" << endl;
	}
};

int main() {
#ifdef MY_LOCAL_BUILD
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int r_row, r_col;
    cin >> r_row >> r_col;

    int id = 1;
    vector<Block> blocks;
    for (int i = 0; i < N; ++i) {
    	int s;
    	cin >> s;
    	for (int j = 0; j < s; ++j) {
    		int start, length;
    		cin >> start >> length;
    		bool is_red = (i == r_row && start == r_col);
    		blocks.push_back(Block(id++, true, is_red, i, start, length));
    	}
    }

    for (int i = 0; i < N; ++i) {
    	int s;
    	cin >> s;
    	for (int j = 0; j < s; ++j) {
    		int start, length;
    		cin >> start >> length;
    		blocks.push_back(Block(id++, false, false, i, start, length));
    	}
    }

    State initial_state(blocks);

    queue<State> q;
    set<string> visited;
    State* destination = NULL;
    map<string, State> prev;

    q.push(initial_state);
    visited.insert(initial_state.hash_code());
    prev[initial_state.hash_code()] = initial_state;

    while (!q.empty() && destination == NULL) {
    	State cur_state = q.front();
    	q.pop();

    	for (State next_state : cur_state.get_neighbors()) {
    		string next_state_hash = next_state.hash_code();
    		if (visited.find(next_state_hash) != visited.end())
    			continue;
    		visited.insert(next_state_hash);
    		q.push(next_state);
    		prev[next_state_hash] = cur_state;
    		if (next_state.is_final()) {
    			destination = &next_state;
    			break;
    		}
    	}
    }

    vector<State> path;
    State *it = destination;
    while (1) {
    	string it_hash_code = it->hash_code();
    	path.push_back(*it);
    	if (it_hash_code == prev[it_hash_code].hash_code())
    		break;
    	it = &prev[it->hash_code()];
    }

   	int step = 0;
    auto prev_it = path.rbegin();
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
    	cout << "Step " << step << ":" << endl;
    	cout << "-------------------------------" << endl;
    	if (prev_it == it) {
    		it->print();
    	} else {
    		it->print_diff(*prev_it);
    	}
    	prev_it = it;
    	step++;
    }
}