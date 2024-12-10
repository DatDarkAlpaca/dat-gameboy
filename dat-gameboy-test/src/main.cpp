#undef DAT_ENABLE_DEBUG_LOG
#undef DAT_ENABLE_WARNINGS

#include "pch.hpp"
#include "tests.hpp"

int main(int argc, char** argv)
{
	dat::initialize_logger();
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}