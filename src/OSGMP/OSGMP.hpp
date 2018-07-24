#pragma once

inline bool IsPointOnTriangle(const Vec3& p, const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
	auto u = v1 - v0;
	auto v = v2 - v0;
	auto n = u ^ v;
	auto w = p - v0;
	auto gamma = ((u ^ w) * n) / (n * n);
	auto beta = ((w ^ v) * n) / (n * n);
	auto alpha = 1 - gamma - beta;
	return ((0 <= alpha) && (alpha <= 1) &&
		(0 <= beta) && (beta <= 1) &&
		(0 <= gamma) && (gamma <= 1));
}

inline bool FloatEqual(float a, float b)
{
	float epsilon;
	/* May as well do the easy check first. */
	if (a == b)
		return true;

	if (a > b) {
		epsilon = a * 0.000001f;
	}
	else {
		epsilon = b * 0.000001f;
	}

	return fabs(a - b) < epsilon;
}

template<typename T> OSGMP<T>::OSGMP(Group* pRootNode)
{
	m_pRootNode = new Group;
	pRootNode->addChild(m_pRootNode);
}

template<typename T> OSGMP<T>::~OSGMP()
{
	for (auto& kvp : m_meshes)
	{
		delete kvp.second;
	}
}

template<typename T>
Mesh<T>* OSGMP<T>::GetMesh(const string& name)
{
	if (m_meshes.count(name) == 0)
	{
		return nullptr;
	}
	else
	{
		return m_meshes[name];
	}
}

template<typename T>
Mesh<T>* OSGMP<T>::GetOrCreateMesh(const string& name)
{
	if (m_meshes.count(name) != 0)
	{
		return m_meshes[name];
	}

	Mesh<T>* pMesh = new Mesh<T>;
	pMesh->Clear();
	m_meshes[name] = pMesh;


	ref_ptr<Geode> pGeode = new Geode;
	ref_ptr<Geometry> pGeometry = new Geometry();
	pGeometry->setName(name);

	auto pSS = pGeometry->getOrCreateStateSet();
	pSS->setRenderingHint(StateSet::TRANSPARENT_BIN);
	ref_ptr<BlendFunc> pBlendFunc = new BlendFunc;
	pBlendFunc->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	pSS->setAttributeAndModes(pBlendFunc);

	pGeode->addDrawable(pGeometry);

	ref_ptr<Group> pGroup = new Group;
	m_models[name] = pGroup;
	ref_ptr<Switch> pSwitch = new Switch;
	pGroup->addChild(pSwitch);
	pSwitch->addChild(pGeode);

	m_pRootNode->addChild(pGroup);

	return pMesh;
}

template<typename T>
bool OSGMP<T>::LoadABDFile(const string& name, const string& fileName)
{
	File file;
	file.Open(fileName, true);

	if (file.isOpen() == false) return false;

	char header[80];
	memset(header, 0, sizeof(char) * 80);
	file.Read(header, sizeof(char) * 80);

	int iVertexNum = -1;
	file.Read((char*)&iVertexNum, 4);

	Vec3* pVertices = new Vec3[iVertexNum];
	file.Read((char*)pVertices, sizeof(Vec3) * iVertexNum);

	int iIndexNum = -1;
	file.Read((char*)&iIndexNum, 4);

	int* pIndices = new int[iIndexNum];
	file.Read((char*)pIndices, sizeof(int) * iIndexNum);

	file.Close();

	Mesh<T>* pMesh = new Mesh<T>;
	pMesh->Clear();

	Vec3 vmin(FLT_MAX, FLT_MAX, FLT_MAX);
	Vec3 vmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (int i = 0; i < iVertexNum; i++)
	{
		MIN_Vec3(vmin, pVertices[i]);
		MAX_Vec3(vmax, pVertices[i]);
	}
	pMesh->SetVolumeMinMax(Vec3toT(vmin), Vec3toT(vmax));

	for (int i = 0; i < iIndexNum / 3; i++)
	{
		auto i0 = pIndices[i * 3 + 0];
		auto i1 = pIndices[i * 3 + 1];
		auto i2 = pIndices[i * 3 + 2];
		auto v0 = pVertices[i0];
		auto v1 = pVertices[i1];
		auto v2 = pVertices[i2];

		pMesh->GetOrCreateFace(v0.x(), v0.y(), v0.z(), v1.x(), v1.y(), v1.z(), v2.x(), v2.y(), v2.z());
	}

	m_meshes[name] = pMesh;

	delete[] pVertices;
	delete[] pIndices;

	ref_ptr<Group> pGroup = new Group;
	m_pRootNode->addChild(pGroup);
	ref_ptr<Switch> pSwitch = new Switch;
	pGroup->addChild(pSwitch);
	ref_ptr<Geode> pGeode = new Geode;
	pSwitch->addChild(pGeode);
	ref_ptr<Geometry> pGeometry = new Geometry();
	pGeode->addDrawable(pGeometry);
	pGeometry->setName(name);

	m_models[name] = pGroup;

	UpdateModel(name);

	return true;
}

template<typename T>
bool OSGMP<T>::SaveABDFile(const string& name, const string& filename)
{
	auto pMesh = GetMesh(name);
	if (pMesh == nullptr) return false;

	File file;
	file.Create(filename, true);
	char header[80] = "Diorco Autolign Binary Data Format V1.0";
	file.Write((char*)header, 80);

	vector<float> vertices;
	vector<int> indices;

	pMesh->GetVerticesAndIndices(vertices, indices);

	int iVertexNum = vertices.size() / 3;
	file.Write((char*)(&iVertexNum), 4);
	file.Write((char*)(&vertices[0]), iVertexNum * 3 * 4);

	int iIndicesNum = indices.size();
	file.Write((char*)(&iIndicesNum), 4);
	file.Write((char*)(&indices[0]), iIndicesNum * 4);

	file.Close();

	return true;
}

