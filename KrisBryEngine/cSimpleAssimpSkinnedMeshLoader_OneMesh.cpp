#include "cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <glad/glad.h>

#include <sstream>


glm::mat4 AIMatrixToGLMMatrix(const aiMatrix4x4 &mat) {
	
	return glm::mat4(mat.a1, mat.b1, mat.c1, mat.d1,	
					mat.a2, mat.b2, mat.c2, mat.d2,
					mat.a3, mat.b3, mat.c3, mat.d3,
					mat.a4, mat.b4, mat.c4, mat.d4);
}


cSimpleAssimpSkinnedMesh::cSimpleAssimpSkinnedMesh(void) {
	
	this->pScene = 0;
	this->mNumBones = 0;

	//this->m_VBO_ID = 0;				// Vertex buffer object 
	this->m_numberOfVertices = 0;
	//this->m_indexBuf_ID = 0;		// Index buffer referring to VBO
	this->m_numberOfIndices = 0;
	this->m_numberOfTriangles = 0;
	//this->m_bVBO_created = false;

	//this->m_VAO_ID = 0;				// Vertex Array Object
	//this->m_bVAO_created = false;
	//this->m_shaderIDMatchingVAO = 0;
	return;
}


cSimpleAssimpSkinnedMesh::~cSimpleAssimpSkinnedMesh(void) {
	
	this->shutErDown();

	return;
}

void cSimpleAssimpSkinnedMesh::shutErDown(void) {
	
	//if ( this->m_pVertexData )
	//{
	//	delete [] this->m_pVertexData;
	//}
	// TODO: Delete the OpenGL buffers, too??

	return;
}

bool cSimpleAssimpSkinnedMesh::loadMeshFromFile(const std::string &filename) {
	
	unsigned int Flags = aiProcess_Triangulate | 
		                 aiProcess_OptimizeMeshes | 
		                 aiProcess_OptimizeGraph | 
		                 aiProcess_JoinIdenticalVertices |
						 aiProcess_GenNormals |
						 aiProcess_CalcTangentSpace;

	this->pScene = this->mImporter.ReadFile(filename.c_str(), Flags);

	if (this->pScene ) {
		
		this->fileName = filename;
		// Assume the friendlyName is the same as the file, for now
		this->friendlyName = filename;

		this->mGlobalInverseTransformation = AIMatrixToGLMMatrix( pScene->mRootNode->mTransformation );
		this->mGlobalInverseTransformation = glm::inverse(this->mGlobalInverseTransformation);
		
		// Calcualte all the bone things
		if (!this->initialize()) {	// There was an issue doing this calculation
			return false;
		}
	}//if ( this->pScene )

	return true;
}


// Looks in the animation map and returns the total time
float cSimpleAssimpSkinnedMesh::findAnimationTotalTime(std::string animationName) {
	
	std::map< std::string /*animation FRIENDLY name*/,
		sAnimationInfo >::iterator itAnimation = this->mapAnimationFriendlyNameTo_pScene.find(animationName);
	
	// Found it? 
	if (itAnimation == this->mapAnimationFriendlyNameTo_pScene.end()) {	// Nope.
		return 0.0f;
	}

	// This is scaling the animation from 0 to 1
	return (float)itAnimation->second.pAIScene->mAnimations[0]->mDuration;
}

float cSimpleAssimpSkinnedMesh::findAnimationTicks(std::string animationName) {
	
	std::map<std::basic_string<char>, sAnimationInfo>::iterator itAnimation = 
		this->mapAnimationFriendlyNameTo_pScene.find(animationName);

	// Found it? 
	if (itAnimation == this->mapAnimationFriendlyNameTo_pScene.end()) {	// Nope.
		return 0.0f;
	}

	// This is scaling the animation from 0 to 1
	return (float)itAnimation->second.pAIScene->mAnimations[0]->mTicksPerSecond;
}


bool cSimpleAssimpSkinnedMesh::loadMeshAnimation(const std::string &friendlyName, const std::string &filename) {	// Only want animations

	// Already loaded this ?
		std::map< std::string /*animation FRIENDLY name*/,
		sAnimationInfo >::iterator itAnimation = this->mapAnimationFriendlyNameTo_pScene.find(friendlyName);

	// Found it? 
	if (itAnimation != this->mapAnimationFriendlyNameTo_pScene.end())
	{	// Yup. So we already loaded it.
		return false;
	}

	unsigned int Flags = aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices;

	Assimp::Importer* pImporter = new Assimp::Importer();

	sAnimationInfo animInfo;
	animInfo.friendlyName = friendlyName;
	animInfo.fileName = filename;
	animInfo.pAIScene = pImporter->ReadFile(animInfo.fileName.c_str(), Flags);

	if (!animInfo.pAIScene)
	{
		return false;
	}
	
	// Animation scene is loaded (we assume)
	// Add it to the map
	this->mapAnimationFriendlyNameTo_pScene[animInfo.friendlyName] = animInfo;

	return true;
}


