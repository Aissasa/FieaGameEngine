#pragma once

namespace UnitTestLibraryDesktop
{
	class Bar
	{
	public:

		Bar(std::float_t fl = 0);
		~Bar() = default;
		Bar(const Bar & rhs) = default;
		Bar(Bar && rhs) = default;
		Bar& operator=(const Bar& rhs) = default;
		Bar& operator=(Bar&& rhs) = default;

		std::float_t GetFloat() const;
		void SetFloat(std::float_t fl);

	private:

		std::float_t mFloat;
	};
}