template<typename T>
bool OSGMP<T>::LoadABDEFile(const string& name, const string& fileName, int* iReservedDataSize, void** pReservedData)
{
	File file;
	file.Open(fileName, true);

	if (file.isOpen() == false) return false;

	char header[80];
	memset(header, 0, sizeof(char) * 80);
	file.Read(header, sizeof(char) * 80);

	int iVertexNum = -1;
	file.Read((char*)&iVertexNum, 4);

	Vec3* pVertices = new Vec3[iVertexNum];
	file.Read((char*)pVertices, sizeof(Vec3) * iVertexNum);

	int iIndexNum = -1;
	file.Read((char*)&iIndexNum, 4);

	int* pIndices = new int[iIndexNum];
	file.Read((char*)pIndices, sizeof(int) * iIndexNum);

	if (iReservedDataSize != nullptr)
	{
		file.Read((char*)iReservedDataSize, 4);
		if (iReservedDataSize != 0)
		{
			*pReservedData = new char[*iReservedDataSize];
			file.Read((char*)*pReservedData, *iReservedDataSize);
		}
	}

	file.Close();

	Mesh<T>* pMesh = new Mesh<T>;
	pMesh->Clear();

	Vec3 vmin(FLT_MAX, FLT_MAX, FLT_MAX);
	Vec3 vmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (int i = 0; i < iVertexNum; i++)
	{
		MIN_Vec3(vmin, pVertices[i]);
		MAX_Vec3(vmax, pVertices[i]);
	}
	pMesh->SetVolumeMinMax(Vec3toT(vmin), Vec3toT(vmax));

	for (int i = 0; i < iIndexNum / 3; i++)
	{
		auto i0 = pIndices[i * 3 + 0];
		auto i1 = pIndices[i * 3 + 1];
		auto i2 = pIndices[i * 3 + 2];
		auto v0 = pVertices[i0];
		auto v1 = pVertices[i1];
		auto v2 = pVertices[i2];

		pMesh->GetOrCreateFace(v0.x(), v0.y(), v0.z(), v1.x(), v1.y(), v1.z(), v2.x(), v2.y(), v2.z());
	}

	m_meshes[name] = pMesh;

	delete[] pVertices;
	delete[] pIndices;

	ref_ptr<Group> pGroup = new Group;
	m_pRootNode->addChild(pGroup);
	ref_ptr<Switch> pSwitch = new Switch;
	pGroup->addChild(pSwitch);
	ref_ptr<Geode> pGeode = new Geode;
	pSwitch->addChild(pGeode);
	ref_ptr<Geometry> pGeometry = new Geometry();
	pGeode->addDrawable(pGeometry);
	pGeometry->setName(name);

	m_models[name] = pGroup;

	UpdateModel(name);

	return true;
}

template<typename T>
bool OSGMP<T>::SaveABDEFile(const string& name, const string& filename, int iReservedDataSize, void* pReservedData)
{
	auto pMesh = GetMesh(name);
	if (pMesh == nullptr) return false;

	File file;
	file.Create(filename, true);
	char header[80] = "Diorco Autolign Binary Data Format V1.0";
	file.Write((char*)header, 80);

	vector<float> vertices;
	vector<int> indices;

	pMesh->GetVerticesAndIndices(vertices, indices);

	int iVertexNum = vertices.size() / 3;
	file.Write((char*)(&iVertexNum), 4);
	file.Write((char*)(&vertices[0]), iVertexNum * 3 * 4);

	int iIndicesNum = indices.size();
	file.Write((char*)(&iIndicesNum), 4);
	file.Write((char*)(&indices[0]), iIndicesNum * 4);

	if (iReservedDataSize != 0 && pReservedData != nullptr)
	{
		file.Write((char*)(&iReservedDataSize), 4);
		file.Write((char*)(pReservedData), iReservedDataSize);
	}

	file.Close();

	return true;
}

