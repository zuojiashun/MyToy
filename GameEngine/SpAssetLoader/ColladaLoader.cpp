#include"ColladaLoader.h"
#include<fstream>
#include<sstream>

using namespace std;
using namespace rapidxml;
USING_NAMESPACE_ENGINE

void ColladaLoader::LoadXmlDocument(const char* filename)
{
	ifstream is(filename);
	stringstream ss;
	ss << is.rdbuf();
	is.close();
	size_t size = sizeof(char) * ss.str().size();
	m_xmlData = (char*)malloc(size + 1);
	ss.read(m_xmlData, size);
	m_xmlData[size] = '\0';
	m_xmlDoc.parse<0>(m_xmlData);
}

vector<xml_node<>*> ColladaLoader::GetNodesByName(xml_node<>* node, const string name)
{
	vector<xml_node<>*> list;
	for (xml_node<> *it = node->first_node();
		it; it = it->next_sibling())
	{
		if (it->name() == name)
			list.push_back(it);
	}
	return list;
}

xml_node<>* ColladaLoader::GetNodeByName(xml_node<>* node, const string name)
{
	for (xml_node<> *it = node->first_node();
		it; it = it->next_sibling())
	{
		if (it->name() == name)
			return it;
	}
	return NULL;
}

xml_node<>* ColladaLoader::GetNodeById(xml_node<>* node, const string name)
{
	for (xml_node<> *it = node->first_node();
		it; it = it->next_sibling())
	{
		if (GetAttribute(it, "id") == name)
			return it;
	}
	return NULL;
}

string ColladaLoader::GetAttribute(xml_node<>* node, const string name)
{
	for (xml_attribute<> *attr = node->first_attribute();
		attr; attr = attr->next_attribute())
	{
		if (attr->name() == name)
			return attr->value();
	}
	return string("");
}

vector<string> ColladaLoader::UnpackValues(string& str, size_t count)
{
	int start_pos = 0, array_index = 0, str_size = 0, total_size = str.size();
	bool begin = false;
	vector<string> list;
	for (int i = 0; i <= total_size; ++i)
	{
		if (str[i] != ' ' && str[i] != '\r' && str[i] != '\n' && i < total_size)
		{
			if (!begin)
			{
				start_pos = i;
				begin = true;
			}
			str_size++;
		}
		else
		{
			if (begin)
			{
				begin = false;
				list.push_back(str.substr(start_pos, str_size));
				str_size = 0;
			}
		}
	}
	return list;
}

void ColladaLoader::AddSample(map<float, AnimationSample>& p_samples, float time, byte jointIndex, const JointPose& pose)
{
	auto it = p_samples.find(time);
	if (it == p_samples.end())
		p_samples.insert(make_pair(time, AnimationSample{ time }));
	AnimationSample& sample = p_samples[time];
	if (sample.m_JointPoses.find(jointIndex) == sample.m_JointPoses.end())
	{
		sample.m_JointPoses.insert(make_pair(jointIndex, pose));
	}
}

vector<string> ColladaLoader::ReadSource(xml_node<>* node, const string id)
{
	map<string, vector<string>> dic;
	xml_node<>* source_node = GetNodeById(node, id);
	xml_node<>* accessor_node = GetNodeByName(source_node, "technique_common")->first_node();
	int count = GetAttribute<int>(accessor_node, "count");
	string source_id = RemoveAtFirst(GetAttribute(accessor_node, "source"));
	xml_node<>* array_node = GetNodeById(source_node, source_id);
	count = GetAttribute<int>(array_node, "count");
	string value = array_node->value();
	return  UnpackValues(value, count);
}

map<string, vector<string>> ColladaLoader::ReadSources(xml_node<>* node)
{
	map<string, vector<string>> dic;
	vector<xml_node<>*> nodes = GetNodesByName(node, "source");
	for (xml_node<>* source_node : nodes)
	{
		xml_node<>* accessor_node = GetNodeByName(source_node, "technique_common")->first_node();
		int count = GetAttribute<int>(accessor_node, "count");
		string source_id = GetAttribute(accessor_node, "source");
		source_id = source_id.substr(1, source_id.size());
		xml_node<>* array_node = GetNodeById(source_node, source_id);
		count = GetAttribute<int>(array_node, "count");
		string value = array_node->value();
		dic.insert(make_pair("#" + GetAttribute(array_node, "id"), UnpackValues(value, count)));
	}
	return dic;
}