bool cSimpleAssimpSkinnedMesh::initialize(void) {
	
	this->m_numberOfVertices = this->pScene->mMeshes[0]->mNumVertices;

	// This is the vertex information for JUST the bone stuff
	this->vecVertexBoneData.resize(this->m_numberOfVertices);

	this->loadBones( this->pScene->mMeshes[0], this->vecVertexBoneData );

	return true;
}


void cSimpleAssimpSkinnedMesh::sVertexBoneData::addBoneData(unsigned int BoneID, float Weight) {
	
	for ( unsigned int Index = 0; Index < sizeof(this->ids) / sizeof(this->ids[0]); Index++ ) {
		
		if (this->weights[Index] == 0.0f) {
			this->ids[Index]		= (float)BoneID;
			this->weights[Index]	= Weight;
			return;
		}
	}
}

// In the original code, these vectors are being passed out into the "character" object.
// It's unclear what the Globals matrices are actually for...
//void cSimpleAssimpSkinnedMesh::BoneTransform( float TimeInSeconds, 
//                                              std::vector<glm::mat4> &FinalTransformation, 
//								              std::vector<glm::mat4> &Globals, 
//								              std::vector<glm::mat4> &Offsets)
void cSimpleAssimpSkinnedMesh::boneTransform( float TimeInSeconds, 
											  std::string animationName,		// Now we can pick the animation
                                              std::vector<glm::mat4> &FinalTransformation, 
								              std::vector<glm::mat4> &Globals, 
								              std::vector<glm::mat4> &Offsets) {
	glm::mat4 Identity(1.0f);

	float TicksPerSecond = this->findAnimationTicks(animationName);
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, this->findAnimationTotalTime(animationName));
	
	// use the "animation" file to look up these nodes
	// (need the matOffset information from the animation file)
	this->readNodeHeirarchy(AnimationTime, animationName, this->pScene->mRootNode, Identity);

	FinalTransformation.resize(this->mNumBones);
	Globals.resize(this->mNumBones);
	Offsets.resize(this->mNumBones);

	for (unsigned int BoneIndex = 0; BoneIndex < this->mNumBones; BoneIndex++) {
		
		FinalTransformation[BoneIndex] = this->mBoneInfo[BoneIndex].FinalTransformation;
		Globals[BoneIndex] = this->mBoneInfo[BoneIndex].ObjectBoneTransformation;
		Offsets[BoneIndex] = this->mBoneInfo[BoneIndex].BoneOffset;
	}

	return;
}

// Added to assist with the forward kinematics (id and bone names)
void cSimpleAssimpSkinnedMesh::getListOfBoneIDandNames(std::vector<std::string>& vecBoneNames) {
	
	// Go through the list of channels and return the id and name
	for (unsigned int chanID = 0; chanID != this->pScene->mAnimations[0]->mNumChannels; chanID++) {
		
		std::string boneName;
		boneName.append(this->pScene->mAnimations[0]->mChannels[chanID]->mNodeName.data);
		vecBoneNames.push_back(boneName);
	}

	return;
}


void cSimpleAssimpSkinnedMesh::loadBones(const aiMesh* Mesh, std::vector<sVertexBoneData> &vertexBoneData) {
	
	for (unsigned int boneIndex = 0; boneIndex != Mesh->mNumBones; boneIndex++) {
		
		unsigned int BoneIndex = 0;
		std::string BoneName(Mesh->mBones[boneIndex]->mName.data);

		std::map<std::string, unsigned int>::iterator it = this->m_mapBoneNameToBoneIndex.find(BoneName);
		if (it == this->m_mapBoneNameToBoneIndex.end()) {
			BoneIndex = this->mNumBones;
			this->mNumBones++;
			sBoneInfo bi;
			this->mBoneInfo.push_back(bi);

			this->mBoneInfo[BoneIndex].BoneOffset = AIMatrixToGLMMatrix(Mesh->mBones[boneIndex]->mOffsetMatrix);
			this->m_mapBoneNameToBoneIndex[BoneName] = BoneIndex;
		}
		else {
			BoneIndex = it->second;
		}

		for (unsigned int WeightIndex = 0; WeightIndex != Mesh->mBones[boneIndex]->mNumWeights; WeightIndex++) {
			
			unsigned int VertexID = /*mMeshEntries[MeshIndex].BaseVertex +*/ Mesh->mBones[boneIndex]->mWeights[WeightIndex].mVertexId;
			float Weight = Mesh->mBones[boneIndex]->mWeights[WeightIndex].mWeight;
			vertexBoneData[VertexID].addBoneData(BoneIndex, Weight);
		}
	}
	return;
}

