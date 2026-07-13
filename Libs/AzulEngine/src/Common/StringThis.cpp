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

		case HierarchyTable::Name::DogBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(HierarchyTable::DogBot));
			break;

		case HierarchyTable::Name::SpiderBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(HierarchyTable::SpiderBot));
			break;

		case HierarchyTable::Name::Mousey:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(HierarchyTable::Mousey));
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

		case JointTable::Name::Mousey:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(JointTable::Mousey));
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

		case Clip::Name::walk_Spiderbot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::walk_Spiderbot));
			break;

		case Clip::Name::Mousey_Silly_Dance:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::Mousey_Silly_Dance));
			break;

		case Clip::Name::Mousey_Run:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::Mousey_Run));
			break;

		case Clip::Name::Mousey_Gangnam:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Clip::Mousey_Gangnam));
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

		case Skel::Name::SpiderBot:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Skel::SpiderBot));
			break;

		case Skel::Name::Mousey:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Skel::Mousey));
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

			case Mesh::Name::CUBE_COLOR:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::CUBE_COLOR));
				break;

			case Mesh::Name::PYRAMID:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::PYRAMID));
				break;

			case Mesh::Name::CRATE:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::CRATE));
				break;

			case Mesh::Name::SPHERE:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::SPHERE));
				break;

			case Mesh::Name::SPRITE:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::SPRITE));
				break;

			case Mesh::Name::SPACE_FRIGATE:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::SPACE_FRIGATE));
				break;

			case Mesh::Name::R2D2:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::R2D2));
				break;

			case Mesh::Name::BONE_A:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::BONE_A));
				break;

			case Mesh::Name::BONE_B:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::BONE_B));
				break;

			case Mesh::Name::ChickenBot:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::ChickenBot));
				break;

			case Mesh::Name::Mousey:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::Mousey));
				break;

			case Mesh::Name::Halo0:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::Halo0));
				break;

			case Mesh::Name::Halo1:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::Halo1));
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

			case ShaderObject::Name::SkinFlatTexture:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::SkinFlatTexture));
				break;

			case ShaderObject::Name::SkinLightTexture:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::SkinLightTexture));
				break;

			case ShaderObject::Name::MixerACompute:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::MixerACompute));
				break;

			case ShaderObject::Name::MixerBCompute:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::MixerBCompute));
				break; 

			case ShaderObject::Name::MixerCCompute:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::MixerCCompute));
				break;

			case ShaderObject::Name::ConstColor:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::ConstColor));
				break;

			case ShaderObject::Name::ConstColorLight:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::ConstColorLight));
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
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::NullTexture));
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

	// AnimMan::Name -> string moved to AnimMan::NameToString (app-side);
	// AnimMan is gameplay glue and does not move into AzulEngine.


	StringThis::operator char *()
	{
		return this->buffer;
	}

}

// --- End of File ---