template<typename T>
bool OSGMP<T>::LoadSTLFile(const string& name, const string& filename)
{
	if (m_meshes.count(name) != 0)
		return false;

	File file;
	file.Open(filename, false);
	if (!file.isOpen()) return false;

	vector<Vec3> vertices;
	vector<Vec3> normals;
	vector<int> indices;

	{
		map<Vec3, int> vertexMap;

		string word;
		file.GetWord(word);
		if (word == "solid")
		{
			while (file.GetWord(word))
			{
				if (word == "facet")
				{
					file.GetWord(word);
					Vec3 normal;
					if (word == "normal")
					{
						file.GetWord(word);
						normal.x() = (float)atof(word.c_str());
						file.GetWord(word);
						normal.y() = (float)atof(word.c_str());
						file.GetWord(word);
						normal.z() = (float)atof(word.c_str());
					}
					file.GetWord(word);
					if (word == "outer")
					{
						file.GetWord(word);
						if (word == "loop")
						{
							file.GetWord(word);
							if (word == "vertex")
							{
								Vec3 v1;
								file.GetWord(word);
								v1.x() = (float)atof(word.c_str());
								file.GetWord(word);
								v1.y() = (float)atof(word.c_str());
								file.GetWord(word);
								v1.z() = (float)atof(word.c_str());

								if (vertexMap.count(v1) == 0)
								{
									vertices.push_back(v1);
									normals.push_back(normal);
									vertexMap[v1] = vertices.size() - 1;
									indices.push_back(vertices.size() - 1);
								}
								else
								{
									auto index = vertexMap[v1];
									indices.push_back(index);
								}
							}
							file.GetWord(word);
							if (word == "vertex")
							{
								Vec3 v2;
								file.GetWord(word);
								v2.x() = (float)atof(word.c_str());
								file.GetWord(word);
								v2.y() = (float)atof(word.c_str());
								file.GetWord(word);
								v2.z() = (float)atof(word.c_str());

								if (vertexMap.count(v2) == 0)
								{
									vertices.push_back(v2);
									normals.push_back(normal);
									vertexMap[v2] = vertices.size() - 1;
									indices.push_back(vertices.size() - 1);
								}
								else
								{
									auto index = vertexMap[v2];
									indices.push_back(index);
								}
							}
							file.GetWord(word);
							if (word == "vertex")
							{
								Vec3 Vec3;
								file.GetWord(word);
								Vec3.x() = (float)atof(word.c_str());
								file.GetWord(word);
								Vec3.y() = (float)atof(word.c_str());
								file.GetWord(word);
								Vec3.z() = (float)atof(word.c_str());

								if (vertexMap.count(Vec3) == 0)
								{
									vertices.push_back(Vec3);
									normals.push_back(normal);
									vertexMap[Vec3] = vertices.size() - 1;
									indices.push_back(vertices.size() - 1);
								}
								else
								{
									auto index = vertexMap[Vec3];
									indices.push_back(index);
								}
							}
						}
						file.GetWord(word); // endloop
						file.GetWord(word); // endfacet
					}
				}
			}
		}
		else
		{
			file.Close();
			file.Open(filename, true);

			char header[80];
			file.Read(header, 80);
			char buffer[4];
			unsigned int numberOfTriangles;
			file.Read(buffer, sizeof(int));

			numberOfTriangles = *((unsigned int*)&buffer);

			for (unsigned int i = 0; i < numberOfTriangles; i++)
			{
				float nx, ny, nz;
				float v1x, v1y, v1z;
				float v2x, v2y, v2z;
				float Vec3x, Vec3y, Vec3z;
				short attribute;

				file.Read((char*)&nx, sizeof(float));
				file.Read((char*)&ny, sizeof(float));
				file.Read((char*)&nz, sizeof(float));

				file.Read((char*)&v1x, sizeof(float));
				file.Read((char*)&v1y, sizeof(float));
				file.Read((char*)&v1z, sizeof(float));

				file.Read((char*)&v2x, sizeof(float));
				file.Read((char*)&v2y, sizeof(float));
				file.Read((char*)&v2z, sizeof(float));

				file.Read((char*)&Vec3x, sizeof(float));
				file.Read((char*)&Vec3y, sizeof(float));
				file.Read((char*)&Vec3z, sizeof(float));

				file.Read((char*)&attribute, sizeof(short));

				Vec3 v1(v1x, v1y, v1z);
				Vec3 v2(v2x, v2y, v2z);
				Vec3 T(Vec3x, Vec3y, Vec3z);
				Vec3 n(nx, ny, nz);

				if (vertexMap.count(v1) == 0)
				{
					vertices.push_back(v1);
					normals.push_back(n);
					vertexMap[v1] = vertices.size() - 1;
					indices.push_back(vertices.size() - 1);
				}
				else
				{
					auto index = vertexMap[v1];
					indices.push_back(index);
				}

				if (vertexMap.count(v2) == 0)
				{
					vertices.push_back(v2);
					normals.push_back(n);
					vertexMap[v2] = vertices.size() - 1;
					indices.push_back(vertices.size() - 1);
				}
				else
				{
					auto index = vertexMap[v2];
					indices.push_back(index);
				}

				if (vertexMap.count(T) == 0)
				{
					vertices.push_back(T);
					normals.push_back(n);
					vertexMap[T] = vertices.size() - 1;
					indices.push_back(vertices.size() - 1);
				}
				else
				{
					auto index = vertexMap[T];
					indices.push_back(index);
				}
			}
		}
	}

	Mesh* pMesh = new Mesh;
	pMesh->Clear();

	Vec3 vmin;
	Vec3 vmax;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		MIN_Vec3(vmin, vertices[i]);
		MAX_Vec3(vmax, vertices[i]);
	}
	pMesh->SetVolumeMinMax(Vec3toT(vmin), Vec3toT(vmax));

	for (unsigned int i = 0; i < indices.size() / 3; i++)
	{
		auto i0 = indices[i * 3 + 0];
		auto i1 = indices[i * 3 + 1];
		auto i2 = indices[i * 3 + 2];
		auto& v0 = vertices[i0];
		auto& v1 = vertices[i1];
		auto& v2 = vertices[i2];

		pMesh->GetOrCreateFace(v0.x(), v0.y(), v0.z(), v1.x(), v1.y(), v1.z(), v2.x(), v2.y(), v2.z());
	}

	m_meshes[name] = pMesh;

	ref_ptr<Group> pGroup = new Group;
	m_pRootNode->addChild(pGroup);
	ref_ptr<Switch> pSwitch = new Switch;
	pGroup->addChild(pSwitch);
	ref_ptr<Geode> pGeode = new Geode;
	pSwitch->addChild(pGeode);
	ref_ptr<Geometry> pGeometry = new Geometry();
	pGeode->addDrawable(pGeometry);
	pGeometry->setName(name);

	m_models[name] = pGroup;

	UpdateModel(name);

	return true;
}

