#include "AnimFrameBucket.h"

namespace Azul {

	AnimFrameBucket::~AnimFrameBucket()
	{
		delete[] this->poBone;
		this->poBone = nullptr;
	}

	AnimFrameBucket::AnimFrameBucket()
	{
		this->keyFrame = 0;
		this->numNodes = 0;
		this->keyTime = 0;
		this->keyTimeIndex = 0;

		this->poBone = nullptr;
	}

	AnimFrameBucket::AnimFrameBucket(unsigned int _numBones)
	{
		this->keyFrame = 0;
		this->numNodes = _numBones;
		this->keyTime = 0.0f;
		this->keyTimeIndex = 0;

		this->poBone = new AnimBone[_numBones]();
		assert(this->poBone);
	}

	AnimFrameBucket& AnimFrameBucket::operator = (const AnimFrameBucket& r)
	{
		if (this != &r)
		{
			// Nuke orginal
			this->keyFrame = 0;
			this->numNodes = 0;
			this->keyTime = 0.0f;
			this->keyTimeIndex = 0;
			delete[] this->poBone;

			// copy the new
			this->keyFrame = r.keyFrame;
			this->numNodes = r.numNodes;
			this->keyTime = r.keyTime;
			this->keyTimeIndex = r.keyTimeIndex;
			this->poBone = new AnimBone[r.numNodes]();

			for (size_t i = 0; i < r.numNodes; i++)
			{
				this->poBone[i] = r.poBone[i];
			}
		}

		return *this;
	}

	void AnimFrameBucket::Serialize(AnimFrameBucket_proto& out) const
	{
		AZUL_UNUSED_VAR(out);

		// Add the number of bones
		out.set_numnodes(this->numNodes);
		out.set_keyframe(this->keyFrame);
		out.set_keytime(this->keyTime);
		out.set_keytimeindex(this->keyTimeIndex);

		AnimBone_proto tmpBoneEntry_proto;

		for (unsigned int i = 0; i < this->numNodes; i++)
		{
			// create one proto
			this->poBone[i].Serialize(tmpBoneEntry_proto);

			// add it to the message(package)
			*out.add_pobone() = tmpBoneEntry_proto;
		}

		assert(out.numnodes() == this->numNodes);
	}

	void AnimFrameBucket::Deserialize(const AnimFrameBucket_proto& in)
	{
		AZUL_UNUSED_VAR(in);
		// Safety
		delete[] this->poBone;
		this->poBone = nullptr;

		this->keyFrame = in.keyframe();
		this->numNodes = in.numnodes();
		this->keyTime = in.keytime();
		this->keyTimeIndex = in.keytimeindex();

		this->poBone = new AnimBone[this->numNodes]();
		assert(this->poBone);

		for (unsigned int i = 0; i < this->numNodes; i++)
		{
			poBone[i].Deserialize(in.pobone((int)i));
		}
	}

	void AnimFrameBucket::Print(const char* const pName) const
	{
		AZUL_UNUSED_VAR(pName);
		Trace::out("%s: \n", pName);

		Trace::out("     keyFrame: %d\n", this->keyFrame);
		Trace::out("     numNodes: %d\n", this->numNodes);
		Trace::out("      keyTime: %f\n", this->keyTime);
		Trace::out(" keyTimeIndex: %d\n", this->keyTimeIndex);

		for (size_t i = 0; i < this->numNodes; i++)
		{
			this->poBone[i].Print(pName);
		}

		Trace::out("\n");
	}
}