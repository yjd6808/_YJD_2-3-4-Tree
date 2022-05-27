/*
 * �ۼ��� : ������
 * �ۼ��� : 22/05/26
 * �׽�Ʈ : VS2022
 *
 * 234 Ʈ���� �ùķ��̼� ���� ������ ���� �����غ���
 * �ùķ��̼� ����Ʈ : https://www.cs.usfca.edu/~galles/visualization/BTree.html
 */

#include <iostream>
#include <queue>
#include <crtdbg.h>
#include <cassert>

// C++ 17�� Structured Binding ����� ����ϹǷ� ������ C++ 17�� Ȱ��ȭ�Ǿ��Ѵ�. 
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

	// ������ 1���� ��� ��带 �����ϴ� ���
	tree_node(int val1)
		: value_count(1) {
		values[0] = std::move(val1);
	}

	// is_n_node(4) = 4����?
	bool is_n_node(const int n) const {
		return value_count == n;
	}
	
	// ������ �߰�
	// TŸ�� �����ÿ��� lvalue, rvalue ���� ���� ������ �ʿ���
	void add_value(const int val) {
		if (is_n_node(4)) {
			assert(false && "Ǯ ���");
			return;
		}

		// value_count ���� ���ҵ��� �̹� ���ĵ� �����̴�.
		// ������ ���ҿ� ���ؼ��� ���� ������ �������ָ� �ſ� ȿ�������� ������ ������
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

	// ���ϴ� ��ġ�� �����͸� �̵����Ѽ� �����´�.
	int&& extract_value(int idx) {
		return std::move(values[idx]);
	}

	// ������ ��ġ�� �����͸� �̵����Ѽ� �����´�.
	int&& extract_last_value() {
		return std::move(values[value_count - 1]);
	}

	// ���ϴ� ��ġ�� �����͸� �̵������ش�.
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
		assert(child_count > 0 && "�ڽ��� �����ϴ�.");
		return children[child_count - 1];
	}

	int& get_last_value() {
		assert(value_count > 0 && "�����Ͱ� �����ϴ�.");
		return values[value_count - 1];
	}

	int& get_first_value() {
		assert(value_count > 0 && "�����Ͱ� �����ϴ�.");
		return values[0];
	}

	// �ڽ��� �� �ڿ� �߰��Ѵ�.
	void push_back_child(tree_node* node) {
		if (node == nullptr)
			return;

		children[child_count++] = node;
		node->parent = this;
	}

	// ���ϴ� ��ġ�� �ڽ��� �����Ѵ�.
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

	// �տ������� �����͸� count ��ŭ ������Ų��.
	void pop_front_values(int count) {
		assert(count <= value_count && "���� ������ �������� ���� ���� �� �����.");

		for (int i = count; i < value_count; ++i) {
			values[i - count] = values[i];
		}

		value_count -= count;
	}

	
	// �ڿ������� �����͸� count ��ŭ ������Ų��.
	void pop_back_values(int count) {
		assert(count <= value_count && "���� ������ �������� ���� ���� �� �����.");
		value_count -= count;
	}

	// �տ������� �ڽ��� count ��ŭ ������Ų��.
	void pop_front_children(int count) {
		assert(count <= child_count && "���� �ڽ� �������� ���� ���� �� �����.");

		for (int i = count; i < child_count; ++i) {
			children[i - count] = children[i];
		}

		child_count -= count;
	}

	// �ܸ� ����ΰ�?
	bool is_leaf_node() const {
		return child_count == 0;
	}

	// �� ��尡 �θ��� ���° �ڽ�����
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

	// ���� ��忡�� val���� ������ ��带 ã�´�.
	tree_node* find_next_node(const int val) const {

		for (int i = 0; i < value_count; ++i) {
			if (val < values[i]) {
				return children[i];
			}
		}

		return children[value_count];
	}

	// ���� ��忡�� val���� �ִ��� Ȯ���ϰ� ���� ��� val���� ������ �� �ִ� ��带 ��ȯ�Ѵ�.
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


	// this ��尡 �����Ͱ� ��� ��, ���� ������忡�Լ� �����͸� �g�Ŀ´�.
	// �ļ� �۾��� ���� ������� ��, ���� ���� ��� ������ ���� ��ȯ�Ѵ�.
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

	// Ư�� ��ġ�� ���� �����Ѵ�.
	void remove_value_by_idx(const int idx) {
		assert(idx <= value_count && "�ùٸ��� ���� �ε����Դϴ�.");

		for (int i = idx; i < value_count - 1; ++i) {
			values[i] = values[i + 1];
		}

		value_count--;
	}

	// Ư�� ��ġ�� �ڽ��� �����Ѵ�.
	void remove_child_by_idx(const int idx) {
		delete children[idx];

		for (int i = idx; i < child_count - 1; ++i) {
			children[i] = children[i + 1];
		}

		child_count--;
	}

	void merge_right_parent_value() {
		// ���� ����� ���� �θ� �����͸� �ڽ����� �����ͼ� �����Ѵ�.
		const int current_pos = get_position_from_parent();
		add_value(parent->extract_value(current_pos));

		// �ش� ������ ��ġ ������ �����͸� ������ ����ش�.
		parent->remove_value_by_idx(current_pos);

		// ������ �� ���� ���� �������ش�.
		parent->remove_child_by_idx(current_pos + 1);
	}

	void merge_left_parent_value() {
		// ���� ����� ���� �θ� �����͸� �����ͼ� �����Ѵ�.
		const int current_pos = get_position_from_parent();
		add_value(parent->extract_value(current_pos - 1));

		// �ش� ������ ��ġ ������ �����͸� ������ ����ش�.
		parent->remove_value_by_idx(current_pos - 1);

		// ������ �� ���� ���� �������ش�.
		parent->remove_child_by_idx(current_pos - 1);
	}

