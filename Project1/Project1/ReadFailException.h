#pragma once
#include <iostream>

class ReadFailException : public std::exception
{
	const char * what() const throw()
	{
		return "ReadFailException occurred";
	}
};