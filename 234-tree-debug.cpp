/*
 * 작성자 : 윤정도
 * 작성일 : 22/05/26
 * 테스트 : VS2022
 *
 * 234 트리를 시뮬레이션 동작 과정만 보고 구현해보기
 * 시뮬레이션 사이트 : https://www.cs.usfca.edu/~galles/visualization/BTree.html
 */

#include <iostream>
#include <queue>
#include <crtdbg.h>
#include <cassert>

// C++ 17의 Structured Binding 기능을 사용하므로 무조건 C++ 17이 활성화되야한다. 
#if !_HAS_CXX17
#error C++ 17 feature must be enabled.
#error C++ 17 feature must be enabled.
#error C++ 17 feature must be enabled.
#error C++ 17 feature must be enabled.
#error C++ 17 feature must be enabled.
#error C++ 17 feature must be enabled.
#error C++ 17 feature must be enabled.
#endif

using namespace std;

class tree_node
{
public:
	tree_node() = default;

	// 데이터 1개가 담긴 노드를 생성하는 경우
	tree_node(int val1)
		: value_count(1) {
		values[0] = std::move(val1);
	}

	// is_n_node(4) = 4노드냐?
	bool is_n_node(const int n) const {
		return value_count == n;
	}
	
	// 데이터 추가
	// T타입 구현시에는 lvalue, rvalue 삽입 구분 구현이 필요함
	void add_value(const int val) {
		if (is_n_node(4)) {
			assert(false && "풀 노드");
			return;
		}

		// value_count 앞의 원소들은 이미 정렬된 상태이다.
		// 마지막 원소에 대해서만 삽입 정렬을 수행해주면 매우 효율적으로 정렬이 가능함
		int insertion_idx = value_count;
		values[value_count] = val;
		
		for (int i = value_count - 1; i >= 0; --i) {
			if (values[insertion_idx] < values[i]) {
				std::swap(values[insertion_idx], values[i]);
				insertion_idx = i;
			} else {
				break;
			}
		}

		value_count++;
	}

	int get_value_count() const {
		return value_count;
	}

	bool is_empty() const {
		return value_count == 0;
	}

	int get_value(const int idx) const {
		return values[idx];
	}

	// 원하는 위치의 데이터를 이동시켜서 가져온다.
	int&& extract_value(int idx) {
		return std::move(values[idx]);
	}

	// 마지막 위치의 데이터를 이동시켜서 가져온다.
	int&& extract_last_value() {
		return std::move(values[value_count - 1]);
	}

	// 원하는 위치에 데이터를 이동시켜준다.
	void replace_moved_value(const int idx, int&& val) {
		values[idx] = std::move(val);
	}

	int get_child_count() const {
		return child_count;
	}

	tree_node* get_child(int idx) const {
		return children[idx];
	}

	tree_node* get_last_child() const {
		assert(child_count > 0 && "자식이 없습니다.");
		return children[child_count - 1];
	}

	int& get_last_value() {
		assert(value_count > 0 && "데이터가 없습니다.");
		return values[value_count - 1];
	}

	int& get_first_value() {
		assert(value_count > 0 && "데이터가 없습니다.");
		return values[0];
	}

	// 자식을 맨 뒤에 추가한다.
	void push_back_child(tree_node* node) {
		if (node == nullptr)
			return;

		children[child_count++] = node;
		node->parent = this;
	}

	// 원하는 위치에 자식을 삽입한다.
	void insert_child(const int idx, tree_node* node) {
		if (node == nullptr)
			return;

		for (int i = child_count - 1; i >= idx; i--) {
			children[i + 1] = children[i];
		}

		children[idx] = node;
		child_count++;
		node->parent = this;
	}

	// 앞에서부터 데이터를 count 만큼 삭제시킨다.
	void pop_front_values(int count) {
		assert(count <= value_count && "실제 데이터 갯수보다 많이 없앨 수 없어요.");

		for (int i = count; i < value_count; ++i) {
			values[i - count] = values[i];
		}

		value_count -= count;
	}

	
	// 뒤에서부터 데이터를 count 만큼 삭제시킨다.
	void pop_back_values(int count) {
		assert(count <= value_count && "실제 데이터 갯수보다 많이 없앨 수 없어요.");
		value_count -= count;
	}

