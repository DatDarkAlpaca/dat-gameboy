#include "pch.hpp"
#include "application.hpp"

int main(int argc, char** argv)
{
	dat::initialize_logger();
	dat::initialize_platform();	

	dat::s_DatApplication application;
	application.run();
}