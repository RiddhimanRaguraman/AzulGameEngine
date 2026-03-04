#include "AnimClip.h"

namespace Azul
{
	AnimClip::~AnimClip()
	{
		delete[] this->poFrameBucketEntry;
		this->poFrameBucketEntry = nullptr;
	}

	AnimClip::AnimClip()
		: pVersion{ 0 },
		numKeyFrames{ 0 },
		numBones{ 0 },
		poFrameBucketEntry{ nullptr }
	{
		strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
		assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);
	}

	AnimClip::AnimClip(unsigned int _numKeyFrames, unsigned int _numBones)
		: pVersion{ 0 },
		numKeyFrames{ _numKeyFrames },
		numBones{ _numBones },
		poFrameBucketEntry{ nullptr }
	{
		strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
		assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);

		this->poFrameBucketEntry = new AnimFrameBucket[_numKeyFrames]();

		for (size_t i = 0; i < _numKeyFrames; i++)
		{
			this->poFrameBucketEntry[i] = AnimFrameBucket(_numBones);
		}

		assert(this->poFrameBucketEntry);
	}

	void AnimClip::Serialize(AnimClip_proto& out) const
	{
		AZUL_UNUSED_VAR(out);
		
		std::string sVersion((const char*)this->pVersion, protoVersion::VERSION_NUM_BYTES);
		out.set_pversion(sVersion);

		// Add the number of bones
		out.set_numkeyframes(this->numKeyFrames);
		out.set_numbones(this->numBones);

		for (unsigned int i = 0; i < this->numKeyFrames; i++)
		{
			// Add the FrameBucketEntry
			// Crazy - issue... 
			//     this needs to be unique for the add
			//     Cannot pull out of the loop   
			AnimFrameBucket_proto tmpFrameBucketEntry_proto;

			// create one proto
			this->poFrameBucketEntry[i].Serialize(tmpFrameBucketEntry_proto);

			// add it to the message(package)
			*out.add_poframebucketentry() = tmpFrameBucketEntry_proto;
		}

		assert(out.numkeyframes() == this->numKeyFrames);
	}

	void AnimClip::Deserialize(const AnimClip_proto& in)
	{
		AZUL_UNUSED_VAR(in);
		// CHECK the Version
		memcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, in.pversion().data(), protoVersion::VERSION_NUM_BYTES);
		assert(strcmp(this->pVersion, protoVersion::VERSION_STRING) == 0);

		// Safety
		delete[] this->poFrameBucketEntry;
		this->poFrameBucketEntry = nullptr;

		this->numKeyFrames = in.numkeyframes();
		this->numBones = in.numbones();

		// Create the EMPTY space...
		//    High-level only
		//       the serializer will create the bone array dynamically
		this->poFrameBucketEntry = new AnimFrameBucket[this->numKeyFrames]();
		assert(this->poFrameBucketEntry);

		// Fill it
		for (unsigned int i = 0; i < this->numKeyFrames; i++)
		{
			poFrameBucketEntry[i].Deserialize(in.poframebucketentry((int)i));
		}
	}

	void AnimClip::Print(const char* const pName) const
	{
		AZUL_UNUSED_VAR(pName);

		Trace::out("%s: \n", pName);
		Trace::out("numkeyframe: %d\n", this->numKeyFrames);

		for (size_t i = 0; i < this->numKeyFrames; i++)
		{
			char pKeyName[32];
			sprintf_s(pKeyName, 32, "Key_%d", (int)this->poFrameBucketEntry[i].keyFrame);
			this->poFrameBucketEntry[i].Print(pKeyName);
		}

		Trace::out("\n");
	}
}
