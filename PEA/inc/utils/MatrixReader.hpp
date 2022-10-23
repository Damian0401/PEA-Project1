#pragma once

#include "../structures/AdjacencyMatrix.hpp"

#include <string>

namespace PEA
{
	class MatrixReader
	{
	public:
		MatrixReader();
		MatrixReader(std::string basePath);
		AdjanencyMatrix read(std::string fileName, bool useBasePath = true);

	private:
		std::string _basePath;
	};
}
