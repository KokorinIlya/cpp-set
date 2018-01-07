#pragma once
#include <filesystem>
#include <iostream>

template <typename T> struct set;

template<typename T>
void swap(set<T>& a, set<T>& b) noexcept;

template <typename T> struct set
{
public:
	friend void swap<T>(set& a, set& b) noexcept;

	struct BaseNode
	{
		friend struct set;
	public:
		BaseNode(BaseNode* _left, BaseNode* _right, BaseNode* _parent) : left(_left), right(_right), parent(_parent) {}
		BaseNode() : left(nullptr), right(nullptr), parent(nullptr) {}
	private:
		BaseNode* left;
		BaseNode* right;
		BaseNode* parent;
	};

	struct Node : public BaseNode
	{
		friend struct set;
	public:
		Node() = delete;
		Node(BaseNode* _left, BaseNode* _right, BaseNode* _parent, const T& _data) : BaseNode(_left, _right, _parent), data(_data) {}
	private:
		T data;
	};

	struct iterator
	{
		friend struct set;
	public:
		iterator() = delete;
		iterator(BaseNode* node) : nodePointer(node) {}

		iterator& operator++()
		{
			if (nodePointer->right != nullptr)
			{
				nodePointer = goLeft(nodePointer -> right);
				return *this;
			}
			BaseNode* y = nodePointer->parent;
			while (y != nullptr && nodePointer == y->right)
			{
				nodePointer = y;
				y = y->parent;
			}
			nodePointer = y;
			return *this;
		}

		iterator& operator--()
		{
			if (nodePointer->left != nullptr)
			{
				nodePointer = goRight(nodePointer->left);
				return *this;
			}
			BaseNode* y = nodePointer->parent;
			while (y != nullptr && nodePointer == y->left)
			{
				nodePointer = y;
				y = y->parent;
			}
			nodePointer = y;
			return *this;
		}

		iterator operator++(int notNeed)
		{
			iterator temp(*this);
			this->operator++();
			return temp;
		}

		iterator operator--(int notNeed)
		{
			iterator temp(*this);
			this->operator--();
			return temp;
		}

		T const& operator*() const
		{
			return static_cast<Node*>(nodePointer)->data;
		}

		bool operator==(iterator const& other) const
		{
			return other.nodePointer == nodePointer;
		}

		bool operator!=(iterator const& other) const
		{
			return other.nodePointer != nodePointer;
		}

		typedef std::ptrdiff_t difference_type;
		typedef T value_type;
		typedef T const* pointer;
		typedef T const& reference;
		typedef std::bidirectional_iterator_tag iterator_category;

	private:
		 BaseNode* nodePointer;
	};


	typedef iterator const_iterator;

	set() : root() {}
	set(set const& other) : root()
	{
		root.left = constructNode(static_cast<Node*>(other.root.left), &root);
	}

	~set()
	{
		clear();
	}

	set& operator=(set const& other)
	{
		set temp(other);
		swap(*this, temp);
		return *this;
	}

	bool empty() const
	{
		return root.left == nullptr;
	}
	
	void clear()
	{
		deleteNode(root.left);
	}

	iterator insert(T const& x)
	{
		if (root.left == nullptr)
		{
			root.left = new Node(nullptr, nullptr, &root, x);
			return iterator(root.left);
		}
		Node* result = insertNode(static_cast<Node*>(root.left), x);
		if (result == nullptr)
		{
			return iterator(&root);
		}
		return iterator(result);
	}

	iterator erase(iterator toErase)
	{
		iterator result(toErase);
		++result;
		eraseNode(static_cast<Node*>(toErase.nodePointer), *(toErase));
		return result;
	}

	iterator find(T const& x) const
	{
		Node* result = findNode(static_cast<Node*>(root.left), x);
		if (result == nullptr)
		{
			return iterator(&root);
		}
		return iterator(result);
	}

	iterator lower_bound(T const& x) const
	{
		if (root.left == nullptr)
		{
			return iterator(&root);
		}
		Node* result = lowerBoundNode(static_cast<Node*>(root.left), x);
		if (result == nullptr)
		{
			return iterator(&root);
		}
		return iterator(result);
	}

	iterator upper_bound(T const& x) const
	{
		if (root.left == nullptr)
		{
			return iterator(&root);
		}
		iterator result = find(x);
		if (result != end())
		{
			return ++result;
		}
		return lower_bound(x);
	}

	const_iterator begin() const
	{
		if (root.left == nullptr)
		{
			return iterator(&root);
		}
		Node* result = static_cast<Node*>(goLeft(root.left));
		return iterator(result);
	}

	const_iterator end() const
	{
		return iterator(&root);
	}

	typedef std::reverse_iterator<iterator> reverse_iterator;

	reverse_iterator rbegin() const
	{
		return reverse_iterator(end());
	}

	reverse_iterator rend() const
	{
		return reverse_iterator(begin());
	}

private:
	void deleteNode(BaseNode* _root);
	Node* constructNode(Node* constructor, BaseNode* _parent);
	Node* findNode(Node* _root, T const&) const;
	Node* insertNode(Node* _root, T const& x);
	Node* eraseNode(Node* root, T const& x);
	Node* lowerBoundNode(Node* root, T const& x) const;

	static BaseNode* goLeft(BaseNode* _root);
	static BaseNode* goRight(BaseNode* _root);

	mutable BaseNode root;
};

