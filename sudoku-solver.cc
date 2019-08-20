#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <variant>
#include <array>
#include <typeinfo>
#include <iterator>
#include <type_traits>
#include <utility>
#include <numeric>

using namespace std;

using possibilities = std::vector<int>;
using cell = std::variant<//const int,	//The puzzle started with this filled
						int, 			//What we came up with while solving
						possibilities>;	//What possible values this cell can assume
constexpr int x_size = 9;
constexpr int y_size = 9;
constexpr int board_size = x_size * y_size;
using board = std::array<std::array<cell, x_size>, y_size>;
//using board = std::vector<std::vector<cell>>;

const possibilities all_possibilities{1, 2, 3, 4, 5, 6, 7, 8, 9};
const auto& ap = all_possibilities;
const std::array<cell, 9> empty_row{ap, ap, ap, ap, ap, ap, ap, ap, ap};
//const std::vector<cell> empty_row{9, ap};
const auto& er = empty_row;
//const board starting_board{9, er};
//const board starting_board{ {er}, {er}, {er}, {er}, {er}, {er}, {er}, {er}, {er} };
const board starting_board{ er, er, er, er, er, er, er, er, er };
/*const board starting_board{ {std::array<possibilities, 9>{er}}, 
							{std::array<possibilities, 9>{er}}, 
							{std::array<possibilities, 9>{er}}, 
							{std::array<possibilities, 9>{er}}, 
							{std::array<possibilities, 9>{er}}, 
							{std::array<possibilities, 9>{er}}, 
							{std::array<possibilities, 9>{er}}, 
							{std::array<possibilities, 9>{er}}, 
							{std::array<possibilities, 9>{er}} }; */
//const board starting_board{ {er}, {er}, {er}, {er}, {er}, {er}, {er}, {er}, {er} }; 
/*const board starting_board{ {ap, ap, ap, ap, ap, ap, ap, ap, ap}, 
							{ap, ap, ap, ap, ap, ap, ap, ap, ap}, 
							{ap, ap, ap, ap, ap, ap, ap, ap, ap}, 
							{ap, ap, ap, ap, ap, ap, ap, ap, ap}, 
							{ap, ap, ap, ap, ap, ap, ap, ap, ap}, 
							{ap, ap, ap, ap, ap, ap, ap, ap, ap}, 
							{ap, ap, ap, ap, ap, ap, ap, ap, ap}, 
							{ap, ap, ap, ap, ap, ap, ap, ap, ap}, 
							{ap, ap, ap, ap, ap, ap, ap, ap, ap} };*/

const char cell_separator = '|';
const char line_separator = '-';

struct outputter
{
	std::ostream& out;
	int width;
	outputter(std::ostream& ostr, int in_width) : out{ostr}, width{in_width} {}

	//template<typename T>
	//void operator()(T& t) {
		//out << t << '|' << typeid(T).name();
	//}
	private:
		void pre_pad_for(int size) {
			if (width == 1)
				return;
			if (int pre = (width - size) / 2)
				out << setw(pre) << "";
		}
		void post_pad_for(int size) {
			if (width == 1)
				return;
			int post = (width - size) / 2;
			if ((width - size) % 2 == 1)
				++post;
			if (post)
				out << setw(post) << "";
		}
	public:
		void operator()(int i) {
			pre_pad_for(1);
			out << ' ' << i << ' ';
			post_pad_for(1);
			out << cell_separator;
		}
		void operator()(const possibilities& poss) {
			pre_pad_for(poss.size());
			out << '{';
			std::for_each(begin(poss), end(poss), [this](auto v){
				out << v;
			});
			out << '}';
			post_pad_for(poss.size());
			out << cell_separator;
		}
};

struct get_no_possibilities
{
	int operator()(int) { return 0; }
	int operator()(const possibilities& poss) {
		return poss.size();
	}
};

//Gets maximum no of possibilities
int get_max_cell_width(const board& b) {
	return std::accumulate(begin(b), end(b), 1, [](int curr_max, const auto& row){
		return std::max(
			curr_max, 
			std::accumulate(begin(row), end(row), curr_max, [](int curr_max, const auto& c) {
				return std::max(curr_max, std::visit(get_no_possibilities{}, c));
			})
		);
	});
}