template<typename T>
bool OSGMP<T>::SaveSTLFile(const string& name, const string& filename, bool bianry)
{
	auto pMesh = GetMesh(name);
	if (pMesh == nullptr)
		return false;

	vector<float> vertices;
	vector<int> indices;
	pMesh->GetVerticesAndIndices(vertices, indices);

	//int index = 0;
	//for (auto& pF : pMesh->GetFaces())
	//{
	//	auto& p0 = pF->V0()->GetPosition();
	//	auto& p1 = pF->V1()->GetPosition();
	//	auto& p2 = pF->V2()->GetPosition();

	//	vertices.push_back(p0.x);
	//	vertices.push_back(p0.y);
	//	vertices.push_back(p0.z);
	//	indices.push_back(index++);

	//	vertices.push_back(p1.x);
	//	vertices.push_back(p1.y);
	//	vertices.push_back(p1.z);
	//	indices.push_back(index++);

	//	vertices.push_back(p2.x);
	//	vertices.push_back(p2.y);
	//	vertices.push_back(p2.z);
	//	indices.push_back(index++);
	//}

	bool isBinary = bianry;

	File file;
	if (isBinary)
	{
		file.Create(filename, true);

		unsigned int numberOfTriangles = indices.size() / 3;
		char header[80] = "Header";
		file.Write((char*)header, 80);
		file.Write((char*)&numberOfTriangles, 4);

		for (unsigned int i = 0; i < indices.size() / 3; i++)
		{
			auto i0 = indices[i * 3 + 0];
			auto i1 = indices[i * 3 + 1];
			auto i2 = indices[i * 3 + 2];

			float zero = 0.0f;
			file.Write((char*)&zero, 4);
			file.Write((char*)&zero, 4);
			file.Write((char*)&zero, 4);

			file.Write((char*)&(vertices[i0 * 3 + 0]), 4);
			file.Write((char*)&(vertices[i0 * 3 + 1]), 4);
			file.Write((char*)&(vertices[i0 * 3 + 2]), 4);

			file.Write((char*)&(vertices[i1 * 3 + 0]), 4);
			file.Write((char*)&(vertices[i1 * 3 + 1]), 4);
			file.Write((char*)&(vertices[i1 * 3 + 2]), 4);

			file.Write((char*)&(vertices[i2 * 3 + 0]), 4);
			file.Write((char*)&(vertices[i2 * 3 + 1]), 4);
			file.Write((char*)&(vertices[i2 * 3 + 2]), 4);

			short dummy = 0;
			file.Write((char*)&(dummy), 2);
		}

		file.Close();
	}
	else
	{
		file.Create(filename, false);
		file << "solid " << filename << endl;

		for (unsigned int i = 0; i < indices.size() / 3; i++)
		{
			auto i0 = indices[i * 3 + 0];
			auto i1 = indices[i * 3 + 1];
			auto i2 = indices[i * 3 + 2];

			float zero = 0.0f;
			file << "facet normal " << zero << " " << zero << " " << zero << endl;
			file << "outer loop" << endl;

			file << "vertex " << vertices[i0 * 3 + 0] << " " << vertices[i0 * 3 + 1] << " " << vertices[i0 * 3 + 2] << endl;
			file << "vertex " << vertices[i1 * 3 + 0] << " " << vertices[i1 * 3 + 1] << " " << vertices[i1 * 3 + 2] << endl;
			file << "vertex " << vertices[i2 * 3 + 0] << " " << vertices[i2 * 3 + 1] << " " << vertices[i2 * 3 + 2] << endl;

			file << "endloop" << endl;
			file << "endfacet" << endl;
		}

		file << "endsolid " << filename << endl;

		file.Close();
	}

	return true;
}

template<typename T>
bool OSGMP<T>::LoadOFFFile(const string& name, const string& fileName)
{
	File file;
	file.Open(fileName, true);

	if (file.isOpen() == false) return false;

	string filetype;
	file.GetWord(filetype);
	if (filetype != "OFF") return false;

	int vertexCount = 0;
	int faceCount = 0;
	int edgeCount = 0;

	file >> vertexCount;
	file >> faceCount;
	file >> edgeCount;

	Vec3* pVertices = new Vec3[vertexCount];
	int* pIndices = new int[faceCount * 3];

	for (int i = 0; i < vertexCount; i++)
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		file >> x;
		file >> y;
		file >> z;

		pVertices[i].x() = x;
		pVertices[i].y() = y;
		pVertices[i].z() = z;
	}

	for (int i = 0; i < faceCount; i++)
	{
		int vertexCountOfFace = 0;
		int i1 = 0;
		int i2 = 0;
		int i3 = 0;

		file >> vertexCountOfFace;
		file >> i1;
		file >> i2;
		file >> i3;

		pIndices[i * 3 + 0] = i1;
		pIndices[i * 3 + 1] = i2;
		pIndices[i * 3 + 2] = i3;
	}

	file.Close();

	Mesh* pMesh = new Mesh;
	pMesh->Clear();

	Vec3 vmin;
	Vec3 vmax;
	for (int i = 0; i < vertexCount; i++)
	{
		MIN_Vec3(vmin, pVertices[i]);
		MAX_Vec3(vmax, pVertices[i]);
	}
	pMesh->SetVolumeMinMax(Vec3toT(vmin), Vec3toT(vmax));

	for (int i = 0; i < faceCount; i++)
	{
		auto i0 = pIndices[i * 3 + 0];
		auto i1 = pIndices[i * 3 + 1];
		auto i2 = pIndices[i * 3 + 2];
		auto v0 = pVertices[i0];
		auto v1 = pVertices[i1];
		auto v2 = pVertices[i2];

		pMesh->GetOrCreateFace(v0.x(), v0.y(), v0.z(), v1.x(), v1.y(), v1.z(), v2.x(), v2.y(), v2.z());
	}

	m_meshes[name] = pMesh;

	delete[] pVertices;
	delete[] pIndices;

	ref_ptr<Group> pGroup = new Group;
	m_pRootNode->addChild(pGroup);
	ref_ptr<Switch> pSwitch = new Switch;
	pGroup->addChild(pSwitch);
	ref_ptr<Geode> pGeode = new Geode;
	pSwitch->addChild(pGeode);
	ref_ptr<Geometry> pGeometry = new Geometry();
	pGeode->addDrawable(pGeometry);
	pGeometry->setName(name);

	m_models[name] = pGroup;

	UpdateModel(name);

	return true;
}

template<typename T>
bool OSGMP<T>::SaveOFFFile(const string& name, const string& filename)
{
	auto pMesh = GetMesh(name);
	if (pMesh == nullptr) return false;

	vector<float> vertices;
	vector<int> indices;

	pMesh->GetVerticesAndIndices(vertices, indices);

	File file;
	file.Create(filename, true);

	file << "OFF\n";
	file << vertices.size() / 3 << ' ' << indices.size() / 3 << " 0\n";
	for (unsigned int i = 0; i < vertices.size() / 3; i++)
	{
		file << vertices[i * 3 + 0];
		file << " ";
		file << vertices[i * 3 + 1];
		file << " ";
		file << vertices[i * 3 + 2];
		file << "\n";
	}

	for (unsigned int i = 0; i < indices.size() / 3; i++)
	{
		file << "3 ";
		file << indices[i * 3 + 0];
		file << " ";
		file << indices[i * 3 + 1];
		file << " ";
		file << indices[i * 3 + 2];
		file << "\n";
	}

	file.Close();

	return true;
}