template<typename T>
void swap(set<T>& a, set<T>& b) noexcept
{
	using std::swap;
	swap(a.root, b.root);
}

template <typename T>
void set<T>::deleteNode(BaseNode* _root)
{
	if (_root == nullptr)
	{
		return;
	}
	deleteNode(_root->left);
	deleteNode(_root->right);
	delete _root;
}

template <typename T>
typename set<T>::Node* set<T>::constructNode(Node* constructor, BaseNode* _parent)
{
	if (constructor == nullptr)
	{
		return nullptr;
	}
	Node* cur = new Node(_parent, nullptr, nullptr, constructor->data);
	cur->left = constructNode(static_cast<Node*>(constructor->left), cur);
	cur->right = constructNode(static_cast<Node*>(constructor->right), cur);
	return cur;
}

template <typename T>
typename set<T>::Node* set<T>::findNode(Node* _root, T const& x) const
{
	if (_root == nullptr || x == _root->data)
	{
		return _root;
	}
	if (x < _root -> data)
	{
		return findNode(static_cast<Node*>(_root->left), x);
	}
	return findNode(static_cast<Node*>(_root->right), x);
}

template <typename T>
typename set<T>::Node* set<T>::insertNode(Node* _root, T const& x)
{
	while (_root != nullptr)
	{
		if (x < _root->data)
		{
			if (_root->left != nullptr)
			{
				_root = static_cast<Node*>(_root->left);
			}
			else
			{
				Node* inserted = new Node(nullptr, nullptr, _root, x);
				_root->left = inserted;
				return inserted;
			}
		}

		if (x > _root->data)
		{
			if (_root->right != nullptr)
			{
				_root = static_cast<Node*>(_root->right);
			}
			else
			{
				Node* inserted = new Node(nullptr, nullptr, _root, x);
				_root->right = inserted;
				return inserted;
			}
		}
		if (x == _root->data)
		{
			return nullptr;
		}
	}
	return nullptr;
}

template <typename T>
typename set<T>::Node* set<T>::eraseNode(Node* _root, T const& x)
{
	if (_root == nullptr)
	{
		return _root;
	}
	if (x < _root->data)
	{
		_root->left = eraseNode(static_cast<Node*>(_root->left), x);
		if (_root->left != nullptr)
		{
			_root->left->parent = _root;
		}
	}
	if (x > _root->data)
	{
		_root->right = eraseNode(static_cast<Node*>(_root->right), x);
		if (_root->right != nullptr)
		{
			_root->right->parent = _root;
		}
	}
	if (x == _root->data)
	{
		if (_root->left != nullptr && _root->right != nullptr)
		{
			_root->data = static_cast<Node*>(goLeft(_root->right))->data;
			_root->right = eraseNode(static_cast<Node*>(_root->right), _root->data);
			if (_root->right != nullptr)
			{
				_root->right->parent = _root;
			}
		}
		else if (_root->left != nullptr)
		{
			Node* temp = static_cast<Node*>(_root->left);
			temp->parent = _root->parent;
			if (_root->parent->left == _root)
			{
				_root->parent->left = temp;
			}
			else
			{
				_root->parent->right = temp;
			}
			delete _root;
			_root = nullptr;
		}
		else if (_root->right != nullptr)
		{
			Node* temp = static_cast<Node*>(_root->right);
			temp->parent = _root->parent;
			if (_root->parent->left == _root)
			{
				_root->parent->left = temp;
			}
			else
			{
				_root->parent->right = temp;
			}
			delete _root;
			_root = nullptr;
		}
		else
		{
			if (_root -> parent -> left == _root)
			{
				_root->parent->left = nullptr;
			}
			else
			{
				_root->parent->right = nullptr;
			}
			delete _root;
			_root = nullptr;
		}
	}
	return _root;
}

template <typename T>
typename set<T>::Node* set<T>::lowerBoundNode(Node* _root, T const& x) const
{
	if (_root == nullptr || _root -> data == x)
	{
		return _root;
	}
	if (_root->data < x)
	{
		return lowerBoundNode(static_cast<Node*>(_root->right), x);
	}
	Node* ans = lowerBoundNode(static_cast<Node*>(_root->left), x);
	if (ans != nullptr && ans->data >= x)
	{
		return ans;
	}
	return _root;
}

template <typename T>
typename set<T>::BaseNode* set<T>::goLeft(BaseNode* _root)
{
	if (_root -> left == nullptr)
	{
		return _root;
	}
	return goLeft(_root->left);
}

template <typename T>
typename set<T>::BaseNode* set<T>::goRight(BaseNode* _root)
{
	if (_root->right == nullptr)
	{
		return _root;
	}
	return goRight(_root->right);
}

