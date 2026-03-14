//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "StringThis.h"

namespace Azul
{
	StringThis::StringThis(HierarchyTable::Name status)
	{
		switch (status)
		{
		case HierarchyTable::Name::ChickenBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(HierarchyTable::ChickenBot));
			break;

		case HierarchyTable::Name::Not_Initialized:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(HierarchyTable::Not_Initialized));
			break;

		default:
			assert(false);
		}
	}

	StringThis::StringThis(JointTable::Name status)
	{
		switch (status)
		{
		case JointTable::Name::ChickenBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(JointTable::ChickenBot));
			break;

		case JointTable::Name::Not_Initialized:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(JointTable::Not_Initialized));
			break;

		default:
			assert(false);
		}
	}
	StringThis::StringThis(Clip::Name status)
	{
		switch (status)
		{
		case Clip::Name::Walk_ChickenBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::Walk_ChickenBot));
			break;
		case Clip::Name::Run_ChickenBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::Run_ChickenBot));
			break;
		case Clip::Name::HitBack_ChickenBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::HitBack_ChickenBot));
			break;
		case Clip::Name::ShotUp_ChickenBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::ShotUp_ChickenBot));
			break;
		case Clip::Name::Idle_ChickenBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::Idle_ChickenBot));
			break;
		case Clip::Name::Walk_DogBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::Walk_DogBot));
			break;
		case Clip::Name::Run_DogBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::Run_DogBot));
			break;
		case Clip::Name::HitBack_DogBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::HitBack_DogBot));
			break;
		case Clip::Name::ShotUp_DogBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::ShotUp_DogBot));
			break;
		case Clip::Name::Idle_DogBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::Idle_DogBot));
			break;

		case Clip::Name::Not_Initialized:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::Not_Initialized));
			break;

		default:
			assert(false);
		}
	}

	StringThis::StringThis(Skel::Name status)
	{
		switch (status)
		{
		case Skel::Name::ChickenBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Skel::ChickenBot));
			break;
		case Skel::Name::DogBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Skel::DogBot));
			break;

		case Skel::Name::Not_Initialized:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Skel::Not_Initialized));
			break;

		default:
			assert(false);
		}
	}

	StringThis::StringThis(Mesh::Name status)
	{
		switch(status)
		{
			case Mesh::Name::CUBE:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::CUBE));
				break;

			case Mesh::Name::PYRAMID:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::PYRAMID));
				break;

			case Mesh::Name::NOT_INITIALIZED:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::NOT_INITIALIZED));
				break;

			case Mesh::Name::NULL_MESH:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::NULL_MESH));
				break;

			default:
				assert(false);
		}
	}

	StringThis::StringThis(Camera::Name status)
	{
		switch(status)
		{
			case Camera::Name::CAMERA_0:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::CAMERA_0));
				break;

			case Camera::Name::CAMERA_1:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::CAMERA_1));
				break;

			case Camera::Name::NOT_INITIALIZED:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::NOT_INITIALIZED));
				break;

			case Camera::Name::NullCamera:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::NullCamera));
				break;

			default:
				assert(false);
		}
	}

	StringThis::StringThis(ShaderObject::Name status)
	{
		switch(status)
		{
			case ShaderObject::Name::NullShader:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::NullShader));
				break;

			case ShaderObject::Name::ColorByVertex:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::ColorByVertex));
				break;

			case ShaderObject::Name::FlatTexture:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::FlatTexture));
				break;

			case ShaderObject::Name::LightTexture:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::LightTexture));
				break;

			case ShaderObject::Name::BasicCompute:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::BasicCompute));
				break;

			case ShaderObject::Name::SkinFlatTexture:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::SkinFlatTexture));
				break;

			case ShaderObject::Name::SkinLightTexture:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::SkinLightTexture));
				break;

			case ShaderObject::Name::MixerCompute:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::MixerCompute));
				break;

			case ShaderObject::Name::ConstColor:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::ConstColor));
				break;

			case ShaderObject::Name::Sprite:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::Sprite));
				break;

			case ShaderObject::Name::Uninitialized:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::Uninitialized));
				break;

			default:
				assert(false);
		}
	}

	StringThis::StringThis(TextureObject::Name status)
	{
		switch(status)
		{
			case TextureObject::Name::NullTexture:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::NullShader));
				break;

			case TextureObject::Name::Duckweed:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::Duckweed));
				break;

			case TextureObject::Name::Uninitialized:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::Uninitialized));
				break;

			default:
				assert(false);
		}
	}

	StringThis::StringThis(AnimMan::Name status)
	{
		switch(status)
		{
		case AnimMan::Name::Walk:
			strcpy_s(this->buffer, BUFFER_SIZE, "Walk");
			break;
		case AnimMan::Name::Run:
			strcpy_s(this->buffer, BUFFER_SIZE, "Run");
			break;
		case AnimMan::Name::HitBack:
			strcpy_s(this->buffer, BUFFER_SIZE, "HitBack");
			break;
		case AnimMan::Name::Shotup:
			strcpy_s(this->buffer, BUFFER_SIZE, "Shotup");
			break;
		case AnimMan::Name::Idle:
			strcpy_s(this->buffer, BUFFER_SIZE, "Idle");
			break;
		case AnimMan::Name::Uninitialized:
			strcpy_s(this->buffer, BUFFER_SIZE, "Uninitialized");
			break;
		default:
			assert(false);
		}
	}


	StringThis::operator char *()
	{
		return this->buffer;
	}

}

// --- End of File ---