template<typename T>
bool OSGMP<T>::GetOFFString(const string& name, stringstream& ss)
{
	auto pMesh = GetMesh(name);
	if (pMesh == nullptr)
		return false;

	vector<float> vertices;
	vector<int> indices;

	pMesh->GetVerticesAndIndices(vertices, indices);

	ss << "OFF\n";
	ss << vertices.size() / 3 << ' ' << indices.size() / 3 << " 0\n";
	for (unsigned int i = 0; i < vertices.size() / 3; i++)
	{
		ss << vertices[i * 3 + 0];
		ss << " ";
		ss << vertices[i * 3 + 1];
		ss << " ";
		ss << vertices[i * 3 + 2];
		ss << "\n";
	}

	for (unsigned int i = 0; i < indices.size() / 3; i++)
	{
		ss << "3 ";
		ss << indices[i * 3 + 0];
		ss << " ";
		ss << indices[i * 3 + 1];
		ss << " ";
		ss << indices[i * 3 + 2];
		ss << "\n";
	}

	return true;
}

template<typename T>
void OSGMP<T>::UpdateModel()
{
	for (auto& kvp : m_models)
	{
		UpdateModel(kvp.first);
	}
}

template<typename T>
void OSGMP<T>::UpdateModel(const string& name)
{
	auto pMesh = m_meshes[name];

	pMesh->Refresh();

	auto pGroup = m_models[name];
	auto pSwitch = pGroup->getChild(0)->asSwitch();
	auto pGeode = pSwitch->getChild(0)->asGeode();
	auto pGeometry = pGeode->getDrawable(0)->asGeometry();
	pGeometry->setName(name);

	map<Vec3, int> vertexMap;
	ref_ptr<Vec3Array> pVertexArray = (Vec3Array*)pGeometry->getVertexArray();
	if (pVertexArray == nullptr) pVertexArray = new Vec3Array;
	else pVertexArray->clear();
	for (unsigned int i = 0; i < pGeometry->getNumPrimitiveSets(); i++)
	{
		pGeometry->removePrimitiveSet(i);
	}

	ref_ptr<DrawElementsUInt> pIndices = new DrawElementsUInt(GL_TRIANGLES);

	ref_ptr<Vec4Array> pColorArray = new Vec4Array;
	pColorArray->push_back(Vec4(1, 1, 1, 1));

	vector<float> vertices;
	vector<int> indices;
	pMesh->GetVerticesAndIndices(vertices, indices);

	for (unsigned int i = 0; i < vertices.size() / 3; i++)
	{
		pVertexArray->push_back(Vec3(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2]));
	}

	for (unsigned int i = 0; i < indices.size(); i++)
	{
		pIndices->push_back(indices[i]);
	}

	pGeometry->setVertexArray(pVertexArray);
	pGeometry->addPrimitiveSet(pIndices);
	pGeometry->setColorArray(pColorArray);

	auto pSS = pGeometry->getOrCreateStateSet();
	pSS->setRenderingHint(StateSet::TRANSPARENT_BIN);
	ref_ptr<BlendFunc> pBlendFunc = new BlendFunc;
	pBlendFunc->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	pSS->setAttributeAndModes(pBlendFunc);

	osgUtil::SmoothingVisitor::smooth(*pGeometry);
}

template<typename T>
void OSGMP<T>::TogglePolygonMode()
{
	for (auto& kvp : m_models)
	{
		TogglePolygonMode(kvp.first);
	}
}

template<typename T>
void OSGMP<T>::TogglePolygonMode(const string& name)
{
	auto& pGroup = m_models[name];
	auto pSwitch = pGroup->getChild(0)->asSwitch();
	auto pGeode = pSwitch->getChild(0)->asGeode();
	auto pGeometry = pGeode->getDrawable(0)->asGeometry();
	osg::StateSet* pSS = pGeometry->getOrCreateStateSet();

	PolygonMode* pMode = (PolygonMode*)(pSS->getAttribute(StateAttribute::POLYGONMODE));
	if (pMode == nullptr)
	{
		ref_ptr<PolygonMode> pNewMode = new PolygonMode;
		pNewMode->setMode(PolygonMode::FRONT_AND_BACK, PolygonMode::Mode::FILL);
		pSS->setAttributeAndModes(pNewMode, StateAttribute::ON);
		pGeometry->setStateSet(pSS);
		return;
	}
	pMode = (PolygonMode*)(pSS->getAttribute(StateAttribute::POLYGONMODE));
	auto mode = pMode->getMode(PolygonMode::FRONT_AND_BACK);

	if (mode == PolygonMode::Mode::POINT)
	{
		mode = PolygonMode::Mode::LINE;
	}
	else if (mode == PolygonMode::Mode::LINE)
	{
		mode = PolygonMode::Mode::FILL;
	}
	else if (mode == PolygonMode::Mode::FILL)
	{
		mode = PolygonMode::Mode::POINT;
	}
	pMode->setMode(PolygonMode::FRONT_AND_BACK, mode);
	pSS->setAttributeAndModes(pMode, StateAttribute::ON);
	pGeometry->setStateSet(pSS);
}

template<typename T>
void OSGMP<T>::ToggleVisible()
{
	for (auto& kvp : m_models)
	{
		ToggleVisible(kvp.first);
	}
}

template<typename T>
void OSGMP<T>::ToggleVisible(const string& name)
{
	auto& pGroup = m_models[name];
	auto pSwitch = pGroup->getChild(0)->asSwitch();
	pSwitch->setValue(0, !pSwitch->getValue(0));
}

