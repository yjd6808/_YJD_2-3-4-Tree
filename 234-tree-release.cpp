///*
// * 작성자 : 윤정도
// * 작성일 : 22/05/26
// * 테스트 : VS2022
// *
// * 234 트리를 시뮬레이션 동작 과정만 보고 구현해보기
// * 시뮬레이션 사이트 : https://www.cs.usfca.edu/~galles/visualization/BTree.html
// */
//
//#include <iostream>
//#include <queue>
//#include <crtdbg.h>
//#include <cassert>
//
//// C++ 17의 Structured Binding 기능을 사용하므로 무조건 C++ 17이 활성화되야한다. 
//#if !_HAS_CXX17
//#error C++ 17 feature must be enabled.
//#error C++ 17 feature must be enabled.
//#error C++ 17 feature must be enabled.
//#error C++ 17 feature must be enabled.
//#error C++ 17 feature must be enabled.
//#error C++ 17 feature must be enabled.
//#error C++ 17 feature must be enabled.
//#endif
//
//using namespace std;
//
//class tree_node
//{
//public:
//	tree_node() = default;
//
//	tree_node(int val1)
//		: value_count(1) {
//		values[0] = std::move(val1);
//	}
//
//	bool is_n_node(const int n) const {
//		return value_count == n;
//	}
//	
//	void add_value(const int val) {
//		if (is_n_node(4)) {
//			assert(false && "풀 노드");
//			return;
//		}
//
//		int insertion_idx = value_count;
//		values[value_count] = val;
//		
//		for (int i = value_count - 1; i >= 0; --i) {
//			if (values[insertion_idx] < values[i]) {
//				std::swap(values[insertion_idx], values[i]);
//				insertion_idx = i;
//			} else {
//				break;
//			}
//		}
//
//		value_count++;
//	}
//
//	int get_value_count() const {
//		return value_count;
//	}
//
//	bool is_empty() const {
//		return value_count == 0;
//	}
//
//	int get_value(const int idx) const {
//		return values[idx];
//	}
//
//	int&& extract_value(int idx) {
//		return std::move(values[idx]);
//	}
//
//	int&& extract_last_value() {
//		return std::move(values[value_count - 1]);
//	}
//
//	void replace_moved_value(const int idx, int&& val) {
//		values[idx] = std::move(val);
//	}
//
//	int get_child_count() const {
//		return child_count;
//	}
//
//	tree_node* get_child(int idx) const {
//		return children[idx];
//	}
//
//	tree_node* get_last_child() const {
//		assert(child_count > 0 && "자식이 없습니다.");
//		return children[child_count - 1];
//	}
//
//	int& get_last_value() {
//		assert(value_count > 0 && "데이터가 없습니다.");
//		return values[value_count - 1];
//	}
//
//	int& get_first_value() {
//		assert(value_count > 0 && "데이터가 없습니다.");
//		return values[0];
//	}
//
//	void push_back_child(tree_node* node) {
//		if (node == nullptr)
//			return;
//
//		children[child_count++] = node;
//		node->parent = this;
//	}
//
//	void insert_child(const int idx, tree_node* node) {
//		if (node == nullptr)
//			return;
//
//		for (int i = child_count - 1; i >= idx; i--) {
//			children[i + 1] = children[i];
//		}
//
//		children[idx] = node;
//		child_count++;
//		node->parent = this;
//	}
//
//	void pop_front_values(int count) {
//		for (int i = count; i < value_count; ++i) {
//			values[i - count] = values[i];
//		}
//
//		value_count -= count;
//	}
//
//	void pop_back_values(int count) {
//		value_count -= count;
//	}
//
//	void pop_front_children(int count) {
//		for (int i = count; i < child_count; ++i) {
//			children[i - count] = children[i];
//		}
//
//		child_count -= count;
//	}
//
//	bool is_leaf_node() const {
//		return child_count == 0;
//	}
//
//	int get_position_from_parent() const {
//		if (parent == nullptr) {
//			return -1;
//		}
//
//		for (int i = 0; i < parent->child_count; ++i) {
//			if (parent->children[i] == this) {
//				return i;
//			}
//		}
//
//		return -1;
//	}
//
//	tree_node* find_next_node(const int val) const {
//
//		for (int i = 0; i < value_count; ++i) {
//			if (val < values[i]) {
//				return children[i];
//			}
//		}
//
//		return children[value_count];
//	}
//
//	tuple<tree_node*, int> find_next_node_with_value(const int val) {
//		for (int i = 0; i < value_count; ++i) {
//
//			if (val == values[i]) {
//				return { this, i };
//			}
//
//			if (val < values[i]) {
//				return { children[i], -1 };
//			}
//		}
//
//		return { children[value_count], -1 };
//	}
//
//	tree_node* get_parent() const {
//		return parent;
//	}
//
//
//	tuple<tree_node*, tree_node*, bool> steal_data_from_sibling() {
//		const int current_pos = get_position_from_parent();
//
//		if (current_pos == -1) {
//			return { nullptr, nullptr, false };
//		}
//
//		tree_node* left_sibling = nullptr;
//		tree_node* right_sibling = nullptr;
//
//		if (current_pos > 0 && current_pos <= parent->value_count) {
//			left_sibling = parent->children[current_pos - 1];
//		}
//
//		if (current_pos >= 0 && current_pos < parent->value_count) {
//			right_sibling = parent->children[current_pos + 1];
//		}
//
//		if (left_sibling && left_sibling->value_count >= 2) {
//			auto [last_value, last_child] = left_sibling->pop_last_value_and_last_child();
//
//			add_value(parent->extract_value(current_pos - 1));
//			parent->replace_moved_value(current_pos - 1, std::move(last_value));
//
//			insert_child(0, last_child);
//
//			return { left_sibling, right_sibling, true };
//		}
//
//		if (right_sibling && right_sibling->value_count >= 2) {
//			auto [first_value, first_child] = right_sibling->pop_first_value_and_first_child();
//
//			add_value(parent->extract_value(current_pos));
//			parent->replace_moved_value(current_pos, std::move(first_value));
//			right_sibling->pop_front_values(1);
//
//			push_back_child(first_child);
//
//			return { left_sibling, right_sibling, true };
//		}
//
//		return { left_sibling, right_sibling, false };
//	}
//
//	void remove_parent() {
//		delete parent;
//		parent = nullptr;
//	}
//
//	void remove_value_by_idx(const int idx) {
//		for (int i = idx; i < value_count - 1; ++i) {
//			values[i] = values[i + 1];
//		}
//
//		value_count--;
//	}
//
//	void remove_child_by_idx(const int idx) {
//		delete children[idx];
//
//		for (int i = idx; i < child_count - 1; ++i) {
//			children[i] = children[i + 1];
//		}
//
//		child_count--;
//	}
//
//	void merge_right_parent_value() {
//		const int current_pos = get_position_from_parent();
//		add_value(parent->extract_value(current_pos));
//
//		parent->remove_value_by_idx(current_pos);
//		parent->remove_child_by_idx(current_pos + 1);
//	}
//
//	void merge_left_parent_value() {
//		const int current_pos = get_position_from_parent();
//		add_value(parent->extract_value(current_pos - 1));
//
//		parent->remove_value_by_idx(current_pos - 1);
//		parent->remove_child_by_idx(current_pos - 1);
//	}
//
//private:
//	tuple<int&&, tree_node*> pop_first_value_and_first_child() {
//		int&& first_value = std::move(values[0]);
//		// pop_front_values(1); 
//
//		tree_node* first_child = nullptr;
//
//		if (child_count > 0) {
//			first_child = children[0];
//			pop_front_children(1);
//		}
//		
//		return { std::move(first_value), first_child };
//	}
//
//	// 맨 뒤의 데이터와 자식을 가져온다.
//	tuple<int&&, tree_node*> pop_last_value_and_last_child() {
//		return {
//			std::move(values[--value_count]),
//			child_count == 0 ? nullptr : children[--child_count]
//		};
//	}
//
//private:
//	int values[4]{};
//	int value_count = 0;
//	tree_node* children[4 + 1]{};
//	tree_node* parent = nullptr;
//	int child_count = 0;
//};
//
//
//class balanced_tree
//{
//public:
//	balanced_tree() = default;
//	~balanced_tree() {
//		clear();
//
//		if (root) {
//			delete root;
//			root = nullptr;
//		}
//	}
//
//	void insert(int val) {
//		tree_node* insertion_node = find_insertion_recursive(root, val);
//
//		insertion_node->add_value(val);
//
//		if (insertion_node->is_n_node(4)) {
//			split_recursive(insertion_node);
//		}
//	}
//
//	
//
//	static tree_node* find_insertion_recursive(tree_node* node, const int val) {
//		tree_node* next_node = node->find_next_node(val);
//
//		if (next_node == nullptr) {
//			return node;
//		}
//
//		return find_insertion_recursive(next_node, val);
//	}
//
//	
//	void split_recursive(tree_node* node) {
//		tree_node* parent = node->get_parent();
//		tree_node* child = nullptr;
//
//		const int child_value = node->extract_value(0);
//		const int parent_value = node->extract_value(1);
//
//		node->pop_front_values(2);
//
//		child = new tree_node(child_value);
//
//		// 제 1규칙
//		if (parent == nullptr) {
//			parent = new tree_node(parent_value);
//			parent->push_back_child(child);
//			parent->push_back_child(node);
//
//			root = parent;
//		} else {
//			// 제 2규칙
//			parent->add_value(parent_value);
//			
//			const int insertion_pos = node->get_position_from_parent();
//			parent->insert_child(insertion_pos, child);
//		}
//
//
//		if (!node->is_leaf_node()) {
//			// 제 3규칙
//			 tree_node* child_1 = node->get_child(0);
//			 tree_node* child_2 = node->get_child(1);
//
//			 node->pop_front_children(2);
//
//			 child->push_back_child(child_1);
//			 child->push_back_child(child_2);
//		}
//
//		if (parent->is_n_node(4)) {
//			split_recursive(parent);
//		}
//	}
//
//	void balance_leaf_node(tree_node* find_node, const int find_idx) {
//		find_node->remove_value_by_idx(find_idx);
//
//		if (find_node == root) {
//			return;
//		}
//
//		if (!find_node->is_empty()) {
//			return;
//		}
//
//		merge_recursive(find_node);
//	}
//
//	static tree_node* find_biggest_node_recursive(tree_node* node) {
//		if (node->get_child_count() > 0)
//			return find_biggest_node_recursive(node->get_last_child());
//
//		return node;
//	}
//
//	void balance_not_leaf_node(tree_node* find_node, const int find_idx) {
//		tree_node* left_node = find_node->get_child(find_idx);
//		tree_node* biggest_node = find_biggest_node_recursive(left_node);	
//
//		find_node->replace_moved_value(find_idx, biggest_node->extract_last_value());
//
//		biggest_node->pop_back_values(1);
//
//		if (biggest_node->is_empty()) {
//			merge_recursive(biggest_node);
//		}
//	}
//
//	bool remove(int val) {
//		auto [find_node, find_idx] = search_recursive(root, val);
//
//		if (find_idx == -1) {
//			return false;
//		}
//
//		if (find_node->is_leaf_node()) {
//			balance_leaf_node(find_node, find_idx);
//		} else {
//			balance_not_leaf_node(find_node, find_idx);
//		}
//
//		return true;
//	}
//
//	bool find(const int val) const {
//		auto [find_node, find_idx] = search_recursive(root, val);
//
//		if (find_idx == -1) {
//			return false;
//		}
//
//		return true;
//	}
//
//	static tuple<tree_node*, int> search_recursive(tree_node* node, const int val) {
//		if (node == nullptr) {
//			return { nullptr, -1 };
//		}
//
//		auto [next_node, find_idx] = node->find_next_node_with_value(val);
//
//		if (find_idx >= 0) {
//			return { next_node, find_idx };
//		}
//
//		if (node->get_value_count() == 0) {
//			return { nullptr, -1 };
//		}
//
//		return search_recursive(next_node, val);
//	}
//
//	void merge_recursive(tree_node* node) {
//
//		auto [left_sibling, right_sibling, is_success] = node->steal_data_from_sibling();
//
//		if (is_success) {
//			return;
//		}
//
//		tree_node* parent = node->get_parent();
//		tree_node* merged_child = node->get_child_count() > 0 ? node->get_child(0) : nullptr;
//		
//		if (left_sibling) {
//			left_sibling->merge_right_parent_value();
//			left_sibling->push_back_child(merged_child);
//		} else if (right_sibling) {
//			right_sibling->merge_left_parent_value();
//			right_sibling->insert_child(0, merged_child);
//		} else {
//			merged_child->remove_parent();
//			root = merged_child;
//			return;
//		}
//
//		if (parent->is_empty()) {
//			merge_recursive(parent);
//		}
//	}
//
//	void clear() {
//		queue<tree_node*> q;
//		q.push(root);
//
//		while (!q.empty()) {
//			const tree_node* cur_node = q.front();
//			q.pop();
//
//			for (int i = 0; i < cur_node->get_child_count(); ++i) {
//				tree_node* next_node = cur_node->get_child(i);
//				q.push(next_node);
//			}
//
//			if (root != cur_node) {
//				delete cur_node;
//			}
//		}
//	}
//private:
//	tree_node* root = new tree_node;
//};
//
//
//int main() {
//
//	balanced_tree tree;
//
//	tree.insert(1);
//	tree.insert(2);
//	tree.insert(3);
//	tree.insert(4);
//	tree.insert(5);
//
//	tree.remove(1);
//	tree.remove(2);
//	tree.remove(3);
//	tree.remove(4);
//	tree.remove(5);
//
//    return 0;
//}
//
