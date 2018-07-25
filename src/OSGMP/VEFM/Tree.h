#pragma once

#include "./VEFM_Common.h"

namespace VEFM
{
	template<typename T> class TreeNode;
	template<typename T> class Tree;

	template<typename T>
	class TreeNode
	{
	public:
		inline const string& GetName() const { return m_name; }
		inline TreeNode<T>* GetParent() { return m_pParent; }
		inline TreeNode<T>* GetChild(int index) { if(index > m_children.size() - 1) return nullptr else return m_children; }
		inline vector<TreeNode<T>*>& GetChildren() { return m_children; }

		inline bool IsLeaf() { return m_children.size() == 0; }

		inline vector<T>& GetElements() { return m_elements; }

		TreeNode<T>* FindNode(const string& name)
		{
			if (m_name == name)
			{
				return this;
			}
			else
			{
				for (auto& pC : m_children)
				{
					auto* pNode = pC->FindNode(name);
					if (pNode != nullptr)
					{
						return pNode;
					}
				}
				return nullptr;
			}
		}

		void AddChild(TreeNode<T>* pChildNode)
		{
			auto pParent = pChildNode->GetParent();

			if (pParent != nullptr)
			{
				auto& ci = pParent->m_children.begin();
				while (ci != pParent->m_children.end())
				{
					if ((*ci) == pChildNode)
					{
						ci = pParent->m_children.erase(ci);
					}
					else
					{
						ci++;
					}
				}
			}
			pChildNode->m_pParent = this;
			m_children.push_back(pChildNode);
		}

		void RemoveChild(TreeNode<T>* pChildNode)
		{
			auto& ci = m_children.begin();
			while (ci != m_children.end())
			{
				if ((*ci) == pChildNode)
				{
					ci = m_children.erase(ci);
				}
				else
				{
					ci++;
				}
			}

			pChildNode->m_pParent = nullptr;
		}

	protected:
		Tree<T>* m_pTree = nullptr;
		TreeNode<T>* m_pParent = nullptr;
		vector<TreeNode<T>*> m_children;
		string m_name;
		vector<T> m_elements;

		TreeNode(Tree<T>* pTree, TreeNode<T>* pParent, const string& name)
			: m_pTree(pTree), m_pParent(pParent), m_name(name)
		{
		}

		~TreeNode()
		{
			for (auto& pC : m_children)
			{
				if (pC != nullptr)
				{
					delete pC;
				}
			}
			m_children.clear();
		}

	public:
		friend class Tree<T>;
	};

	template<typename T>
	class Tree
	{
	public:
		Tree()
		{
			m_pRootNode = new TreeNode<T>(this, nullptr, "0");
		}

		~Tree()
		{
			if (m_pRootNode != nullptr)
			{
				delete m_pRootNode;
				m_pRootNode = nullptr;
			}
		}

		inline TreeNode<T>* GetRootNode() { return m_pRootNode; }

		TreeNode<T>* CreateNode(TreeNode<T>* pParent, const string& name = "")
		{
			if (pParent == nullptr)
			{
				return nullptr;
			}

			string nodeName(name);
			if (nodeName.empty())
			{
				char buffer[32];
				memset(buffer, 0, 32);
				itoa(m_nodeCount++, buffer, 10);
				nodeName = buffer;
			}

			auto pNode = new TreeNode<T>(this, pParent, nodeName);
			pParent->m_children.push_back(pNode);

			return pNode;
		}

		TreeNode<T>* FindNode(const string& name)
		{
			return m_pRootNode->FindNode(name);
		}

		void RemoveNode(TreeNode<T>* pNode)
		{
			if (pNode == nullptr)
				return;

			TreeNode<T>* pParent = pNode->m_pParent;
			auto& ci = pParent->m_children.begin();
			while (ci != pParent->m_children.end())
			{
				if (*ci == pNode)
				{
					ci = pParent->m_children.erase(ci);
				}
				else
				{
					ci++;
				}
			}

			delete pNode;
		}

	protected:
		TreeNode<T>* m_pRootNode = nullptr;
		int m_nodeCount = 0;
	};

	//===========================================================================================================//

	template<typename T> class BinaryTreeNode;
	template<typename T> class BinaryTree;

