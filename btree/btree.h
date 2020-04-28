#ifndef BTREE_H
#define BTREE_H

#include "traits.h"
#include <iostream>
#include <algorithm>
#include <vector>

template <class T, int S> 
class BNode {
public:
	using value_t = typename T::value_t;
	using container_t = typename T::container_t;
	using pcontainer_t = std::vector<BNode*>;

	enum state_t{
		NORMAL,
		OVERFLOW,
		UNDERFLOW
	};

	container_t keys;
	pcontainer_t ptrs;
	int size;
	int order;

	BNode(void): order(S), size(0) {
		keys = container_t(order + 1, 0);
		ptrs = pcontainer_t(order + 2, nullptr);
	}

	void insertInNode(int idx, value_t val) {
		for (int i = size; i > idx; i--) {
			keys[i] = keys[i - 1];
			ptrs[i + 1] = ptrs[i];
		}
		keys[idx] = val;
		ptrs[idx + 1] = ptrs[idx];
		size++;
	}

	state_t getstate_t() {
		if (size * 2 < order) {
			return UNDERFLOW;
		}
		if (size > order) {
			return OVERFLOW;
		}
		return NORMAL;
	}

	void killself() {
		if (ptrs[0]) {
			for (int i = 0; i <= size; i++) {
				ptrs[i]->killself();
			}
		}
		delete this;
	}
};

template <class T, int S>
class BTree {
public: 
	using value_t = typename T::value_t;
	using search_t = typename T::functor_t;
	using print_t = typename T::template print_t<S>;
	using Node = BNode<T, S>;
	using status_code_t = enum Node::state_t;

	Node* root;
	print_t print;
	search_t search;

	BTree(void) {
		root = new Node();
	}

	~BTree(void){
		if (root) {
			root->killself();
		}
	}

	void insert(const value_t &val = 0){
		status_code_t state = insert(root, val);
		if (state == Node::OVERFLOW) {
			split(root, 0, true);
		}
	}

	bool find(const value_t &val = 0) const{
		return find(root, val);
	}

	template <typename _T, int _S>
	friend std::ostream& operator<<(std::ostream& out, BTree<_T,_S> &tree){
		tree.print(tree.root, out);
		return out;
	}

private:
	status_code_t insert(Node* node, const value_t& val) {
		int idx = search(node->keys, node->size, val);

		if (node->ptrs[idx] != nullptr) {
			status_code_t state = insert(node->ptrs[idx], val);
			if (state == Node::OVERFLOW) {
				split(node, idx);
			}
		}
		else {
			node->insertInNode(idx, val);
		}

		return node->getstate_t();
	}

	void split(Node *parent, int pos, bool isRoot = false) {
		Node *overflowedNode = isRoot ? parent : parent->ptrs[pos];

		Node *child1 = isRoot ? new Node() : overflowedNode;
		Node *child2 = new Node();

		child1->size = 0;

		int iter = 0;
		int i;
		for (int i = 0; iter < S / 2; i++) {
			child1->ptrs[i] = overflowedNode->ptrs[iter];
			child1->keys[i] = overflowedNode->keys[iter];
			child1->size++;

			iter++;
		}

		child1->ptrs[iter] = overflowedNode->ptrs[iter];
		if (!isRoot) {
			parent->insertInNode(pos, overflowedNode->keys[iter]);
		}
		
		iter++;

		for (i = 0; iter <= S; i++) {
			child2->ptrs[i] = overflowedNode->ptrs[iter];
			child2->keys[i] = overflowedNode->keys[iter];
			child2->size++;

			iter++;
		}
		child2->ptrs[i] = overflowedNode->ptrs[iter];

		parent->ptrs[pos] = child1;
		parent->ptrs[pos + 1] = child2;

		if (isRoot) {
			parent->keys[0] = overflowedNode->keys[S / 2];
			parent->size = 1;
		}
	}

	bool find(Node* node, const value_t &val = 0) const{
		int idx = search(node->keys, node->size, val);
		
		if (node->keys[idx] == val) {
			return true;
		}

		if (node->ptrs[idx]) {
			return find(node->ptrs[idx], val);
		}

		return false;
	}

};

#endif
