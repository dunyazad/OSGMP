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

		vector<T>& GetElements() { return m_elements; }

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

	protected:
		TreeNode<T>* m_pParent = nullptr;
		vector<TreeNode<T>*> m_children;
		string m_name;
		vector<T> m_elements;

		TreeNode(TreeNode<T>* pParent, const string& name)
			: m_pParent(pParent), m_name(name)
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
			m_pRootNode = new TreeNode<T>(nullptr, "0");
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

			auto pNode = new TreeNode<T>(pParent, nodeName);
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

}