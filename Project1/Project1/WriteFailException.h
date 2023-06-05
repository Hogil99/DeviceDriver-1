#pragma once
#include <iostream>

class WriteFailException : public std::exception
{
	const char* what() const throw()
	{
		return "WriteFailException occurred";
	}
};