	// 앞에서부터 자식을 count 만큼 삭제시킨다.
	void pop_front_children(int count) {
		assert(count <= child_count && "실제 자식 갯수보다 많이 없앨 수 없어요.");

		for (int i = count; i < child_count; ++i) {
			children[i - count] = children[i];
		}

		child_count -= count;
	}

	// 단말 노드인가?
	bool is_leaf_node() const {
		return child_count == 0;
	}

	// 이 노드가 부모의 몇번째 자식인지
	int get_position_from_parent() const {
		if (parent == nullptr) {
			return -1;
		}

		for (int i = 0; i < parent->child_count; ++i) {
			if (parent->children[i] == this) {
				return i;
			}
		}

		return -1;
	}

	// 현재 노드에서 val값이 들어가야할 노드를 찾는다.
	tree_node* find_next_node(const int val) const {

		for (int i = 0; i < value_count; ++i) {
			if (val < values[i]) {
				return children[i];
			}
		}

		return children[value_count];
	}

	// 현재 노드에서 val값이 있는지 확인하고 없을 경우 val값이 존재할 수 있는 노드를 반환한다.
	tuple<tree_node*, int> find_next_node_with_value(const int val) {
		for (int i = 0; i < value_count; ++i) {

			if (val == values[i]) {
				return { this, i };
			}

			if (val < values[i]) {
				return { children[i], -1 };
			}
		}

		return { children[value_count], -1 };
	}

	tree_node* get_parent() const {
		return parent;
	}


	// this 노드가 데이터가 없어서 좌, 우측 형제노드에게서 데이터를 훕쳐온다.
	// 후속 작업을 위해 결과값과 좌, 우측 형제 노드 정보도 같이 반환한다.
	tuple<tree_node*, tree_node*, bool> steal_data_from_sibling() {
		const int current_pos = get_position_from_parent();

		if (current_pos == -1) {
			return { nullptr, nullptr, false };
		}

		tree_node* left_sibling = nullptr;
		tree_node* right_sibling = nullptr;

		if (current_pos > 0 && current_pos <= parent->value_count) {
			left_sibling = parent->children[current_pos - 1];
		}

		if (current_pos >= 0 && current_pos < parent->value_count) {
			right_sibling = parent->children[current_pos + 1];
		}

		if (left_sibling && left_sibling->value_count >= 2) {
			auto [last_value, last_child] = left_sibling->pop_last_value_and_last_child();

			add_value(parent->extract_value(current_pos - 1));
			parent->replace_moved_value(current_pos - 1, std::move(last_value));

			insert_child(0, last_child);

			return { left_sibling, right_sibling, true };
		}

		if (right_sibling && right_sibling->value_count >= 2) {
			auto [first_value, first_child] = right_sibling->pop_first_value_and_first_child();

			add_value(parent->extract_value(current_pos));
			parent->replace_moved_value(current_pos, std::move(first_value));
			right_sibling->pop_front_values(1);

			push_back_child(first_child);

			return { left_sibling, right_sibling, true };
		}

		return { left_sibling, right_sibling, false };
	}

	void remove_parent() {
		delete parent;
		parent = nullptr;
	}

	// 특정 위치의 값을 삭제한다.
	void remove_value_by_idx(const int idx) {
		assert(idx <= value_count && "올바르지 않은 인덱스입니다.");

		for (int i = idx; i < value_count - 1; ++i) {
			values[i] = values[i + 1];
		}

		value_count--;
	}

	// 특정 위치의 자식을 삭제한다.
	void remove_child_by_idx(const int idx) {
		delete children[idx];

		for (int i = idx; i < child_count - 1; ++i) {
			children[i] = children[i + 1];
		}

		child_count--;
	}

