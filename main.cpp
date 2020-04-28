#include "btree/btree.h"
#include <iostream>
#include <vector> 
#include <set>

int main() {
	using btrait_t = BS_Traits<int>;
	BTree<btrait_t, 4> tree;

	srand(time(nullptr));

	std::set<int> st;
	for (int i = 0; i < 1000; i ++) {
		int x = rand() % 1000;

		if (!st.count(x)) {
			st.insert(x);
			tree.insert(x);
		}
	}

	for (int i = 0; i < 1000; i ++) {
		int x = rand() % 1000;
		assert(st.count(x) == tree.find(x));
	}

	std::cout << tree << std::endl;

	return 0;
}
