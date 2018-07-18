#pragma once

#include "VEFM_Common.h"

#include "Octree.h"

namespace VEFM
{
	template <typename T>
	class Mesh : public VEFMElement
	{
	public:
		Mesh();
		~Mesh();
		void Clear();

		Vertex<T>* GetVertex(const T& position);
		Vertex<T>* GetVertex(float x, float y, float z);
		
		Vertex<T>* GetOrCreateVertex(const T& position);
		Vertex<T>* GetOrCreateVertex(float x, float y, float z);
		
		Edge<T>* GetEdge(Vertex<T>* pV0, Vertex<T>* pV1);
		Edge<T>* GetEdge(const T& p0, const T& p1);
		Edge<T>* GetEdge(float x0, float y0, float z0, float x1, float y1, float z1);
		
		Edge<T>* GetOrCreateEdge(Vertex<T>* pV0, Vertex<T>* pV1);
		Edge<T>* GetOrCreateEdge(const T& p0, const T& p1);
		Edge<T>* GetOrCreateEdge(float x0, float y0, float z0, float x1, float y1, float z1);

		Face<T>* GetFace(Edge<T>* pE0, Edge<T>* pE1, Edge<T>* pE2);
		Face<T>* GetFace(const T& p0, const T& p1, const T& p2);
		Face<T>* GetFace(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2);

		Face<T>* GetOrCreateFace(Edge<T>* pE0, Edge<T>* pE1, Edge<T>* pE2);
		Face<T>* GetOrCreateFace(const T& p0, const T& p1, const T& p2);
		Face<T>* GetOrCreateFace(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2);

		void GetVerticesAndIndices(vector<float>& positions, vector<int>& indices);

		void Refresh();

		void CollapseEdge(Edge<T>* pE);

		void FlipEdge(Edge<T>* pE);
		void FlipEdge(Face<T>* pF0, Face<T>* pF1);
		void FlipEdge(Face<T>* pF0, Face<T>* pF1, Edge<T>* pE);

		Edge<T>* FindFirstMetBorderEdge();
		vector<Edge<T>*> FindBorderEdges();
		vector<Edge<T>*> FindNonManifoldEdges();

		//inline const map<T, Vertex<T>*>& GetVertices() const { return m_vertices; }
		inline void GetVertices(set<Vertex<T>*>& vertices) const { m_pVolumeInfo->GetAllElements(vertices); }
		inline const vector<Edge<T>*>& GetEdges() const { return m_edges; }
		inline const vector<Face<T>*>& GetFaces() const { return m_faces; }

		inline const T& GetMin() const { return m_min; }
		inline const T& GetCenter() const { return m_center; }
		inline const T& GetMax() const { return m_max; }

		inline void SetVolumeMinMax(const T& min, const T& max) { m_min = min; m_max = max; m_center = (min + max) * 0.5f; }
		inline Octree<Vertex<T>, T>* GetVolumeInfo() const { return m_pVolumeInfo; }

		void PrintInformation();

	private:
		//map<T, Vertex<T>*> m_vertices;
		vector<Edge<T>*> m_edges;
		vector<Face<T>*> m_faces;

		T m_min;
		T m_center;
		T m_max;

		Octree<Vertex<T>, T>* m_pVolumeInfo = nullptr;
		int m_volumeInfoMaxLevel = 10;

		virtual void OnQueryDelete();
	};
}

#include "Mesh.hpp"