const aiNodeAnim* cSimpleAssimpSkinnedMesh::findNodeAnimationChannel(const aiAnimation* pAnimation, aiString boneName) {
	
	for (unsigned int ChannelIndex = 0; ChannelIndex != pAnimation->mNumChannels; ChannelIndex++) {
		if (pAnimation->mChannels[ChannelIndex]->mNodeName == boneName) {
			return pAnimation->mChannels[ChannelIndex];
		}
	}
	return 0;
}


unsigned int cSimpleAssimpSkinnedMesh::findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim) {
	
	for (unsigned int RotationKeyIndex = 0; RotationKeyIndex != pNodeAnim->mNumRotationKeys - 1; RotationKeyIndex++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[RotationKeyIndex+1].mTime) {
			return RotationKeyIndex;
		}
	}

	return 0;
}

unsigned int cSimpleAssimpSkinnedMesh::findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim) {
	
	for (unsigned int PositionKeyIndex = 0; PositionKeyIndex != pNodeAnim->mNumPositionKeys - 1; PositionKeyIndex++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[PositionKeyIndex+1].mTime) {
			return PositionKeyIndex;
		}
	}

	return 0;
}

unsigned int cSimpleAssimpSkinnedMesh::findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim) {
	for (unsigned int ScalingKeyIndex = 0; ScalingKeyIndex != pNodeAnim->mNumScalingKeys - 1; ScalingKeyIndex++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[ScalingKeyIndex+1].mTime) {
			return ScalingKeyIndex;
		}
	}

	return 0;
}

void cSimpleAssimpSkinnedMesh::readNodeHeirarchy(float AnimationTime, 
												 std::string animationName, 
												 const aiNode* pNode,
												 const glm::mat4 &ParentTransformMatrix) {
	aiString NodeName(pNode->mName.data);

	// Original version picked the "main scene" animation...
	const aiAnimation* pAnimation = this->pScene->mAnimations[0];

	// Search for the animation we want... 
	std::map< std::string /*animation FRIENDLY name*/,
		sAnimationInfo >::iterator itAnimation = this->mapAnimationFriendlyNameTo_pScene.find(animationName);		// Animations

	// Did we find it? 
	if (itAnimation != this->mapAnimationFriendlyNameTo_pScene.end()) {
		// Yes, there is an animation called that...
		// ...replace the animation with the one we found
		pAnimation = reinterpret_cast<const aiAnimation*>(itAnimation->second.pAIScene->mAnimations[0]);
	}

	// Transformation of the node in bind pose
	glm::mat4 NodeTransformation = AIMatrixToGLMMatrix( pNode->mTransformation );

	const aiNodeAnim* pNodeAnim = this->findNodeAnimationChannel(pAnimation, NodeName);

	if (pNodeAnim) {
		// Get interpolated scaling
		glm::vec3 scale;
		this->calcGLMInterpolatedScaling(AnimationTime, pNodeAnim, scale);
		glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), scale);

		// Get interpolated rotation (quaternion)
		glm::quat ori;
		this->calcGLMInterpolatedRotation(AnimationTime, pNodeAnim, ori);
		glm::mat4 RotationM = glm::mat4_cast(ori);
		
		// Get interpolated position 
		glm::vec3 pos;
		this->calcGLMInterpolatedPosition(AnimationTime, pNodeAnim, pos);
		glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), pos);
		
		
		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 ObjectBoneTransformation = ParentTransformMatrix * NodeTransformation;

	std::map<std::string, unsigned int>::iterator it = this->m_mapBoneNameToBoneIndex.find(std::string(NodeName.data));
	if (it != this->m_mapBoneNameToBoneIndex.end()) {
		
		unsigned int BoneIndex = it->second;
		this->mBoneInfo[BoneIndex].ObjectBoneTransformation = ObjectBoneTransformation;
		this->mBoneInfo[BoneIndex].FinalTransformation = this->mGlobalInverseTransformation 
		                                                 * ObjectBoneTransformation 
			                                             * this->mBoneInfo[BoneIndex].BoneOffset;
	}
	else {
		int breakpoint = 0;
	}

	for (unsigned int ChildIndex = 0; ChildIndex != pNode->mNumChildren; ChildIndex++) {
		
		this->readNodeHeirarchy( AnimationTime, animationName,
								 pNode->mChildren[ChildIndex], ObjectBoneTransformation);
	}

	return;
}