	void merge_right_parent_value() {
		// 현재 노드의 우측 부모 데이터를 자식으로 가져와서 병합한다.
		const int current_pos = get_position_from_parent();
		add_value(parent->extract_value(current_pos));

		// 해당 데이터 위치 이후의 데이터를 앞으로 당겨준다.
		parent->remove_value_by_idx(current_pos);

		// 우측에 빈 형제 노드는 삭제해준다.
		parent->remove_child_by_idx(current_pos + 1);
	}

	void merge_left_parent_value() {
		// 현재 노드의 좌측 부모 데이터를 가져와서 병합한다.
		const int current_pos = get_position_from_parent();
		add_value(parent->extract_value(current_pos - 1));

		// 해당 데이터 위치 이후의 데이터를 앞으로 당겨준다.
		parent->remove_value_by_idx(current_pos - 1);

		// 좌측에 빈 형제 노드는 삭제해준다.
		parent->remove_child_by_idx(current_pos - 1);
	}

private:
	// 맨 앞의 데이터와 자식을 가져온다.
	tuple<int&&, tree_node*> pop_first_value_and_first_child() {
		assert(value_count >= 2 && "steal_data_from_sibling() 함수에서 무조건 호출되야함");

		// 맨 앞의 데이터와 자식 노드를 빼왔으니 1칸씩 앞으로 당겨준다.
		int&& first_value = std::move(values[0]);

		// 성능 좀 더 올릴려면 밖에서 데이터가 move된 후 수동으로 수행하자.
		// 여기서 수행하는게 구조상 맞긴 한데 ㅠ
		// 나중에 테스트 돌려보고 거의 차이 안나면 원래대로 돌려야겠다.
		// pop_front_values(1); 

		tree_node* first_child = nullptr;

		if (child_count > 0) {
			first_child = children[0];
			pop_front_children(1);
		}
		
		return { std::move(first_value), first_child };
	}

	// 맨 뒤의 데이터와 자식을 가져온다.
	tuple<int&&, tree_node*> pop_last_value_and_last_child() {
		assert(value_count >= 2 && "steal_data_from_sibling() 함수에서 무조건 호출되야함");

		return {
			std::move(values[--value_count]),
			child_count == 0 ? nullptr : children[--child_count]
		};
	}

private:
	int values[4]{};
	int value_count = 0;
	tree_node* children[4 + 1]{};
	tree_node* parent = nullptr;
	int child_count = 0;
};


class balanced_tree
{
public:
	balanced_tree() = default;
	~balanced_tree() {
		clear();

		if (root) {
			delete root;
			root = nullptr;
		}
	}

	void insert(int val) {
		tree_node* insertion_node = find_insertion_recursive(root, val);

		insertion_node->add_value(val);

		// 4노드가 되어버린 경우 노드 분할을 진행한다.
		if (insertion_node->is_n_node(4)) {
			split_recursive(insertion_node);
		}
	}

	

	static tree_node* find_insertion_recursive(tree_node* node, const int val) {
		tree_node* next_node = node->find_next_node(val);

		// node의 자식중에서 val값이 들어갈 자식을 못 찾은 경우
		//  = node가 리프노드인 경우이다.
		if (next_node == nullptr) {
			return node;
		}

		return find_insertion_recursive(next_node, val);
	}

	

	void split_recursive(tree_node* node) {
		// 이 함수에 들어온 node는 4노드로 꽉찬 상태이다.
		tree_node* parent = node->get_parent();
		tree_node* child = nullptr;

		const int child_value = node->extract_value(0);
		const int parent_value = node->extract_value(1);

		node->pop_front_values(2);

		child = new tree_node(child_value);

		// 제 1규칙
		if (parent == nullptr) {
			parent = new tree_node(parent_value);
			parent->push_back_child(child);
			parent->push_back_child(node);

			root = parent;
		} else {
			// 제 2규칙
			parent->add_value(parent_value);
			
			const int insertion_pos = node->get_position_from_parent();
			parent->insert_child(insertion_pos, child);
		}


		// 분할을 수행해야하는 노드가 단말 노드가 아닌 경우
		if (!node->is_leaf_node()) {
			// 제 3규칙
			 tree_node* child_1 = node->get_child(0);
			 tree_node* child_2 = node->get_child(1);

			 node->pop_front_children(2);

			 child->push_back_child(child_1);
			 child->push_back_child(child_2);
		}

		// 데이터가 이동한 후 부모노드가 4노드가 된 경우
		// 부모 노드에 대해서도 마찬가지로 노드 분할 진행
		if (parent->is_n_node(4)) {
			split_recursive(parent);
		}
	}

