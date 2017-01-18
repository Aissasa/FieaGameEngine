#pragma once

namespace FieaGameEngine
{
	class SList
	{
	public:
		SList(std::int32_t data) { mData = data; }

		std::int32_t Data();

	private:

		std::int32_t mData;
	};
}