void cSimpleAssimpSkinnedMesh::calcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, aiQuaternion &out) {
	
	if (pNodeAnim->mNumRotationKeys == 1) {
		out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = this->findRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);

	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(out, StartRotationQ, EndRotationQ, Factor);
	out = out.Normalize();

	return;
}

void cSimpleAssimpSkinnedMesh::calcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D &out) {
	
	if (pNodeAnim->mNumPositionKeys == 1) {
		out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = this->findPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D& StartPosition = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& EndPosition = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	out = (EndPosition - StartPosition) * Factor + StartPosition;

	return;
}

void cSimpleAssimpSkinnedMesh::calcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D &out) {
	
	if (pNodeAnim->mNumScalingKeys == 1) {
		out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = this->findScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);

	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D& StartScale = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& EndScale = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	out = (EndScale - StartScale) * Factor + StartScale;

	return;
}

void cSimpleAssimpSkinnedMesh::calcGLMInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::quat &out) {
	
	if (pNodeAnim->mNumRotationKeys == 1) {
		out.w = pNodeAnim->mRotationKeys[0].mValue.w;
		out.x = pNodeAnim->mRotationKeys[0].mValue.x;
		out.y = pNodeAnim->mRotationKeys[0].mValue.y;
		out.z = pNodeAnim->mRotationKeys[0].mValue.z;
		return;
	}

	unsigned int RotationIndex = this->findRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);

	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	if ( Factor < 0.0f ) Factor = 0.0f;
	if ( Factor > 1.0f ) Factor = 1.0f;
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	
	glm::quat StartGLM = glm::quat(StartRotationQ.w, StartRotationQ.x, StartRotationQ.y, StartRotationQ.z);
	glm::quat EndGLM = glm::quat(EndRotationQ.w, EndRotationQ.x, EndRotationQ.y, EndRotationQ.z);

	out = glm::slerp(StartGLM, EndGLM, Factor);

	out = glm::normalize(out);

	return;
}

void cSimpleAssimpSkinnedMesh::calcGLMInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3 &out) {
	
	if (pNodeAnim->mNumPositionKeys == 1) {
		out.x = pNodeAnim->mPositionKeys[0].mValue.x;
		out.y = pNodeAnim->mPositionKeys[0].mValue.y;
		out.z = pNodeAnim->mPositionKeys[0].mValue.z;
		return;
	}

	unsigned int PositionIndex = this->findPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	if ( Factor < 0.0f ) Factor = 0.0f;
	if ( Factor > 1.0f ) Factor = 1.0f;
	const aiVector3D& StartPosition = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& EndPosition = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	glm::vec3 start = glm::vec3(StartPosition.x, StartPosition.y, StartPosition.z);
	glm::vec3 end = glm::vec3(EndPosition.x, EndPosition.y, EndPosition.z);

	out = (end - start) * Factor + start;

	return;
}

void cSimpleAssimpSkinnedMesh::calcGLMInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3 &out) {
	
	if (pNodeAnim->mNumScalingKeys == 1) {
		out.x = pNodeAnim->mScalingKeys[0].mValue.x;
		out.y = pNodeAnim->mScalingKeys[0].mValue.y;
		out.z = pNodeAnim->mScalingKeys[0].mValue.z;
		return;
	}

	unsigned int ScalingIndex = this->findScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);

	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	if ( Factor < 0.0f ) Factor = 0.0f;
	if ( Factor > 1.0f ) Factor = 1.0f;
	const aiVector3D& StartScale = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& EndScale = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	glm::vec3 start = glm::vec3(StartScale.x, StartScale.y, StartScale.z);
	glm::vec3 end = glm::vec3(EndScale.x, EndScale.y, EndScale.z);
	out = (end - start) * Factor + start;

	return;
}

