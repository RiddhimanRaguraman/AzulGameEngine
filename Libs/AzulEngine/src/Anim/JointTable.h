#ifndef JOINT_TABLE_H
#define JOINT_TABLE_H
#include "EngineDLLInterface.h"

#include "DLink.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API JointTable : public DLink
	{
	public:
		enum Name
		{
			ChickenBot,
			Mousey,
			Halo,
			Ward,
			Crownboi,
			Maw,
			Pirate,
			Drax,
			Not_Initialized
		};

	public:
		JointTable();
		JointTable(const JointTable &) = delete;
		JointTable &operator = (const JointTable &) = delete;
		virtual ~JointTable();

		size_t	GetNumJoints();
		void SetJointTableName(Name JointTableName);

		void Set(Name JointTableName, size_t numJoints, unsigned int *pJointTable);


		virtual void Dump() override;
		virtual void Wash() override;


	public:
		Name         mName;
		size_t       numJoints;
		unsigned int *poJointTable;
	};
}

#endif

//--- End of File ---