	int get_height() const {
		return height_recursive(root, 1);
	}

	void print_height() const {
		cout << "트리의 높이 : " << get_height() << "\n";
	}

	void print_data_count() const {
		int count = 0;
		data_count_recursive(root, count);
		cout << "데이터 수 : " << count << "\n";
	}

	static int height_recursive(tree_node* node, int height) {
		if (node->is_leaf_node())
			return height;

		const int next_height = height + 1;

		for (int i = 0; i < node->get_child_count(); ++i) {
			tree_node* child = node->get_child(i);
			height = max(height_recursive(child, next_height), height);
		}

		return height;
	}

	static void data_count_recursive(tree_node* node, int& count) {
		count += node->get_value_count();
		for (int i = 0; i < node->get_child_count(); ++i) {
			data_count_recursive(node->get_child(i), count);
		}
	}

	void balance_leaf_node(tree_node* find_node, const int find_idx) {
		// 먼저 데이터를 제거해준다.
		find_node->remove_value_by_idx(find_idx);

		// 루트 노드는 예외적으로 비어있는 것을 허용해준다.
		if (find_node == root) {
			return;
		}

		// 남아있는 데이터가 있다면 더이상 추가 작업을 할 필요가 없다.
		if (!find_node->is_empty()) {
			return;
		}

		merge_recursive(find_node);
	}

	static tree_node* find_biggest_node_recursive(tree_node* node) {
		if (node->get_child_count() > 0)
			return find_biggest_node_recursive(node->get_last_child());

		return node;
	}

	void balance_not_leaf_node(tree_node* find_node, const int find_idx) {
		// 삭제되어야할 데이터기준의 좌측 서브트리에서 데이터가 가장 큰 녀석과 교체해준다.
		tree_node* left_node = find_node->get_child(find_idx);

		// 먼저 가장 큰 데이터가 존재하는 노드를 가져온다.
		// 좌측 서브트리에서 맨 우측 노드로 계속 이동하면 됨
		tree_node* biggest_node = find_biggest_node_recursive(left_node);	

		// biggest_node에서 가장 큰 값의 데이터를
		// 삭제되어야할 데이터 위치에 이동시켜준다.
		find_node->replace_moved_value(find_idx, biggest_node->extract_last_value());

		// biggest_node의 데이터 개수를 1 감소시킨다.
		biggest_node->pop_back_values(1);

		// 해당 노드가 비어버린 경우 병합을 시도한다.
		if (biggest_node->is_empty()) {
			merge_recursive(biggest_node);
		}
	}

	bool remove(int val) {
		// 루트노드 부터 시작해서 재귀적으로 찾아나간다.
		auto [find_node, find_idx] = search_recursive(root, val);

		if (find_idx == -1) {
			return false;
		}

		if (find_node->is_leaf_node()) {
			// 규칙1. 삭제한 데이터가 있던 노드가 단말 노드인 경우
			balance_leaf_node(find_node, find_idx);
		} else {
			// 규칙2. 삭제한 데이터가 있던 노드가 루트 또는 브랜치 노드인 경우
			balance_not_leaf_node(find_node, find_idx);
		}

		return true;
	}

	bool find(const int val) const {
		// 루트노드 부터 시작해서 재귀적으로 찾아나간다.
		auto [find_node, find_idx] = search_recursive(root, val);

		if (find_idx == -1) {
			return false;
		}

		return true;
	}