template<typename T>
bool OSGMP<T>::CheckIntersection(const string& nameA, const string& nameB, map<tuple<Mesh<T>*, Face<T>*>, set<T>>& result)
{
	auto pMeshA = GetMesh(nameA);
	auto pMeshB = GetMesh(nameB);

	if (pMeshA == nullptr || pMeshB == nullptr)
	{
		return false;
	}

	VoxelSpace<Face<T>, T> vsA(1);
	VoxelSpace<Face<T>, T> vsB(1);

	auto facesA = pMeshA->GetFaces();
	auto facesB = pMeshB->GetFaces();

	for (auto& pF : facesA)
	{
		vsA.InsertTriangle(pF->V0()->P(), pF->V1()->P(), pF->V2()->P(), pF);
	}

	for (auto& pF : facesB)
	{
		vsB.InsertTriangle(pF->V0()->P(), pF->V1()->P(), pF->V2()->P(), pF);
	}

	//auto& voxelsA = vsB.GetVoxels();
	auto& voxelsB = vsB.GetVoxels();

	set<tuple<int, int, int>> intersectionIndices;
	for (auto& kvp : voxelsB)
	{
		if (vsA.GetVoxel(kvp.first) != nullptr)
		{
			intersectionIndices.insert(kvp.first);
		}
	}

	for (auto& index : intersectionIndices)
	{
		//auto delta = vp->GetMax() - vp->GetMin();
		//m_pVD->AddBoxByT(vp->GetCenter(), delta.x, delta.y, delta.z, V4_RED, true);
		auto pVoxelA = vsA.GetVoxel(index);
		auto pVoxelB = vsB.GetVoxel(index);

		auto& facesA = pVoxelA->GetElements();
		auto& facesB = pVoxelB->GetElements();

		for (auto& pFA : facesA)
		{
			auto pVA0 = pFA->V0();
			auto pVA1 = pFA->V1();
			auto pVA2 = pFA->V2();
			auto& a0 = pVA0->P();
			auto& a1 = pVA1->P();
			auto& a2 = pVA2->P();
			float aa0[3] {a0.x(), a0.y(), a0.z()};
			float aa1[3] {a1.x(), a1.y(), a1.z()};
			float aa2[3] {a2.x(), a2.y(), a2.z()};

			for (auto& pFB : facesB)
			{
				auto pVB0 = pFB->V0();
				auto pVB1 = pFB->V1();
				auto pVB2 = pFB->V2();
				auto& b0 = pVB0->P();
				auto& b1 = pVB1->P();
				auto& b2 = pVB2->P();
				float ab0[3] {b0.x(), b0.y(), b0.z()};
				float ab1[3] {b1.x(), b1.y(), b1.z()};
				float ab2[3] {b2.x(), b2.y(), b2.z()};

				int coplanar = 0;
				T A(-FLT_MAX, -FLT_MAX, -FLT_MAX);
				T B(-FLT_MAX, -FLT_MAX, -FLT_MAX);
				//if (1 == tri_tri_intersect_with_isectline((float*)a0.elements, (float*)a1.elements, (float*)a2.elements, (float*)b0.elements, (float*)b1.elements, (float*)b2.elements, &coplanar, A.elements, B.elements))
				if (1 == tri_tri_intersect_with_isectline(aa0, aa1, aa2, ab0, ab1, ab2, &coplanar, &(A.x()), &(B.x())))
				{
					tuple<Mesh<T>*, Face<T>*> keyA(pMeshA, pFA);
					tuple<Mesh<T>*, Face<T>*> keyB(pMeshB, pFB);
					if (result.count(keyA) == 0)
					{
						result[keyA] = set<T>();
					}
					if (result.count(keyB) == 0)
					{
						result[keyB] = set<T>();
					}
					result[keyA].insert(A);
					result[keyA].insert(B);

					result[keyB].insert(A);
					result[keyB].insert(B);
				}
			}
		}
	}

	if (result.size() > 0) return true;
	else return false;
}

template<typename T>
bool OSGMP<T>::CheckIntersections(const string& baseName, const vector<string>& names, map<tuple<Mesh<T>*, Face<T>*>, set<pair<T, T>>>& result)
{
	auto pMeshBase = GetMesh(baseName);
	if (pMeshBase == nullptr)
	{
		return false;
	}

	vector<Mesh<T>*> meshes;
	for (auto& name : names)
	{
		auto pMesh = GetMesh(name);
		if (pMesh == nullptr)
		{
			return false;
		}
		else
		{
			meshes.push_back(pMesh);
		}
	}

	VoxelSpace<Face<T>, T> vsBase(1);
	auto& facesBase = pMeshBase->GetFaces();

	for (auto& pF : facesBase)
	{
		vsBase.InsertTriangle(pF->V0()->P(), pF->V1()->P(), pF->V2()->P(), pF);
	}

	for (auto& pMesh : meshes)
	{
		auto& faces = pMesh->GetFaces();

		VoxelSpace<Face<T>, T> vs(1);
		for (auto& pF : faces)
		{
			vs.InsertTriangle(pF->V0()->P(), pF->V1()->P(), pF->V2()->P(), pF);
		}

		auto& voxels = vs.GetVoxels();

		set<tuple<int, int, int>> intersectionIndices;
		for (auto& kvp : voxels)
		{
			if (vsBase.GetVoxel(kvp.first) != nullptr)
			{
				intersectionIndices.insert(kvp.first);
			}
		}

		for (auto& index : intersectionIndices)
		{
			//auto delta = vp->GetMax() - vp->GetMin();
			//m_pVD->AddBoxByT(vp->GetCenter(), delta.x, delta.y, delta.z, V4_RED, true);
			auto pVoxelBase = vsBase.GetVoxel(index);
			auto pVoxel = vs.GetVoxel(index);

			auto& facesBase = pVoxelBase->GetElements();
			auto& faces = pVoxel->GetElements();

			for (auto& pFBase : facesBase)
			{
				auto pVBase0 = pFBase->V0();
				auto pVBase1 = pFBase->V1();
				auto pVBase2 = pFBase->V2();
				auto& abase0 = pVBase0->P();
				auto& abase1 = pVBase1->P();
				auto& abase2 = pVBase2->P();
				float aabase0[3] {abase0.x(), abase0.y(), abase0.z()};
				float aabase1[3] {abase1.x(), abase1.y(), abase1.z()};
				float aabase2[3] {abase2.x(), abase2.y(), abase2.z()};

				for (auto& pF : faces)
				{
					auto pV0 = pF->V0();
					auto pV1 = pF->V1();
					auto pV2 = pF->V2();
					auto& p0 = pV0->P();
					auto& p1 = pV1->P();
					auto& p2 = pV2->P();
					float a0[3] {p0.x(), p0.y(), p0.z()};
					float a1[3] {p1.x(), p1.y(), p1.z()};
					float a2[3] {p2.x(), p2.y(), p2.z()};

					int coplanar = 0;
					T A(-FLT_MAX, -FLT_MAX, -FLT_MAX);
					T B(-FLT_MAX, -FLT_MAX, -FLT_MAX);
					//if (1 == tri_tri_intersect_with_isectline((float*)a0.elements, (float*)a1.elements, (float*)a2.elements, (float*)b0.elements, (float*)b1.elements, (float*)b2.elements, &coplanar, A.elements, B.elements))
					if (1 == tri_tri_intersect_with_isectline(aabase0, aabase1, aabase2, a0, a1, a2, &coplanar, &(A.x()), &(B.x())))
					{
						tuple<Mesh<T>*, Face<T>*> keyBase(pMeshBase, pFBase);
						tuple<Mesh<T>*, Face<T>*> key(pMesh, pF);
						if (result.count(keyBase) == 0)
						{
							result[keyBase] = set<pair<T, T>>();
						}
						if (result.count(key) == 0)
						{
							result[key] = set<pair<T, T>>();
						}
						result[keyBase].insert(make_pair(A, B));
						result[key].insert(make_pair(A, B));
					}
				}
			}
		}
	}

	if (result.size() > 0) return true;
	else return false;
}

