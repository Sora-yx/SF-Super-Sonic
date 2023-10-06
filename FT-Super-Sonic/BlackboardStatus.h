#pragma once

namespace app::player
{
	class alignas(0x10) BlackboardStatus : public BlackboardContent
	{
		inline static const char* ms_pBlackboardName = "BlackboardStatus";

	public:
		INSERT_PADDING(0x4);
		bool isSuper;
		INSERT_PADDING(39); //44 bytes here
		int64_t StateFlags;
		INSERT_PADDING(8);
		int64_t WorldFlags; //60 bytes here
		INSERT_PADDING(0xF0); //300
		bool IsIslandSideView; //301
		INSERT_PADDING(83);

		size_t GetNameHash() override
		{
			return csl::ut::StringMapOperation::hash(ms_pBlackboardName);
		}

		static const char* GetBlackboardName()
		{
			return ms_pBlackboardName;
		}
	};
}