	static tuple<tree_node*, int> search_recursive(tree_node* node, const int val) {
		if (node == nullptr) {
			return { nullptr, -1 };
		}

		auto [next_node, find_idx] = node->find_next_node_with_value(val);

		if (find_idx >= 0) {
			return { next_node, find_idx };
		}

		// 데이터가 아예 없거나, 범위를 벗어나버린 경우 더이상 탐색하지 않고 종료한다.
		if (node->get_value_count() == 0) {
			return { nullptr, -1 };
		}

		return search_recursive(next_node, val);
	}

	void merge_recursive(tree_node* node) {

		// 병합시도 전에 좌측 또는 우측 형제 노드를 확인해서 데이터를 훔쳐올 수 있으면 훔쳐오도록 한다.
		auto [left_sibling, right_sibling, is_success] = node->steal_data_from_sibling();

		if (is_success) {
			return;
		}

		tree_node* parent = node->get_parent();
		// 이미 병합된 브랜치/루트 노드의 자식
		// 단말 노드인 경우에는 무조건 nullptr이다.
		tree_node* merged_child = node->get_child_count() > 0 ? 
								  node->get_child(0) : nullptr;
		
		if (left_sibling) {
			// 좌측 형제 노드가 있는 경우 좌 -> 우 병합을 시도한다.
			left_sibling->merge_right_parent_value();
			left_sibling->push_back_child(merged_child);
		} else if (right_sibling) {
			 // 우측 형제 노드가 있는 경우 우 -> 좌 병합을 시도한다.
			right_sibling->merge_left_parent_value();
			right_sibling->insert_child(0, merged_child);
		} else {
			// 형제 노드가 없는 경우는 루트 노드인 경우 뿐이다.
			merged_child->remove_parent();
			root = merged_child;
			return;
		}

		if (parent->is_empty()) {
			merge_recursive(parent);
		}
	}

	static void print_node(tree_node* node, int level) {
		cout << level << " : [";

		for (int i = 0; i < node->get_value_count(); ++i) {
			if (i == node->get_value_count() - 1) {
				cout << node->get_value(i);
			}
			else {
				cout << node->get_value(i) << ',';
			}
		}

		cout << "]\n";
	}

	void print_hierarchical() const {

		struct level_node {
			tree_node* node;
			int level;
		};

		queue<level_node> q;
		q.push({root, 1});
		print_node(root, 1);

		while (!q.empty()) {
			const tree_node* cur_node = q.front().node;
			const int cur_level = q.front().level;

			q.pop();

			for (int i = 0; i < cur_node->get_child_count(); ++i) {
				tree_node* next_node = cur_node->get_child(i);
				print_node(next_node, cur_level + 1);
				q.push({ next_node, cur_level + 1 });
			}
		}

		cout << "================= END =================\n";
	}


	void clear() {
		queue<tree_node*> q;
		q.push(root);

		while (!q.empty()) {
			const tree_node* cur_node = q.front();
			q.pop();

			for (int i = 0; i < cur_node->get_child_count(); ++i) {
				tree_node* next_node = cur_node->get_child(i);
				q.push(next_node);
			}

			// 루트 노드 빼고 삭제
			if (root != cur_node) {
				delete cur_node;
			}
		}
	}
private:
	tree_node* root = new tree_node;
};


int main() {

	// 메모리릭 체크 시작
	_CrtMemState memState_;
	_CrtMemCheckpoint(&memState_);

	{
		balanced_tree tree;

		for (int i = 0; i < 18; ++i) {
			tree.insert(50 * i);
		}

		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(400);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(600);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(250);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(50);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(0);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(100);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(350);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(650);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(450);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(500);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(200);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(700);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(750);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(800);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(550);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(850);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(150);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();

		tree.remove(300);
		tree.print_hierarchical();
		tree.print_data_count();
		tree.print_height();
	}

	// 메모리릭 체크 종료
	_CrtMemState stateNow, stateDiff;
	_CrtMemCheckpoint(&stateNow);
	const int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);

	if (diffResult) {
		cout << stateDiff.lSizes[1] << "바이트 메모리 릭\n";
		_CrtMemDumpStatistics(&stateDiff);
	}

    return 0;
}