private:
	// �� ���� �����Ϳ� �ڽ��� �����´�.
	tuple<int&&, tree_node*> pop_first_value_and_first_child() {
		assert(value_count >= 2 && "steal_data_from_sibling() �Լ����� ������ ȣ��Ǿ���");

		// �� ���� �����Ϳ� �ڽ� ��带 �������� 1ĭ�� ������ ����ش�.
		int&& first_value = std::move(values[0]);

		// ���� �� �� �ø����� �ۿ��� �����Ͱ� move�� �� �������� ��������.
		// ���⼭ �����ϴ°� ������ �±� �ѵ� ��
		// ���߿� �׽�Ʈ �������� ���� ���� �ȳ��� ������� �����߰ڴ�.
		// pop_front_values(1); 

		tree_node* first_child = nullptr;

		if (child_count > 0) {
			first_child = children[0];
			pop_front_children(1);
		}
		
		return { std::move(first_value), first_child };
	}

	// �� ���� �����Ϳ� �ڽ��� �����´�.
	tuple<int&&, tree_node*> pop_last_value_and_last_child() {
		assert(value_count >= 2 && "steal_data_from_sibling() �Լ����� ������ ȣ��Ǿ���");

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

		// 4��尡 �Ǿ���� ��� ��� ������ �����Ѵ�.
		if (insertion_node->is_n_node(4)) {
			split_recursive(insertion_node);
		}
	}

	

	static tree_node* find_insertion_recursive(tree_node* node, const int val) {
		tree_node* next_node = node->find_next_node(val);

		// node�� �ڽ��߿��� val���� �� �ڽ��� �� ã�� ���
		//  = node�� ��������� ����̴�.
		if (next_node == nullptr) {
			return node;
		}

		return find_insertion_recursive(next_node, val);
	}

	

	void split_recursive(tree_node* node) {
		// �� �Լ��� ���� node�� 4���� ���� �����̴�.
		tree_node* parent = node->get_parent();
		tree_node* child = nullptr;

		const int child_value = node->extract_value(0);
		const int parent_value = node->extract_value(1);

		node->pop_front_values(2);

		child = new tree_node(child_value);

		// �� 1��Ģ
		if (parent == nullptr) {
			parent = new tree_node(parent_value);
			parent->push_back_child(child);
			parent->push_back_child(node);

			root = parent;
		} else {
			// �� 2��Ģ
			parent->add_value(parent_value);
			
			const int insertion_pos = node->get_position_from_parent();
			parent->insert_child(insertion_pos, child);
		}


		// ������ �����ؾ��ϴ� ��尡 �ܸ� ��尡 �ƴ� ���
		if (!node->is_leaf_node()) {
			// �� 3��Ģ
			 tree_node* child_1 = node->get_child(0);
			 tree_node* child_2 = node->get_child(1);

			 node->pop_front_children(2);

			 child->push_back_child(child_1);
			 child->push_back_child(child_2);
		}

		// �����Ͱ� �̵��� �� �θ��尡 4��尡 �� ���
		// �θ� ��忡 ���ؼ��� ���������� ��� ���� ����
		if (parent->is_n_node(4)) {
			split_recursive(parent);
		}
	}

	int get_height() const {
		return height_recursive(root, 1);
	}

	void print_height() const {
		cout << "Ʈ���� ���� : " << get_height() << "\n";
	}

	void print_data_count() const {
		int count = 0;
		data_count_recursive(root, count);
		cout << "������ �� : " << count << "\n";
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
		// ���� �����͸� �������ش�.
		find_node->remove_value_by_idx(find_idx);

		// ��Ʈ ���� ���������� ����ִ� ���� ������ش�.
		if (find_node == root) {
			return;
		}

		// �����ִ� �����Ͱ� �ִٸ� ���̻� �߰� �۾��� �� �ʿ䰡 ����.
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
		// �����Ǿ���� �����ͱ����� ���� ����Ʈ������ �����Ͱ� ���� ū �༮�� ��ü���ش�.
		tree_node* left_node = find_node->get_child(find_idx);

		// ���� ���� ū �����Ͱ� �����ϴ� ��带 �����´�.
		// ���� ����Ʈ������ �� ���� ���� ��� �̵��ϸ� ��
		tree_node* biggest_node = find_biggest_node_recursive(left_node);	

		// biggest_node���� ���� ū ���� �����͸�
		// �����Ǿ���� ������ ��ġ�� �̵������ش�.
		find_node->replace_moved_value(find_idx, biggest_node->extract_last_value());

		// biggest_node�� ������ ������ 1 ���ҽ�Ų��.
		biggest_node->pop_back_values(1);

		// �ش� ��尡 ������ ��� ������ �õ��Ѵ�.
		if (biggest_node->is_empty()) {
			merge_recursive(biggest_node);
		}
	}

	bool remove(int val) {
		// ��Ʈ��� ���� �����ؼ� ��������� ã�Ƴ�����.
		auto [find_node, find_idx] = search_recursive(root, val);

		if (find_idx == -1) {
			return false;
		}

		if (find_node->is_leaf_node()) {
			// ��Ģ1. ������ �����Ͱ� �ִ� ��尡 �ܸ� ����� ���
			balance_leaf_node(find_node, find_idx);
		} else {
			// ��Ģ2. ������ �����Ͱ� �ִ� ��尡 ��Ʈ �Ǵ� �귣ġ ����� ���
			balance_not_leaf_node(find_node, find_idx);
		}

		return true;
	}

	bool find(const int val) const {
		// ��Ʈ��� ���� �����ؼ� ��������� ã�Ƴ�����.
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

		// �����Ͱ� �ƿ� ���ų�, ������ ������� ��� ���̻� Ž������ �ʰ� �����Ѵ�.
		if (node->get_value_count() == 0) {
			return { nullptr, -1 };
		}

		return search_recursive(next_node, val);
	}

	void merge_recursive(tree_node* node) {

		// ���սõ� ���� ���� �Ǵ� ���� ���� ��带 Ȯ���ؼ� �����͸� ���Ŀ� �� ������ ���Ŀ����� �Ѵ�.
		auto [left_sibling, right_sibling, is_success] = node->steal_data_from_sibling();

		if (is_success) {
			return;
		}

		tree_node* parent = node->get_parent();
		// �̹� ���յ� �귣ġ/��Ʈ ����� �ڽ�
		// �ܸ� ����� ��쿡�� ������ nullptr�̴�.
		tree_node* merged_child = node->get_child_count() > 0 ? 
								  node->get_child(0) : nullptr;
		
		if (left_sibling) {
			// ���� ���� ��尡 �ִ� ��� �� -> �� ������ �õ��Ѵ�.
			left_sibling->merge_right_parent_value();
			left_sibling->push_back_child(merged_child);
		} else if (right_sibling) {
			 // ���� ���� ��尡 �ִ� ��� �� -> �� ������ �õ��Ѵ�.
			right_sibling->merge_left_parent_value();
			right_sibling->insert_child(0, merged_child);
		} else {
			// ���� ��尡 ���� ���� ��Ʈ ����� ��� ���̴�.
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

			// ��Ʈ ��� ���� ����
			if (root != cur_node) {
				delete cur_node;
			}
		}
	}
private:
	tree_node* root = new tree_node;
};


int main() {

	// �޸𸮸� üũ ����
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

	// �޸𸮸� üũ ����
	_CrtMemState stateNow, stateDiff;
	_CrtMemCheckpoint(&stateNow);
	const int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);

	if (diffResult) {
		cout << stateDiff.lSizes[1] << "����Ʈ �޸� ��\n";
		_CrtMemDumpStatistics(&stateDiff);
	}

    return 0;
}