void print(const board& b) {
	const int cell_width = get_max_cell_width(b);
	const int line_width = (cell_width + 2/*space b4 & aftr*/) * y_size + (y_size + 1); //separator '|'
	std::for_each(begin(b), end(b), [cell_width, line_width](const auto& row) {
	    std::cout << std::setfill(line_separator) << std::setw(line_width) << '\n';
	    std::cout << cell_separator;
		std::for_each(begin(row), end(row), [cell_width](const auto& c) {
			std::visit(outputter{std::cout, cell_width}, c);
		});
		std::cout << '\n';
	});
	std::cout << std::setfill(line_separator) << std::setw(line_width) << '\n';
}

struct assignment
{
	int x;
	int y;
	int value;
	bool operator==(const assignment& other) const {
		return x == other.x && y == other.y && value == other.value;
	}
	bool operator!=(const assignment& other) const {
		return !(*this == other);
	}
};
void print(const assignment& assign) {
	std::cout << "b[" << assign.x << "][" << assign.y << "] = " << assign.value;
}
void print(const std::vector<assignment>& moves) {
	for (const auto& assign : moves) {
		print(assign);
		std::cout << '\n';
	}
}

struct eliminate_possibility
{
	int value;
	eliminate_possibility(int val) : value(val) {}
	size_t operator()(int constant) { return 0; }
	size_t operator()(std::vector<int>& a_cell) {
		a_cell.erase(std::remove(begin(a_cell), end(a_cell), value), end(a_cell));
		return a_cell.size();
	}
};

int get_adj_start(int x) {
	if (x < 3)
		return 0;
	if (x < 6)
		return 3;
	return 6;
}

std::pair<int, int> 
get_adj_start(const assignment& assign) {
	return {get_adj_start(assign.x), get_adj_start(assign.y)};
}

template<typename T>
auto& operator<<(std::vector<T>& container, T value) {
	if (auto it = std::find(cbegin(container), cend(container), value);
			it == cend(container)) {
		container.push_back(value);
	}
	return container;
}

//To track how many iterators are there for each of the 9 numbers
using tracking = std::array<std::vector<assignment>, 9>;
auto size_is_1 = [](const auto& container){ return container.size() == 1; };

/*auto& operator<<(tracking& track, const possibilities& poss) {
	//Add iterator of each number at the position of that number
	//i.e. track[5].push_back(it_to_5);
	for (auto it = begin(poss); it != end(poss); ++it) {
		track[*it].push_back(it);
	}
}*/