float cSimpleAssimpSkinnedMesh::getDuration(std::string animationName) {
	
	std::map< std::string /*animation FRIENDLY name*/,
		sAnimationInfo >::iterator itAnimation = this->mapAnimationFriendlyNameTo_pScene.find(animationName);

	// Found it? 
	if (itAnimation == this->mapAnimationFriendlyNameTo_pScene.end()) {	// Nope.
		return 0.0f;
	}

	// This is scaling the animation from 0 to 1
	return (float)itAnimation->second.pAIScene->mAnimations[0]->mDuration / (float)itAnimation->second.pAIScene->mAnimations[0]->mTicksPerSecond;
}


// Returns NULL (0) if there is no mesh at that index
cMesh* cSimpleAssimpSkinnedMesh::createMeshObjectFromCurrentModel( unsigned int meshIndex /*=0*/) {
	
	if (this->pScene->mNumMeshes < meshIndex) {	// Doesn't have this mesh
		return NULL;
	}

	// Assume there is a valid mesh there
	cMesh* pTheMesh = new cMesh();

	aiMesh* pAIMesh = this->pScene->mMeshes[meshIndex];

	pTheMesh->numberOfVertices = pAIMesh->mNumVertices;

	pTheMesh->pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[ pTheMesh->numberOfVertices ];

	for (int vertIndex = 0; vertIndex != pTheMesh->numberOfVertices; vertIndex++) {
		
		sVertex_xyz_rgba_n_uv2_bt_4Bones* pCurVert = &( pTheMesh->pVertices[vertIndex] );

		aiVector3D* pAIVert =&(pAIMesh->mVertices[vertIndex]);

		pCurVert->x = pAIVert->x;
		pCurVert->y = pAIVert->y;
		pCurVert->z = pAIVert->z;

		// Colours
		if (pAIMesh->GetNumColorChannels() > 0) {
			
			pCurVert->r = this->pScene->mMeshes[0]->mColors[vertIndex]->r;
			pCurVert->g = this->pScene->mMeshes[0]->mColors[vertIndex]->g;
			pCurVert->b = this->pScene->mMeshes[0]->mColors[vertIndex]->b;
			pCurVert->a = this->pScene->mMeshes[0]->mColors[vertIndex]->a;
		}
		else {
			pCurVert->r = pCurVert->g = pCurVert->b = pCurVert->a = 1.0f;
		}

		// Normals
		if (pAIMesh->HasNormals()) {
			
			pCurVert->nx = pAIMesh->mNormals[vertIndex].x;
			pCurVert->ny = pAIMesh->mNormals[vertIndex].y;
			pCurVert->nx = pAIMesh->mNormals[vertIndex].z;
		}

		// UVs
		if (pAIMesh->GetNumUVChannels() > 0) {	// Assume 1st channel is the 2D UV coordinates
			
			pCurVert->u0 = pAIMesh->mTextureCoords[0][vertIndex].x;
			pCurVert->v0 = pAIMesh->mTextureCoords[0][vertIndex].y;
			
			if ( pAIMesh->GetNumUVChannels() > 1 ) {
				
				pCurVert->u1 = pAIMesh->mTextureCoords[1][vertIndex].x;
				pCurVert->v1 = pAIMesh->mTextureCoords[1][vertIndex].y;
			}
		}

		// Tangents and Bitangents (bi-normals)
		if (pAIMesh->HasTangentsAndBitangents()) {
			
			pCurVert->tx = pAIMesh->mTangents[vertIndex].x;
			pCurVert->ty = pAIMesh->mTangents[vertIndex].y;
			pCurVert->tz = pAIMesh->mTangents[vertIndex].z;

			pCurVert->bx = pAIMesh->mBitangents[vertIndex].x;
			pCurVert->by = pAIMesh->mBitangents[vertIndex].y;
			pCurVert->bz = pAIMesh->mBitangents[vertIndex].z;
		}

		// Bone IDs are being passed OK
		pCurVert->boneID[0] = this->vecVertexBoneData[vertIndex].ids[0];
		pCurVert->boneID[1] = this->vecVertexBoneData[vertIndex].ids[1];
		pCurVert->boneID[2] = this->vecVertexBoneData[vertIndex].ids[2];
		pCurVert->boneID[3] = this->vecVertexBoneData[vertIndex].ids[3];

		// Weights are being passed OK
		pCurVert->boneWeights[0] = this->vecVertexBoneData[vertIndex].weights[0];
		pCurVert->boneWeights[1] = this->vecVertexBoneData[vertIndex].weights[1];
		pCurVert->boneWeights[2] = this->vecVertexBoneData[vertIndex].weights[2];
		pCurVert->boneWeights[3] = this->vecVertexBoneData[vertIndex].weights[3];


	}//for ( int vertIndex

	// Triangles
	pTheMesh->numberOfTriangles = pAIMesh->mNumFaces;
	pTheMesh->pTriangles = new cTriangle[pTheMesh->numberOfTriangles];

	for (unsigned int triIndex = 0; triIndex != pTheMesh->numberOfTriangles; triIndex++) {
		
		aiFace* pAIFace = &(pAIMesh->mFaces[triIndex]);

		pTheMesh->pTriangles[triIndex].vertex_ID_0 = pAIFace->mIndices[0];
		pTheMesh->pTriangles[triIndex].vertex_ID_1 = pAIFace->mIndices[1];
		pTheMesh->pTriangles[triIndex].vertex_ID_2 = pAIFace->mIndices[2];

	}//for ( unsigned int triIndex...

	pTheMesh->name = this->friendlyName;
	pTheMesh->calculateExtents();

	return pTheMesh;
}