void ColladaLoader::ReadSkeleton(xml_node<>* node, Model* model)
{
	model->m_Skeleton = make_shared<Skeleton>();
	xml_node<>* scene_root = GetNodeByName(node, "library_visual_scenes")->first_node();
	for (xml_node<> *it = scene_root->first_node(); it; it = it->next_sibling())
	{
		if (GetNodeByName(it, "matrix") != NULL)
		{
			ReadJoint(it, JOINT_ROOT, 0, model);
		}
	}
	model->m_Skeleton->m_GlobalPoses.resize(model->m_Skeleton->GetSize());
	model->m_Skeleton->m_LocalPoses.resize(model->m_Skeleton->GetSize());
	model->m_Skeleton->m_SkiningMatrices.resize(model->m_Skeleton->GetSize());
}

void ColladaLoader::ReadJoint(xml_node<>* joint_node, byte parent_ref, int depth, Model* model)
{
	string values = GetNodeByName(joint_node, "matrix")->value();
	float* m = UnpackValues<float>(values, 16);
	Joint joint;
	joint.m_LocalMatrix = Matrix4x4(m);
	joint.m_Name = GetAttribute(joint_node, "name");
	joint.m_iParent = parent_ref;
	joint.m_Index = model->m_Skeleton->GetSize();
	model->m_Skeleton->AddJoint(joint);
	byte index = model->m_Skeleton->GetSize() - 1;
	//--------------------打印关节树----------------------
	//string str;
	//for (int i = 0; i < depth; i++)
	//{
	//	str += "   ";
	//}
	//Debug::Log("%s p:%d i:%d", (str + GetAttribute(joint_node, "name")).c_str(), parent_ref, joint.m_Index);

	for (xml_node<> *it = joint_node->first_node(); it; it = it->next_sibling())
	{
		if (GetNodeByName(it, "matrix") != NULL)
		{
			ReadJoint(it, index, depth + 1, model);
		}
	}
}

void ColladaLoader::ReadSkin(xml_node<>* root, vector<Vector4>& weights, vector<BVector4>& indices, Model* model)
{
	//读取蒙皮
	if (GetNodeByName(root, "library_controllers") == NULL)
		return;
	xml_node<>* skin_node = GetNodeByName(root, "library_controllers")->first_node()->first_node();
	string mat_value = GetNodeByName(skin_node, "bind_shape_matrix")->value();
	float* p_mat = UnpackValues<float>(mat_value, 16);
	model->m_Skeleton->m_BindShapeMat = p_mat;
	free(p_mat);
	xml_node<>* joint_node = GetNodeByName(skin_node, "joints");
	vector<xml_node<>*> joint_inputs = GetNodesByName(joint_node, "input");
	vector<string> joint_source;
	vector<float> weight_source;
	vector<Matrix4x4> matrix_source;
	for (xml_node<>* input : joint_inputs)
	{
		string semantic = GetAttribute(input, "semantic");
		string source_id = RemoveAtFirst(GetAttribute(input, "source"));
		int count = 0;
		if (semantic == "JOINT")
		{
			joint_source = ReadSource(skin_node, source_id);
		}
		else if (semantic == "INV_BIND_MATRIX")
		{
			float* f = ReadSource<float>(skin_node, source_id, &count);
			for (int i = 0; i < count; i += 16)
			{
				matrix_source.push_back(Matrix4x4(
					f[i + 0], f[i + 1], f[i + 2], f[i + 3],
					f[i + 4], f[i + 5], f[i + 6], f[i + 7],
					f[i + 8], f[i + 9], f[i + 10], f[i + 11],
					f[i + 12], f[i + 13], f[i + 14], f[i + 15]
				));
			}
			free(f);
		}
	}

	xml_node<>* vertex_weights_node = GetNodeByName(skin_node, "vertex_weights");
	int weights_count = GetAttribute<int>(vertex_weights_node, "count");
	vector<xml_node<>*> weights_inputs = GetNodesByName(vertex_weights_node, "input");
	int weights_offsets[2];
	for (xml_node<>* weights_input : weights_inputs)
	{
		string semantic = GetAttribute(weights_input, "semantic");
		int offset = GetAttribute<int>(weights_input, "offset");
		if (semantic == "JOINT")
		{
			weights_offsets[0] = offset;
		}
		else if (semantic == "WEIGHT")
		{
			string source_id = RemoveAtFirst(GetAttribute(weights_input, "source"));
			weights_offsets[1] = offset;
			int count = 0;
			float* f = ReadSource<float>(skin_node, source_id, &count);
			for (int i = 0; i < count; ++i)
			{
				weight_source.push_back(f[i]);
			}
			free(f);
		}
	}
	string vcount_str = GetNodeByName(vertex_weights_node, "vcount")->value();
	string v_str = GetNodeByName(vertex_weights_node, "v")->value();
	int* vcount = UnpackValues<int>(vcount_str, weights_count);
	int v_length = 0;
	for (int i = 0; i < weights_count; i++)
		v_length += vcount[i] * 2;
	int* v_array = UnpackValues<int>(v_str, v_length);
	weights.resize(weights_count);
	indices.resize(weights_count);

	for (int i = 0, v = 0; i < weights_count; i++)
	{
		int num = vcount[i];
		for (int j = 0; j < num; j++)
		{
			indices[i][j] = model->m_Skeleton->GetJointIndex(joint_source[v_array[weights_offsets[0] + v + j * 2]]);
			weights[i][j] = weight_source[v_array[weights_offsets[1] + v + j * 2]];
		}
		v += num * 2;
	}

	for (uint i = 0; i < joint_source.size(); i++)
	{
		Joint& joint = *model->m_Skeleton->GetJoint(joint_source[i]);
		joint.m_InvBindPose = matrix_source[i];
	}
}