auto analyse_row(board& b, const assignment& assign) {
	std::vector<assignment> next_assignments;
	//std::array<std::vector<std::vector<int>::iterator>, 9> numbers;
	tracking track;
	for (auto it = begin(b[assign.x]); it != end(b[assign.x]); ++it) {
		auto x = assign.x;
		auto y = static_cast<int>(it - begin(b[assign.x]));
		auto no_possibilities = std::visit(eliminate_possibility{assign.value}, *it);
		if (no_possibilities == 1) {
			next_assignments.push_back({x, y, get<std::vector<int>>(*it)[0]});
			std::cout << __func__ << " found: ";
			print(next_assignments.back());
			std::cout << ' ';
		}
		if (no_possibilities != 0) {	//track vectors, ignore ints
			//track << get<std::vector<int>>(*it);
			const auto& a_cell = std::get<std::vector<int>>(*it);
			for_each(begin(a_cell), end(a_cell), [&track, x, y] (int n) {
				track[n - 1].push_back({x, y, n});
			});
		}
	}
	for (auto& vect : track) {
		if (size_is_1(vect)) {
			std::cout << "Row tracking found: ";
			print(vect[0]);
			std::cout << ' ';
			next_assignments.push_back(vect[0]);
		}
	}
	return next_assignments;
}
auto analyse_col(board& b, const assignment& assign) {
	std::vector<assignment> next_assignments;
	tracking track;
	for (int curr = 0; curr != 9; ++curr) {
		auto no_possibilities = std::visit(eliminate_possibility{assign.value}, b[curr][assign.y]);
		if (no_possibilities == 1) {
			next_assignments.push_back({curr, assign.y, 
				get<std::vector<int>>(b[curr][assign.y])[0]});
			std::cout << __func__ << " found: ";
			print(next_assignments.back());
			std::cout << ' ';
		}
		if (no_possibilities != 0) {	//track vectors, ignore ints
			const auto& a_cell = std::get<std::vector<int>>(b[curr][assign.y]);
			for_each(begin(a_cell), end(a_cell), [&track, x = curr, y = assign.y] (int n) {
				track[n - 1].push_back({x, y, n});
			});
		}
	}
	for (auto& vect : track) {
		if (size_is_1(vect)) {
			std::cout << "Column tracking found: ";
			print(vect[0]);
			std::cout << ' ';
			next_assignments.push_back(vect[0]);
		}
	}
	return next_assignments;
}
auto analyse_adj(board& b, const assignment& assign) {
	std::vector<assignment> next_assignments;
	tracking track;
	auto [x, y_init] = get_adj_start(assign);
	//const auto y_init = y;	//to restart inner for loop for column
	const auto x_end = x + 3;
	const auto y_end = y_init + 3;
	//std::cout << "analyse_adj: x_init = " << x << ", y_init = " << y_init << ", x_end = " << x_end 
		//<< ", y_end = " << y_end << std::endl;

	for (; x < x_end; ++x) {
		//std::cout << "x = " << x << std::endl;
		//Cannot skip this row despite analyse_row because we need to track
		//if (x == assign.x)
			//continue;
		for (auto y = y_init; y < y_end; ++y) {
			//std::cout << "y = " << y << std::endl;
			//Cannot skip this column despite analyse_col because we need to track
			//if (y == assign.y)
				//continue;
			auto no_possibilities = std::visit(eliminate_possibility{assign.value}, b[x][y]);
			//std::cout << "visited b[" << x << "][" << y << "], no_possibilities = " << no_possibilities << '\n';
			if (no_possibilities == 1) {
				try {
				//next_assignments.push_back({x, y, std::get<std::vector<int>>(b[x][y])[0]});
				const auto& a_cell = std::get<std::vector<int>>(b[x][y]);
				next_assignments.push_back({x, y, a_cell[0]});
				//next_assignments << assignment{x, y, a_cell[0]};
				std::cout << __func__ << " found: ";
				print(next_assignments.back());
				std::cout << ' ';
				} catch (std::bad_variant_access&) {
					std::cout << "bad_variant_access\n";
				}
			}
			if (no_possibilities != 0) {	//track vectors, ignore ints
				const auto& a_cell = std::get<std::vector<int>>(b[x][y]);
				for_each(begin(a_cell), end(a_cell), [&track, x, y] (int n) {
					track[n - 1].push_back({x, y, n});
				});
			}
		}
	}
	//TODO: This has not worked correctly
	for (auto& vect : track) {
		if (size_is_1(vect)) {
			std::cout << "Adjacent tracking found: ";
			print(vect[0]);
			std::cout << ' ';
			next_assignments.push_back(vect[0]);
		}
	}
	return next_assignments;
}
auto make_move(board& b, const assignment& assign) {
	//TODO: Sanity check? not needed for starting conditions
	std::cout << "make_move: ";
	print(assign);
	std::cout << ' ';

	b[assign.x][assign.y] = assign.value;

	auto next_row_assigns = analyse_row(b, assign);

	auto next_col_assigns = analyse_col(b, assign);
	next_row_assigns.insert(end(next_row_assigns), begin(next_col_assigns), end(next_col_assigns));

	auto next_adj_assigns = analyse_adj(b, assign);
	next_row_assigns.insert(end(next_row_assigns), begin(next_adj_assigns), end(next_adj_assigns));

	return next_row_assigns;
}
void make_moves(board& b, std::vector<assignment>& moves) {
	print(moves);
	for (auto it = begin(moves); it != end(moves); ++it) {
		auto next_assigns = make_move(b, *it);
		//moves.insert(end(moves), begin(next_assigns), end(next_assigns));
		for (auto& new_assign : next_assigns)
			moves << new_assign;
		//debug
		std::cout << "Assigned: ";
		print(*it);
		std::cout << "; new moves = " << next_assigns.size() << ":\n";
		print(next_assigns);
		std::cout << "Rest of moves (total = " << moves.size() << "): ";
		std::for_each(it+1, end(moves), [](const auto& assign) { print(assign); std::cout << '\n'; });
		std::cout << "updated board = " << '\n';
		print(b);
	}
}

std::vector<assignment> read_init_assignments() {
	std::vector<assignment> moves;
	moves.reserve(board_size);
	
	std::istream_iterator<int> in(std::cin);
	for (int count = *in++; count; --count) {
		int x = *in++;
		int y = *in++;
		int value = *in++;
		moves.emplace_back(assignment{x, y, value});
		//moves << assignment{x, y, value};
	}
	
	return moves;
}

int main(int argc, char** argv)
{
	board b = starting_board;
	print(b);

	//std::vector<std::tuple<int, int, int>> moves;
	//std::vector<assignment> moves;
	//moves.reserve(810);

	auto moves = read_init_assignments();
	make_moves(b, moves);

	std::cout << "Finally: \n";
	print(b);
	std::cout << "moves made/filled cells = " << moves.size() << '\n';
	return 0;
}