template<typename T>
void OSGMP<T>::SplitFaces(const map<tuple<Mesh<T>*, Face<T>*>, set<T>>& input)
{
	for (auto& kvp : input)
	{
		auto pMesh = get<0>(kvp.first);
		auto pF = get<1>(kvp.first);
		if (pF->IsDeleteQueried())
			continue;

		auto fv0 = pF->V0()->P();
		auto fv1 = pF->V1()->P();
		auto fv2 = pF->V2()->P();

		pF->QueryDelete();

		vector<Vec3> points(kvp.second.begin(), kvp.second.end());
		
		//vector<pair<T, T>> input;
		//for (auto& vv : kvp.second)
		//{
		//	input.emplace_back(vv);
		//}

		//vector<vector<T>> result;
		//SortPairs<T>(input, result);

		//vector<pair<T, T>> edges;
		//for (auto& va : result)
		//{
		//	for (unsigned int i = 1; i < va.size(); i++)
		//	{
		//		edges.push_back(make_pair(va[i - 1], va[i]));
		//	}
		//}

		vector<tuple<Vec3, Vec3, Vec3>> triangulated;
//////////////////////////////////////////////////////////////////////////////////////////////////////////		BowyerWatson(fv0, fv1, fv2, points, triangulated);
		Triangulate(fv0, fv1, fv2, points, triangulated);
		//SplitTriangleByPoints(TTT(fv0, fv1, fv2), edges, triangulated);

		for (auto& vvv : triangulated)
		{
			////if (vvv.v0 != vvv.v1 && vvv.v0 != vvv.v2 && vvv.v1 != vvv.v2)
			//{
			auto& v0 = get<0>(vvv);
			auto& v1 = get<1>(vvv);
			auto& v2 = get<2>(vvv);

			if (v0 != v1 && v0 != v2 && v1 != v2)
			{
				auto d01 = v1 - v0;
				auto d02 = v2 - v0;
				if (fabsf(d01 * d02) > 0.00001f)
				{
					pMesh->GetOrCreateFace(v0, v1, v2);
				}
			}
			//}
			//else
			//{
			//	printf("WTF!!!!!\n");
			//}
		}

		//for (auto& vv : kvp.second)
		//{
		//	auto pE = pMesh->GetEdge(vv.first, vv.second);
		//	if (pE)
		//	{
		//		pE->SetFlag("IntersectionBorder", true);
		//	}
		//}
	}
}

//template<typename T>
//void OSGMP<T>::Triangulate(const T& fv0, const T& fv1, const T& fv2, const vector<T>& points, vector<tuple<T, T, T>>& result)
//{
//	auto d01 = fv1 - fv0;
//	auto d02 = fv2 - fv0;
//	auto normal = d01 ^ d02;
//	normal.normalize();
//
//	auto center = (fv0 + fv1 + fv2) / 3;
//
//	Matrix m;
//	m.makeRotate(T(0, 0, 1), normal);
//	m.setTrans(center);
//
//	auto im = Matrix::inverse(m);
//
//	auto pfv0 = fv0 * im;
//	auto pfv1 = fv1 * im;
//	auto pfv2 = fv2 * im;
//
//	vector<T> projected;
//	for (auto& p : points)
//	{
//		projected.push_back(p * im);
//	}
//
//	Mesh<T>* pMesh = new Mesh<T>();
//	pMesh->GetOrCreateFace(pfv0, pfv1, pfv2);
//
//
//	vector<tuple<T, T, T>> trianglePoints;
//	for (auto& pF : pMesh->GetFaces())
//	{
//		trianglePoints.push_back(make_tuple(pF->V0()->P(), pF->V1()->P(), pF->V2()->P()));
//	}
//
//	int i = 0;
//	for (auto& p : projected)
//	{
//		pMesh->InsertVertex(p);
//	}
//
//	pMesh->Refresh();
//
//	for (auto& pF : pMesh->GetFaces())
//	{
//		if (pF->IsDeleteQueried() == false)
//		{
//			result.push_back(make_tuple(pF->V0()->P() * m, pF->V1()->P() * m, pF->V2()->P() * m));
//		}
//	}
//
//	delete pMesh;
//}

template<typename T>
void OSGMP<T>::Triangulate(const T& fv0, const T& fv1, const T& fv2, const vector<T>& points, vector<tuple<T, T, T>>& result)
{
	auto d01 = fv1 - fv0;
	auto d02 = fv2 - fv0;
	auto normal = d01 ^ d02;
	normal.normalize();

	auto center = (fv0 + fv1 + fv2) / 3;

	vector<T> projected;

	ProjectToAxis(center, normal, points, projected);

	T pfv0;
	T pfv1;
	T pfv2;

	ProjectToAxis(center, normal, fv0, pfv0);
	ProjectToAxis(center, normal, fv1, pfv1);
	ProjectToAxis(center, normal, fv2, pfv2);

	ref_ptr<Vec3Array> pVertices = new Vec3Array(projected.begin(), projected.end());
	pVertices->push_back(pfv0);
	pVertices->push_back(pfv1);
	pVertices->push_back(pfv2);
	
	ref_ptr<osgUtil::DelaunayTriangulator> pDT = new osgUtil::DelaunayTriangulator;
	pDT->setInputPointArray(pVertices);
	pDT->triangulate();

	auto pTriangles = pDT->getTriangles();
	for (unsigned int i = 0; i < pTriangles->size() / 3; i++)
	{
		auto& v0 = pVertices->at(pDT->getTriangles()->at(i * 3 + 0));
		auto& v1 = pVertices->at(pDT->getTriangles()->at(i * 3 + 1));
		auto& v2 = pVertices->at(pDT->getTriangles()->at(i * 3 + 2));

		auto vd01 = v1 - v0;
		auto vd02 = v2 - v0;
		auto vnormal = vd01 ^ vd02;
		vnormal.normalize();
		if (normal * vnormal < 0)
		{
			result.push_back(make_tuple(v0, v2, v1));
		}
		else
		{
			result.push_back(make_tuple(v0, v1, v2));
		}
	}
}