void ColladaLoader::ReadMesh(xml_node<>* root, vector<Vector4>& weights, vector<BVector4>& weight_indices, Model* model)
{
	if (GetNodeByName(root, "library_geometries") == NULL)
		return;
	//读取几何信息
	xml_node<>* mesh = GetNodeByName(root, "library_geometries")->first_node()->first_node();

	//读取source
	map<string, ValueArray<float>> source_map;
	vector<xml_node<>*> sources = GetNodesByName(mesh, "source");
	for (auto it = sources.begin(); it != sources.end(); ++it)
	{
		xml_node<>* float_array = GetNodeByName(*it, "float_array");
		int count = GetAttribute<int>(float_array, "count");
		string values = float_array->value();
		float* v3 = UnpackValues<float>(values, count);
		source_map.insert(make_pair("#" + GetAttribute(*it, "id"), ValueArray<float>{ v3, count }));
	}

	//根据下标读取实际的值
	int m_triangleNum = 0;
	vector<xml_node<>*> triangles = GetNodesByName(mesh, "triangles");
	for (auto it = triangles.begin(); it != triangles.end(); ++it)
	{
		m_triangleNum += GetAttribute<int>(*it, "count");
	}
	MeshPtr p_mesh = make_shared<Mesh>();
	model->m_Meshes.push_back(p_mesh);
	p_mesh->m_VertexCount = m_triangleNum * 3;

	int vertIndex = 0, normalIndex = 0, uvIndex = 0; //读取时已到达的索引
	p_mesh->m_Vertices = (Vector3*)malloc(sizeof(Vector3) * p_mesh->m_VertexCount);
	p_mesh->m_Normals = (Vector3*)malloc(sizeof(Vector3) * p_mesh->m_VertexCount);
	p_mesh->m_Texcoords = (Vector2*)malloc(sizeof(Vector2) * p_mesh->m_VertexCount);
	p_mesh->m_JointWeights = (Vector4*)malloc(sizeof(Vector4) * p_mesh->m_VertexCount);
	p_mesh->m_JointIndices = (BVector4*)malloc(sizeof(BVector4) * p_mesh->m_VertexCount);

	int index_num = 0;
	bool hasSkeletonInfo = weights.size() > 0;
	for (auto it = triangles.begin(); it != triangles.end(); ++it)
	{
		int count = GetAttribute<int>(*it, "count");
		vector<xml_node<>*> inputs = GetNodesByName(*it, "input");
		string p = GetNodeByName(*it, "p")->value();

		bool flags[3];
		int offsets[3];
		int step = 0;
		string sourceIds[3];
		for (auto it_input = inputs.begin(); it_input != inputs.end(); ++it_input, ++step)
		{
			string semantic = GetAttribute(*it_input, "semantic");
			int offset = GetAttribute<int>(*it_input, "offset");
			string source = GetAttribute(*it_input, "source");
			int index = 0;
			if (semantic == "VERTEX")
				source = GetAttribute(GetNodeByName(mesh, "vertices")->first_node(), "source");
			else if (semantic == "NORMAL")
				index = 1;
			else if (semantic == "TEXCOORD")
				index = 2;
			flags[index] = true;
			offsets[index] = offset;
			sourceIds[index] = source;
		}
		index_num = count * 3 * step;
		int* indices = UnpackValues<int>(p, (size_t)index_num);

		for (int i = 0; i < index_num; i += step)
		{
			int vi = indices[i + offsets[0]];
			if (hasSkeletonInfo)
			{
				p_mesh->m_JointWeights[vertIndex] = weights[vi];
				p_mesh->m_JointIndices[vertIndex] = weight_indices[vi];
			}
			if (flags[0])
				p_mesh->m_Vertices[vertIndex++] = ((Vector3*)source_map[sourceIds[0]].array)[vi];
			if (flags[1])
				p_mesh->m_Normals[normalIndex++] = ((Vector3*)source_map[sourceIds[1]].array)[indices[i + offsets[1]]];
			if (flags[2])
				p_mesh->m_Texcoords[uvIndex++] = ((Vector2*)source_map[sourceIds[2]].array)[indices[i + offsets[2]]];
		}
		free(indices);
	}
}

