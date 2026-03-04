//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "FontData.h"
#include "FontData.pb.h"

namespace Azul
{
	FontData::~FontData()
	{
		delete[] this->poFontEntry;
		this->poFontEntry = nullptr;
	}

	FontData::FontData()
		: pVersion{ 0 },
		numKeys{ 0 },
		poFontEntry{ nullptr }
	{
		strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
		assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);
	}

	FontData::FontData(unsigned int _numKeys)
		: pVersion{ 0 },
		numKeys{ _numKeys },
		poFontEntry{ nullptr }
	{
		strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
		assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);

		this->poFontEntry = new FontEntry[_numKeys]();
		assert(this->poFontEntry);
	}

	FontData& FontData::operator = (const FontData& r)
	{
		if (this != &r)
		{
			// Nuke orginal
			this->numKeys = 0;
			delete[] this->poFontEntry;

			// copy the new
			this->numKeys = r.numKeys;
			this->poFontEntry = new FontEntry[r.numKeys]();

			for (size_t i = 0; i < r.numKeys; i++)
			{
				this->poFontEntry[i] = r.poFontEntry[i];
			}
		}

		return *this;
	}

	void FontData::Serialize(FontData_proto& out) const
	{
		AZUL_UNUSED_VAR(out);

		std::string sVersion((const char*)this->pVersion, protoVersion::VERSION_NUM_BYTES);
		out.set_pversion(sVersion);

		// Add the number of bones
		out.set_numkeys(this->numKeys);

		// Add the SkelEntry
		FontEntry_proto tmpBoneEntry_proto;

		for (unsigned int i = 0; i < this->numKeys; i++)
		{
			// create one proto
			this->poFontEntry[i].Serialize(tmpBoneEntry_proto);

			// add it to the message(package)
			*out.add_pofontentry() = tmpBoneEntry_proto;
		}

		assert(out.numkeys() == this->numKeys);
	}

	void FontData::Deserialize(const FontData_proto& in)
	{
		AZUL_UNUSED_VAR(in);

		// CHECK the Version
		memcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, in.pversion().data(), protoVersion::VERSION_NUM_BYTES);
		assert(strcmp(this->pVersion, protoVersion::VERSION_STRING) == 0);

		// Safety
		delete[] this->poFontEntry;
		this->poFontEntry = nullptr;

		this->numKeys = in.numkeys();

		this->poFontEntry = new FontEntry[this->numKeys]();
		assert(this->poFontEntry);

		for (unsigned int i = 0; i < this->numKeys; i++)
		{
			poFontEntry[i].Deserialize(in.pofontentry((int)i));
		}
	}

	void FontData::Print(const char* const pName) const
	{
		AZUL_UNUSED_VAR(pName);
		Trace::out("%s: \n", pName);

		Trace::out("numKeys: %d\n", this->numKeys);

		for (size_t i = 0; i < this->numKeys; i++)
		{
			this->poFontEntry[i].Print(pName);
		}

		Trace::out("\n");
	}
}

// --- End of File ---