template<typename T>
void OSGMP<T>::ProjectToPlane(const T& projectionPlanePosition, const T& projectionPlaneNormal, const T& inputPoint, T& projectedPoint)
{
	IntersectRayPlane<T>(inputPoint, -projectionPlaneNormal, projectionPlanePosition, projectionPlaneNormal, projectedPoint);
}

template<typename T>
void OSGMP<T>::ProjectToPlane(const T& projectionPlanePosition, const T& projectionPlaneNormal, const vector<T>& inputPoints, vector<T>& projectedPoints)
{
	for (auto& ip : inputPoints)
	{
		T rp;
		if (IntersectRayPlane<T>(ip, -projectionPlaneNormal, projectionPlanePosition, projectionPlaneNormal, rp))
		{
			projectedPoints.push_back(rp);
		}
		else
		{
			projectedPoints.push_back(ip);
		}
	}
}

template<typename T>
void OSGMP<T>::ProjectToAxis(const T& projectionPlanePosition, const T& projectionPlaneNormal, const T& inputPoint, T& projectedPoint)
{
	float x = projectionPlaneNormal.x();
	float y = projectionPlaneNormal.y();
	float z = projectionPlaneNormal.z();
	if (x > y && x > z)
	{
		ProjectToPlane(projectionPlanePosition, T(1, 0, 0), inputPoint, projectedPoint);
	}
	else if (y > x && y > z)
	{
		ProjectToPlane(projectionPlanePosition, T(0, 1, 0), inputPoint, projectedPoint);
	}
	else if (z > x && z > y)
	{
		ProjectToPlane(projectionPlanePosition, T(0, 0, 1), inputPoint, projectedPoint);
	}
}

template<typename T>
void OSGMP<T>::ProjectToAxis(const T& projectionPlanePosition, const T& projectionPlaneNormal, const vector<T>& inputPoints, vector<T>& projectedPoints)
{
	float x = projectionPlaneNormal.x();
	float y = projectionPlaneNormal.y();
	float z = projectionPlaneNormal.z();
	if (x > y && x > z)
	{
		ProjectToPlane(projectionPlanePosition, T(1, 0, 0), inputPoints, projectedPoints);
	}
	else if (y > x && y > z)
	{
		ProjectToPlane(projectionPlanePosition, T(0, 1, 0), inputPoints, projectedPoints);
	}
	else if (z > x && z > y)
	{
		ProjectToPlane(projectionPlanePosition, T(0, 0, 1), inputPoints, projectedPoints);
	}
}

template<typename T>
void MakeReverse(vector<pair<T, T>>& input)
{
	vector<pair<T, T>> temp(input);
	auto i = temp.rbegin();
	input.clear();
	while (i != temp.rend())
	{
		input.emplace_back(make_pair((*i).second, (*i).first));
		i++;
	}
}

template <typename T>
void SortPairs(const vector<pair<T, T>>& input, vector<vector<T>>& result)
{
	list<vector<pair<T, T>>> groups;
	for (auto& vv : input)
	{
		vector<pair<T, T>> group;
		if (vv.first < vv.second)
		{
			group.emplace_back(make_pair(vv.first, vv.second));
		}
		else
		{
			group.emplace_back(make_pair(vv.second, vv.first));
		}
		groups.emplace_back(group);
	}

	bool bProcessed = false;
	int lastGroupsCount = groups.size();
	int notProcessedCount = 0;
	do
	{
		bProcessed = false;
		vector<pair<T, T>> toProcess(groups.front());
		groups.pop_front();

		if (groups.size() == 0)
		{
			vector<T> group;
			for (auto& vv : toProcess)
			{
				group.emplace_back(vv.first);
			}
			group.emplace_back(toProcess.back().second);
			result.push_back(group);
			return;
		}

		auto gi = groups.begin();
		while (gi != groups.end())
		{
			auto& group = *gi;

			if (toProcess.back().second == group.front().first)
			{
				for (auto& vv : group)
				{
					toProcess.emplace_back(vv);
				}
				group.swap(toProcess);

				bProcessed = true;
				break;
			}
			else if (toProcess.front().first == group.front().first)
			{
				MakeReverse(toProcess);
				for (auto& vv : group)
				{
					toProcess.emplace_back(vv);
				}
				group.swap(toProcess);

				bProcessed = true;
				break;
			}
			else if (toProcess.back().second == group.back().second)
			{
				MakeReverse(group);
				for (auto& vv : group)
				{
					toProcess.emplace_back(vv);
				}
				group.swap(toProcess);

				bProcessed = true;
				break;
			}
			else if (toProcess.front().first == group.back().second)
			{
				MakeReverse(toProcess);
				MakeReverse(group);
				for (auto& vv : group)
				{
					toProcess.emplace_back(vv);
				}
				group.swap(toProcess);

				bProcessed = true;
				break;
			}
			else
			{
				gi++;
			}
		}
		if (bProcessed == false)
		{
			groups.emplace_back(toProcess);
			notProcessedCount++;
			bProcessed = true;
		}

		if (notProcessedCount == groups.size() * 2)
		{
			break;
		}
	} while (bProcessed == true);

	//groups.emplace_back(toProcess);
	for (auto& group : groups)
	{
		vector<T> output;
		for (auto& t : group)
		{
			output.emplace_back(t.first);
		}
		output.emplace_back(group.back().second);
		result.emplace_back(output);
	}
}