void ColladaLoader::ReadAnimation(xml_node<>* root, Model* model)
{
	xml_node<>* lib_animation_node = GetNodeByName(root, "library_animations");
	if (!lib_animation_node)
		return;

	//把所有骨骼同一时间的采样放在一起
	map<float, AnimationSample> p_samples;
	vector<xml_node<>*> animation_nodes = GetNodesByName(lib_animation_node, "animation");
	for (xml_node<>* animation_node : animation_nodes)
	{
		animation_node = animation_node->first_node();
		xml_node<>* channel_node = GetNodeByName(animation_node, "channel");
		string channel_source_id = RemoveAtFirst(GetAttribute(channel_node, "source"));
		string channel_target_joint = GetAttribute(channel_node, "target");
		channel_target_joint = channel_target_joint.substr(0, channel_target_joint.find('/'));
		byte joint_index = model->m_Skeleton->GetJointIndex(channel_target_joint);
		Matrix4x4* matrices = NULL;
		float* times = NULL;
		int sample_count = 0;
		for (xml_node<>* input_node : GetNodesByName(GetNodeById(animation_node, channel_source_id), "input"))
		{
			string semantic = GetAttribute(input_node, "semantic");
			string source_id = RemoveAtFirst(GetAttribute(input_node, "source"));
			if (semantic == "INTERPOLATION")
				continue;
			xml_node<>* float_array_node = GetNodeByName(GetNodeById(animation_node, source_id), "float_array");
			int count = GetAttribute<int>(float_array_node, "count");
			if (count == 0)
				continue;
			string value = float_array_node->value();
			float* float_array = UnpackValues<float>(value, count);
			if (semantic == "INPUT")
			{
				times = float_array;
				sample_count = count;
			}
			else if (semantic == "OUTPUT")
			{
				matrices = (Matrix4x4*)float_array;
			}
		}
		if (matrices == NULL || times == NULL)
			continue;
		for (int i = 0; i < sample_count; ++i)
		{
			AddSample(p_samples, times[i], joint_index, JointPose{ matrices[i].Transpose() });
		}
		free(matrices);
		free(times);
	}
	AnimationClipPtr clip = make_shared<AnimationClip>();
	model->m_Animations.push_back(clip);
	clip->m_IsLooping = true;
	clip->m_aSamples.clear();
	int index = 0;
	for (pair<float, AnimationSample> kv : p_samples)
		clip->m_aSamples.push_back(kv.second);
	if (clip->m_aSamples.size() > 0)
	{
		clip->m_Length = clip->m_aSamples[clip->m_aSamples.size() - 1].m_Time;
	}
}

ModelPtr ColladaLoader::LoadFromFile(const char* filename)
{
	ModelPtr model = make_shared<Model>();
	ReleaseSource();
	LoadXmlDocument(filename);
	xml_node<>* root = m_xmlDoc.first_node();
	ReadSkeleton(root, model.get());
	vector<Vector4> weights;
	vector<BVector4> indices;
	ReadSkin(root, weights, indices, model.get());
	ReadMesh(root, weights, indices, model.get());
	ReadAnimation(root, model.get());
	SkeletonAnimation::CalculateGlobalMatrix(model->m_Skeleton);
	SkeletonAnimation::CalculateSkinningMatrix(model->m_Skeleton);
	return model;
}

void ColladaLoader::ReleaseSource()
{
	if (m_xmlData)
	{
		free(m_xmlData);
		m_xmlData;
	}
}