	template<typename T>
	class BinaryTreeNode
	{
	public:
		inline const string& GetName() const { return m_name; }
		inline BinaryTreeNode<T>* GetParent() { return m_pParent; }
		inline BinaryTreeNode<T>* GetLeftChild() { return m_pLeftChild; }
		inline BinaryTreeNode<T>* GetRightChild() { return m_pRightChild; }
		inline vector<T>& GetElements() { return m_elements; }

		inline bool IsLeaf() { return (m_pLeftChild == nullptr) && (m_pRightChild == nullptr); }

		void AddLeftChild(BinaryTreeNode<T>* pLeftChild)
		{
			if (pLeftChild->m_pParent != nullptr)
			{
				if (pLeftChild == pLeftChild->m_pParent->m_pLeftChild)
				{
					pLeftChild->m_pParent->m_pLeftChild = nullptr;
				}
				if (pLeftChild == pLeftChild->m_pParent->m_pRightChild)
				{
					pLeftChild->m_pParent->m_pRightChild = nullptr;
				}
			}
			pLeftChild->m_pParent = this;

			if (m_pLeftChild != nullptr)
			{
				m_pLeftChild->m_pParent = nullptr;
			}
			m_pLeftChild = pLeftChild;
		}

		void AddRightChild(BinaryTreeNode<T>* pRightChild)
		{
			if (pRightChild->m_pParent != nullptr)
			{
				if (pRightChild == pRightChild->m_pParent->m_pLeftChild)
				{
					pRightChild->m_pParent->m_pLeftChild = nullptr;
				}
				if (pRightChild == pRightChild->m_pParent->m_pRightChild)
				{
					pRightChild->m_pParent->m_pRightChild = nullptr;
				}
			}
			pRightChild->m_pParent = this;

			if (m_pRightChild != nullptr)
			{
				m_pRightChild->m_pParent = nullptr;
			}
			m_pRightChild = pRightChild;
		}

		void RemoveLeftChild()
		{
			auto pLeftChild = m_pLeftChild;
			if (m_pLeftChild != nullptr)
			{
				m_pLeftChild->m_pParent = nullptr;
			}
			m_pLeftChild = nullptr;
			delete pLeftChild;
		}

		void RemoveRightChild()
		{
			auto pRightChild = m_pRightChild;
			if (m_pRightChild != nullptr)
			{
				m_pRightChild->m_pParent = nullptr;
			}
			m_pRightChild = nullptr;
			delete pRightChild;
		}

	protected:
		BinaryTree<T>* m_pTree = nullptr;
		BinaryTreeNode<T>* m_pParent = nullptr;
		BinaryTreeNode<T>* m_pLeftChild = nullptr;
		BinaryTreeNode<T>* m_pRightChild = nullptr;

		string m_name;
		vector<T> m_elements;

		BinaryTreeNode(BinaryTree<T>* pTree, BinaryTreeNode<T>* pParent, const string& name)
			: m_pTree(pTree), m_pParent(pParent), m_name(name)
		{
		}

		~BinaryTreeNode()
		{
			if (m_pLeftChild != nullptr)
			{
				delete m_pLeftChild;
				m_pLeftChild = nullptr;
			}

			if (m_pRightChild != nullptr)
			{
				delete m_pRightChild;
				m_pRightChild = nullptr;
			}
		}

		public:
			friend class BinaryTree<T>;
	};

	template<typename T>
	class BinaryTree
	{
	public:
		BinaryTree()
		{
			m_pRootNode = new BinaryTreeNode<T>(this, nullptr, "0");
		}

		~BinaryTree()
		{
			if (m_pRootNode != nullptr)
			{
				delete m_pRootNode;
				m_pRootNode = nullptr;
			}
		}

		inline BinaryTreeNode<T>* GetRootNode() { return m_pRootNode; }

		BinaryTreeNode<T>* CreateNode(BinaryTreeNode<T>* pParent, bool bLeftChild, const string& name = "")
		{
			if (pParent == nullptr)
			{
				return nullptr;
			}

			string nodeName(name);
			if (nodeName.empty())
			{
				char buffer[32];
				memset(buffer, 0, 32);
				itoa(m_nodeCount++, buffer, 10);
				nodeName = buffer;
			}

			auto pNode = new BinaryTreeNode<T>(this, pParent, nodeName);
			if (bLeftChild)
			{
				pParent->AddLeftChild(pNode);
			}
			else
			{
				pParent->AddRightChild(pNode);
			}

			return pNode;
		}

	protected:
		BinaryTreeNode<T>* m_pRootNode = nullptr;
		int m_nodeCount = 0;
	};
}