//// Creates a VBO, loads the current mesh, then creates a VAO for the current VBO+shader
////bool cAssimpMesh::CreateVBOandVOAfromCurrentMesh(int shaderID, unsigned int &VBO_ID, unsigned int &VAO_ID )
//bool cSimpleAssimpSkinnedMesh::CreateVBOfromCurrentMesh(/* unsigned int &VBO_ID, unsigned int &numVertices,
//                                            unsigned int &indexBufferID, unsigned int &numIndices*/ )
//{
//	this->m_numberOfVertices = this->pScene->mMeshes[0]->mNumVertices;
//
//	this->m_pVertexData = new vert_XYZ_RGBA_N_STU_TanBi_4Bones[this->m_numberOfVertices];
//
//	for ( unsigned int vertexID = 0; vertexID != this->m_numberOfVertices; vertexID++ )
//	{
//		vert_XYZ_RGBA_N_STU_TanBi_4Bones* pCurVert = &(this->m_pVertexData[vertexID]);
//		aiVector3D* pCurAIVert = &(this->pScene->mMeshes[0]->mVertices[vertexID]);
//		
//		pCurVert->x = pCurAIVert->x;
//		pCurVert->y = pCurAIVert->y;
//		pCurVert->z = pCurAIVert->z;
//
//		// Bone IDs are being passed OK
//		pCurVert->boneID[0] = this->vecVertexBoneData[vertexID].ids[0];
//		pCurVert->boneID[1] = this->vecVertexBoneData[vertexID].ids[1];
//		pCurVert->boneID[2] = this->vecVertexBoneData[vertexID].ids[2];
//		pCurVert->boneID[3] = this->vecVertexBoneData[vertexID].ids[3];
//
//		// Weights are being passed OK
//		pCurVert->boneWeights[0] = this->vecVertexBoneData[vertexID].weights[0];
//		pCurVert->boneWeights[1] = this->vecVertexBoneData[vertexID].weights[1];
//		pCurVert->boneWeights[2] = this->vecVertexBoneData[vertexID].weights[2];
//		pCurVert->boneWeights[3] = this->vecVertexBoneData[vertexID].weights[3];
//
//		pCurVert->r = 1.0f;
//		pCurVert->g = 1.0f;
//		pCurVert->b = 1.0f;
//		pCurVert->a = 1.0f;
//
//
//	}//for ( unsigned int vertexID;
//
//	// Create a vertex buffer
//	glGenBuffers(1, &(this->m_VBO_ID) );  
//	// Brings the particular vertex buffer into context
//	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO_ID);
//
//	int vertexBufferSizeInBytes = sizeof( vert_XYZ_RGBA_N_STU_TanBi_4Bones ) * this->m_numberOfVertices;
//
//	glBufferData(GL_ARRAY_BUFFER, 
//					vertexBufferSizeInBytes, 
//					this->m_pVertexData,		// Pointer to vertex array
//					GL_STATIC_DRAW);
//
//	// Now the index buffer...
//	this->m_numberOfTriangles = this->pScene->mMeshes[0]->mNumFaces;
//	unsigned int numIndicesPerFace = this->pScene->mMeshes[0]->mFaces->mNumIndices;	// Will ALWAYS be 3 if we are using "triangulate"
//
//	std::vector< unsigned int > vecTriIndices;
//	this->m_numberOfIndices = this->m_numberOfTriangles * numIndicesPerFace; /*'always' 3*/
//	vecTriIndices.reserve( this->m_numberOfIndices );
//
//	for ( int faceIndex = 0; faceIndex != this->m_numberOfTriangles; faceIndex++ )
//	{	// Lines have 2 indices
//		aiFace* pCurAIFace = &(this->pScene->mMeshes[0]->mFaces[faceIndex]);
//
//		vecTriIndices.push_back( pCurAIFace->mIndices[0] );
//		vecTriIndices.push_back( pCurAIFace->mIndices[1] );
//		vecTriIndices.push_back( pCurAIFace->mIndices[2] );
//	}
//
//	glGenBuffers( 1, &(this->m_indexBuf_ID) );
//	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->m_indexBuf_ID );
//
//	unsigned int sizeOfIndexArrayInBytes = static_cast<unsigned int>(vecTriIndices.size()) * sizeof( GLuint );
//
//	unsigned int* pStartOfIndexBuffer = &(vecTriIndices[0]);
//	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pStartOfIndexBuffer, GL_STATIC_DRAW);
//
//	this->m_bVBO_created = true;
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//
//	return true;
//}
//
//bool cSimpleAssimpSkinnedMesh::CreateVBOandVOAfromCurrentMesh( int shaderProgID, std::string &error )
//{
//	this->m_shaderIDMatchingVAO = shaderProgID;
//
//	glUseProgram( this->m_shaderIDMatchingVAO );
//
//	glGenVertexArrays(1, &(this->m_VAO_ID) );	
//	glBindVertexArray( this->m_VAO_ID );
//	
//	// Associate a particular VBO to this VAO...
//	glBindBuffer( GL_ARRAY_BUFFER, this->m_VBO_ID );
//	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->m_indexBuf_ID );
//
//	unsigned int sizeOfVertexInBytes = sizeof(vert_XYZ_RGBA_N_STU_TanBi_4Bones);
//
//	{// vPosition
//		GLint vPosition_location = glGetAttribLocation(this->m_shaderIDMatchingVAO, "vPosition");	
//		if ( vPosition_location < 0 )	
//		{ error = "Error: can't find vPosition vertex attrib."; return false; }
//
//		glEnableVertexAttribArray(vPosition_location);
//		int offsetInBytesToPosition = offsetof( vert_XYZ_RGBA_N_STU_TanBi_4Bones, x );
//		glVertexAttribPointer( vPosition_location, 3, 
//								GL_FLOAT, GL_FALSE,
//								sizeOfVertexInBytes,	 
//								reinterpret_cast<void*>(static_cast<uintptr_t>(offsetInBytesToPosition)) );	// For 64 bit issues
//	}// vPosition
//
//	{// vColour
//		GLint vColour_location = glGetAttribLocation(this->m_shaderIDMatchingVAO, "vColour");	
//		if ( vColour_location < 0 )	
//		{ error = "Error: can't find vColour vertex attrib."; return false; }
//
//		glEnableVertexAttribArray(vColour_location);
//		int offsetInBytesToColour = offsetof( vert_XYZ_RGBA_N_STU_TanBi_4Bones, r );
//		glVertexAttribPointer( vColour_location, 4, GL_FLOAT, GL_FALSE,
//								sizeOfVertexInBytes,	
//								reinterpret_cast<void*>(static_cast<uintptr_t>(offsetInBytesToColour)) );	// For 64 bit issues
//	}// vColour
//
//	{// vNormal
//		GLint vNormal_location = glGetAttribLocation(this->m_shaderIDMatchingVAO, "vNormal");	
//		if ( vNormal_location < 0 )	
//		{ error = "Error: can't find vNormal vertex attrib."; return false; }
//
//		glEnableVertexAttribArray(vNormal_location);
// 		int offsetInBytesToNormal = offsetof( vert_XYZ_RGBA_N_STU_TanBi_4Bones, nX );
//		glVertexAttribPointer( vNormal_location, 3, GL_FLOAT, GL_TRUE,
//								sizeOfVertexInBytes,	 
//								reinterpret_cast<void*>(static_cast<uintptr_t>(offsetInBytesToNormal)) );		// For 64 bit issues
//	}// vNormal
//
//
//	{// vTexSTU
//		GLint vTexSTU_location = glGetAttribLocation(this->m_shaderIDMatchingVAO, "vTexSTU");	
//		if ( vTexSTU_location < 0 )	
//		{ error = "Error: can't find vTexSTU vertex attrib."; return false; }
//
//		glEnableVertexAttribArray(vTexSTU_location);
// 		int offsetInBytesToSTU = offsetof( vert_XYZ_RGBA_N_STU_TanBi_4Bones, s );
//		glVertexAttribPointer( vTexSTU_location, 3, GL_FLOAT, GL_TRUE,
//								sizeOfVertexInBytes,	
//								reinterpret_cast<void*>(static_cast<uintptr_t>(offsetInBytesToSTU)) );		// For 64 bit issues
//	}// vTexSTU
//
//	{// vTangent
//		GLint vTangent_location = glGetAttribLocation(this->m_shaderIDMatchingVAO, "vTangent");	
//		if ( vTangent_location < 0 )	
//		{ error = "Error: can't find vTangent vertex attrib."; return false; }
//
//		glEnableVertexAttribArray(vTangent_location);
// 		int offsetInBytesToTangent = offsetof( vert_XYZ_RGBA_N_STU_TanBi_4Bones, tanX );
//		glVertexAttribPointer( vTangent_location, 3, GL_FLOAT, GL_TRUE,
//								sizeOfVertexInBytes,	
//								reinterpret_cast<void*>(static_cast<uintptr_t>(offsetInBytesToTangent)) );		// For 64 bit issues
//	}// vTangent
//
//	{//vBitangent
//		GLint vBitangent_location = glGetAttribLocation(this->m_shaderIDMatchingVAO, "vBitangent");	
//		if ( vBitangent_location < 0 )	
//		{ error = "Error: can't find vBitangent vertex attrib."; return false; }
//
//		glEnableVertexAttribArray(vBitangent_location);
//		int offsetInBytesToBiTangent = offsetof( vert_XYZ_RGBA_N_STU_TanBi_4Bones, bitanX );
//		glVertexAttribPointer( vBitangent_location, 3, GL_FLOAT, GL_TRUE,
//								sizeOfVertexInBytes,	
//								reinterpret_cast<void*>(static_cast<uintptr_t>(offsetInBytesToBiTangent)) );		// For 64 bit issues
//	}//vBitangent
//
//	{//vBoneIDs_x4
//		GLint vBoneIDs_location = glGetAttribLocation(this->m_shaderIDMatchingVAO, "vBoneIDs_x4");	
//		if ( vBoneIDs_location < 0 )	
//		{ error = "Error: can't find vBoneIDs_x4 vertex attrib."; return false; }
//
//		glEnableVertexAttribArray(vBoneIDs_location);
//		int offsetInBytesToBoneIDs = (int)offsetof( vert_XYZ_RGBA_N_STU_TanBi_4Bones, boneID[0] );
//		//glVertexAttribPointer( vBoneIDs_location, 4, GL_INT, GL_FALSE,
//		glVertexAttribPointer( vBoneIDs_location, 4, GL_FLOAT, GL_FALSE,
//								sizeOfVertexInBytes,	
//								reinterpret_cast<void*>(static_cast<uintptr_t>(offsetInBytesToBoneIDs)) );		// For 64 bit issues
//	}//vBoneIDs_x4
//
//	{//vBoneWeights_x4
//		GLint vBoneWeights_location = glGetAttribLocation(this->m_shaderIDMatchingVAO, "vBoneWeights_x4");	
//		if ( vBoneWeights_location < 0 )	
//		{ error = "Error: can't find vBoneWeights_x4 vertex attrib."; return false; }
//
//		glEnableVertexAttribArray(vBoneWeights_location);
//		int offsetInBytesToBoneWeights = (int)offsetof( vert_XYZ_RGBA_N_STU_TanBi_4Bones, boneWeights[0] );
//		glVertexAttribPointer( vBoneWeights_location, 4, GL_FLOAT, GL_TRUE,
//								sizeOfVertexInBytes,	
//								reinterpret_cast<void*>(static_cast<uintptr_t>(offsetInBytesToBoneWeights)) );		// For 64 bit issues
//	}//vBoneWeights_x4
//
//	this->m_bVAO_created = true;
//
//	// Unbind everything
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glUseProgram(0);
//
//
//	return true;
//}
//
//

