#ifndef BTREE_TRAITS_H
#define BTREE_TRAITS_H

#include <iostream>
#include <vector>
#include <algorithm>

template <class T, int S>
class BNode;

template <class T>
class SS_Traits{
public:
	using value_t = T;
	using container_t = std::vector<value_t>;
	
	class simple_search {
	public:
		int operator()(container_t a, int size, const value_t &v) const{
			int idx;
			for (idx = 0; idx < size && a[idx] < v; idx++);

			return idx;
		}
	};

	template <int S>
	class print_t{
	public:
		std::ostream& operator ()(BNode<SS_Traits, S> *root, std::ostream &out) {
			out << "Post order:" << std::endl;
			print(root, out);
			return out;
		}

		std::ostream& print(BNode<SS_Traits, S> *node, std::ostream &out) {
			if (node->ptrs[0]) {
				for (int i = 0; i <= node->size; i++) {
					print(node->ptrs[i], out);
				}
			}

			for (int i = 0; i < node->size; i++) {
				out << node->keys[i] << ' ';
			}

			return out;
		}
	};

	using functor_t = simple_search;
};

template <class T>
class BS_Traits{
public:
	using value_t = T;
	using container_t = std::vector<value_t>;
	
	class binary_search{
	public:
		int operator() (container_t a, int size, const value_t &v) const{
			auto it = std::lower_bound(a.begin(), a.begin() + size, v);
			return distance(a.begin(), it);
		}
	};

	template <int S>
	class print_t{
	public:
		std::ostream& operator ()(BNode<BS_Traits, S> *root, std::ostream &out) {
			out << "Pre order:" << std::endl;
			print(root, out);
			return out;
		}

		std::ostream& print(BNode<BS_Traits, S> *node, std::ostream &out) {
			for (int i = 0; i < node->size; i++) {
				out << node->keys[i] << ' ';
			}

			if (node->ptrs[0]) {
				for (int i = 0; i <= node->size; i++) {
					print(node->ptrs[i], out);
				}
			}

			return out;
		}
	};

	using functor_t = binary_search;
};

#endif
