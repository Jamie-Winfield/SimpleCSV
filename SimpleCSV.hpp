/// author Jamie Winfield
/// created 17/10/2022

#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <chrono>


class CSVParser
{
public:

	/// <summary>
	/// Loads all of the specified csv file contents into memory, if a csv has
	///  already been loaded it will be overwritten if another readable filename is provided
	/// </summary>
	/// <param name="_filename"> the name of the file on disk to load</param>
	/// <returns> returns true if successful </returns>
	bool LoadCsv(const char* _filename)
	{
		std::string tempFileName = _filename;
		std::ifstream file{ _filename };
		std::string extension;
		extension.push_back(tempFileName[tempFileName.size() - 4]);
		extension.push_back(tempFileName[tempFileName.size() - 3]);
		extension.push_back(tempFileName[tempFileName.size() - 2]);
		extension.push_back(tempFileName[tempFileName.size() - 1]);
		if (extension != ".csv")
		{
			return false;
		}
		if (!file.is_open())
		{
			return false;
		}
		filename = _filename;
		csvData.clear();
		columnHeaders.clear();
		length = 0;
		width = 0;
		std::vector<std::string> tempColumnHeaders;
		for (std::string line; std::getline(file, line);)
		{
			std::istringstream ss(std::move(line));
			if (length == 0)
			{
				for (std::string value; std::getline(ss, value, ',');)
				{
					tempColumnHeaders.push_back(value);
					width++;
				}
				for (auto _header : tempColumnHeaders)
				{
					columnHeaders.push_back(_header);
				}
			}
			else
			{
				for (std::string value; std::getline(ss, value, ',');)
				{
					csvData.push_back(value);
				}
			}
			length++;
		}
		file.close();
		return true;

	}
	
	/// <summary>
	/// Returns the width of each line in the csv file
	/// </summary>
	/// <returns> size_t line width </returns>
	[[nodiscard]] size_t GetFileWidth()
	{
		return width;
	}
	/// <summary>
	/// Returns the total length in lines of the csv file excluding the column titles
	/// </summary>
	[[nodiscard]] size_t GetFileLength()
	{
		return length;
	}
	/// <summary>
	/// Returns the name of the file currently loaded by the parser
	/// </summary>
	[[nodiscard]] std::string GetFileName()
	{
		return filename;
	}
	/// <summary>
	/// Returns a vector containing all of the csv data which was collected from the file
	/// excluding the column titles
	/// </summary>
	[[nodiscard]] std::vector<std::string> GetAllData()
	{
		return csvData;
	}
	/// <summary>
	/// Returns a vector containing the column headers of the csv file
	/// </summary>
	[[nodiscard]] std::vector<std::string> GetColumnHeaders()
	{
		return columnHeaders;
	}
	/// <summary>
	/// Returns a vector holding all the data held in the specified line of the csv file
	/// </summary>
	/// <param name="_lineNo"> line number to get data starting at 0 
	/// and excluding the column headers line </param>
	/// <returns> returns empty vector if unsuccessful </returns> 
	[[nodiscard]] std::vector<std::string> GetLine(size_t _lineNo)
	{
		if (_lineNo > length)
		{
			return std::vector<std::string>();
		}
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		std::vector<std::string> lineData;
		for (size_t i = 0; i < width; ++i)
		{
			lineData.push_back(csvData[_lineNo * width + i]);
		}
		return lineData;
	}
	/// <summary>
	/// Returns a vector holding all of the line data from each of the
	///  specified lines in the csv file
	/// </summary>
	/// <param name="_lines"> a vector holding the line numbers to get data starting at 0 
	/// and excluding the column headers line</param>
	/// <returns> returns empty vector if unsuccessful </returns> 
	[[nodiscard]] std::vector<std::string> GetLines(std::vector<size_t> _lines)
	{
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		std::vector<std::string> lineData;
		for (auto& lineNo : _lines)
		{
			if (lineNo > length)
			{
				return std::vector<std::string>();
			}
			for (size_t i = 0; i < width; ++i)
			{
				lineData.push_back(csvData[lineNo * width + i]);
			}
		}
		return lineData;
	}
	/// <summary>
	/// Returns a vector holding all of the line data from the specified 
	/// starting line to the end line inclusive;
	/// </summary>
	/// <param name="_startLine"> line number to get data starting at 0 
	/// and excluding the column headers line</param>
	/// <param name="_endLine"> line number to get data starting at 0 
	/// and excluding the column headers line</param>
	/// <returns> returns empty vector if unsuccessful </returns> 
	[[nodiscard]] std::vector<std::string> GetLines(size_t _startLine, size_t _endLine)
	{
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		if (_startLine > _endLine)
		{
			size_t temp = _endLine;
			_endLine = _startLine;
			_startLine = _endLine;
		}
		std::vector<std::string> lineData;
		for (size_t lineNo = _startLine; lineNo < _endLine; ++lineNo)
		{
			if (lineNo > length)
			{
				return std::vector<std::string>();
			}
			for (size_t i = 0; i < width; ++i)
			{
				lineData.push_back(csvData[lineNo * width + i]);
			}
		}
		return lineData;

	}
	/// <summary>
	/// Returns a vector holding all of the data from a column in the entire csv file
	/// </summary>
	/// <param name="_header"> title of the column header</param>
	/// <returns> returns empty vector if unsuccessful </returns>
	[[nodiscard]] std::vector<std::string> GetColumn(const char* _header)
	{
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		size_t column = GetColumnFromHeader(_header);
		if (column > width)
		{
			return std::vector<std::string>();
		}
		std::vector<std::string> columnData;
		for (size_t i = 0; i < length; ++i)
		{
			columnData.push_back(csvData[column + i * width]);
		}
		return columnData;
	}
	/// <summary>
	/// Returns a vector holding all of the data from a column starting at 0 
	/// and ending at the specified max line inclusive in the csv file
	/// </summary>
	/// <param name="_header"> title of the column header</param>
	/// <param name="_maxLine"> max line to get data from</param>
	/// <returns> returns empty vector if unsuccessful </returns>
	[[nodiscard]] std::vector<std::string> GetColumn(const char* _header, size_t _maxLine)
	{
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		size_t column = GetColumnFromHeader(_header);
		if (column > width)
		{
			return std::vector<std::string>();
		}
		std::vector<std::string> columnData;
		if (_maxLine > length)
		{
			_maxLine = length;
		}
		for (size_t i = 0; i < _maxLine; ++i)
		{
			columnData.push_back(csvData[column + i * width]);
		}
		return columnData;
	}
	/// <summary>
	/// Returns a vector holding all of the data from the specified column starting
	/// from the specified starting line to the end line inclusive 
	/// </summary>
	/// <param name="_header"> title of the column header </param>
	/// <param name="_startLine"> first line to get data from </param>
	/// <param name="_endLine"> final line to get data from </param>
	/// <returns> returns empty vector if unsuccessful </returns>
	[[nodiscard]] std::vector<std::string> GetColumn(const char* _header, size_t _startLine, size_t _endLine)
	{
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		size_t column = GetColumnFromHeader(_header);
		if (column > width)
		{
			return std::vector<std::string>();
		}
		std::vector<std::string> columnData;
		if (_startLine > _endLine)
		{
			size_t temp = _endLine;
			_endLine = _startLine;
			_startLine = _endLine;
		}
		if (_endLine > length)
		{
			_endLine = length;
		}
		for (size_t i = _startLine; i < _endLine; ++i)
		{
			columnData.push_back(csvData[column + i * width]);
		}
		return columnData;
	}
	/// <summary>
	/// Returns a vector holding all of the data from a column in the entire csv file
	/// </summary>
	/// <param name="_column"> index of the column header starting at 0</param>
	/// <returns> returns empty vector if unsuccessful </returns>
	[[nodiscard]] std::vector<std::string> GetColumn(size_t _column)
	{
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		if (_column > width)
		{
			return std::vector<std::string>();
		}
		std::vector<std::string> columnData;
		for (size_t i = 0; i < length; ++i)
		{
			columnData.push_back(csvData[_column + i * width]);
		}
		return columnData;
	}
	/// <summary>
	/// Returns a vector holding all of the data from a column starting at 0 
	/// and ending at the specified max line inclusive in the csv file
	/// </summary>
	/// <param name="_column"> Index of the column starting at 0</param>
	/// <param name="_maxLine"> max line to get data from</param>
	/// <returns> returns empty vector if unsuccessful </returns>
	[[nodiscard]] std::vector<std::string> GetColumn(size_t _column, size_t _maxLine)
	{
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		if (_column > width)
		{
			return std::vector<std::string>();
		}
		std::vector<std::string> columnData;
		if (_maxLine > length)
		{
			_maxLine = length;
		}
		for (size_t i = 0; i < _maxLine; ++i)
		{
			columnData.push_back(csvData[_column + i * width]);
		}
		return columnData;
	}
	/// <summary>
	/// Returns a vector holding all of the data from the specified column starting
	/// from the specified starting line to the end line inclusive 
	/// </summary>
	/// <param name="_column"> Index of the column starting at 0 </param>
	/// <param name="_startLine"> first line to get data from </param>
	/// <param name="_endLine"> final line to get data from </param>
	/// <returns> returns empty vector if unsuccessful </returns>
	[[nodiscard]] std::vector<std::string> GetColumn(size_t _column, size_t _startLine, size_t _endLine)
	{
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		if (_column > width)
		{
			return std::vector<std::string>();
		}
		std::vector<std::string> columnData;
		if (_startLine > _endLine)
		{
			size_t temp = _endLine;
			_endLine = _startLine;
			_startLine = _endLine;
		}
		if (_endLine > length)
		{
			_endLine = length;
		}
		for (size_t i = _startLine; i < _endLine; ++i)
		{
			columnData.push_back(csvData[_column + i * width]);
		}
		return columnData;
	}
	/// <summary>
	/// Returns a vector holding all of the data from the specified column from
	/// the specified lines
	/// </summary>
	/// <param name="_column"> Index of the column starting at 0 </param>
	/// <param name="_lines">a vector holding the line numbers to get data starting at 0 
	/// and excluding the column headers line </param>
	/// <returns></returns>
	[[nodiscard]] std::vector<std::string> GetColumn(size_t _column, std::vector<size_t> _lines)
	{
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		if (_column > width)
		{
			return std::vector<std::string>();
		}
		std::vector<std::string> columnData;
		for (auto& line : _lines)
		{
			if (line > length)
			{
				return std::vector<std::string>();
			}
			columnData.push_back(csvData[_column + line * width]);
		}
		return columnData;
	}
	/// <summary>
	/// Returns a vector holding all of the data from the specified column from
	/// the specified lines
	/// </summary>
	/// <param name="_header"> title of the column header </param>
	/// <param name="_lines">a vector holding the line numbers to get data starting at 0 
	/// and excluding the column headers line </param>
	/// <returns></returns>
	[[nodiscard]] std::vector<std::string> GetColumn(const char* _header, std::vector<size_t> _lines)
	{
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		size_t column = GetColumnFromHeader(_header);
		if (column > width)
		{
			return std::vector<std::string>();
		}
		if (column > width)
		{
			return std::vector<std::string>();
		}
		std::vector<std::string> columnData;
		for (auto& line : _lines)
		{
			if (line > length)
			{
				return std::vector<std::string>();
			}
			columnData.push_back(csvData[column + line * width]);
		}
		return columnData;
	}
	/// <summary>
	/// Gets a column index from its title
	/// </summary>
	/// <param name="_header"> title of the column header</param>
	/// <returns> returns a size larger than the 
	/// actual width of the file if unsuccessful </returns>
	[[nodiscard]] size_t GetColumnFromHeader(const char* _header)
	{
		auto pos = std::find(columnHeaders.begin(), columnHeaders.end(), _header);
		if (pos == columnHeaders.end())
		{
			return width + 1;
		}
		return std::distance(columnHeaders.begin(), pos);
	}
	/// <summary>
	/// Returns the total sum of a column in the file as a float
	/// </summary>
	/// <param name="_header">title of the column header</param>
	/// <returns> returns NAN if unsuccessful</returns>
	[[nodiscard]] float SumColumnf(const char* _header)
	{
		if (csvData.empty())
		{
			return NAN;
		}
		size_t column = GetColumnFromHeader(_header);
		if (column > width)
		{
			return NAN;
		}
		std::vector<float> columnData;
		for (size_t i = 0; i < length; ++i)
		{

			
			auto _dataStr = csvData[column + i * width];
			if (IsANumber(_dataStr))
			{
				columnData.push_back(std::stof(_dataStr));
			}
		}
		return std::accumulate(columnData.begin(), columnData.end(), 0);
	}
	/// <summary>
	/// Returns the total sum of a column in the file as a float
	/// </summary>
	/// <param name="_header">index of the column starting at 0</param>
	/// <returns> returns NAN if unsuccessful</returns>
	[[nodiscard]] float SumColumnf(size_t _column)
	{
		if (csvData.empty())
		{
			return NAN;
		}
		if (_column > width)
		{
			return NAN;
		}
		std::vector<float> columnData;
		for (size_t i = 0; i < length; ++i)
		{
			
			auto _dataStr = csvData[_column + i * width];
			if (IsANumber(_dataStr))
			{
				columnData.push_back(std::stof(_dataStr));
			}
		}
		return std::accumulate(columnData.begin(), columnData.end(), 0);
	}
	/// <summary>
	/// Checks if provided string is a number
	/// </summary>
	/// <param name="_str"> String to be tested</param>
	/// <returns>returns true if successful</returns>
	[[nodiscard]] bool IsANumber(std::string _str )
	{
		for (auto& _char : _str)
		{
			if (!std::isdigit(_char))
			{
				if (_char != '.' || _str.size() <= 1)
				{
					return false;
				}
			}
		}
		return true;
	}
	/// <summary>
	/// Returns a vector containing all of the line data from
	///  the lines which contain the provided search value
	/// </summary>
	/// <param name="_value"> Value to be searched</param>
	/// <returns> Returns an empty vector if unsuccessful</returns>
	[[nodiscard]] std::vector<std::string> FindLines(std::string _value)
	{
		if (csvData.empty())
		{
			return std::vector<std::string>();
		}
		std::vector<std::string> lines;
		for (size_t i = 0; i < length; ++i)
		{
			auto checkLine = GetLine(i);
			if (std::find(checkLine.begin(), checkLine.end(), _value) != checkLine.end())
			{
				for (auto& _char : checkLine)
				{
					lines.push_back(_char);
				}
			}
		}
		return lines;
	}
	/// <summary>
	/// Returns the value at the provided point in the file
	/// </summary>
	/// <param name="_header">Title of the header of the column</param>
	/// <param name="_line">The line number of the value</param>
	/// <returns> Returns NAN if unsuccessful </returns>
	[[nodiscard]] float GetValue(const char* _header, size_t _line)
	{
		if (csvData.empty())
		{
			return NAN;
		}
		size_t column = GetColumnFromHeader(_header);
		if (column > width)
		{
			return NAN;
		}
		if (_line > length)
		{
			return NAN;
		}
		auto value =csvData[_line * width + column];
		if (IsANumber(value))
		{
			return std::stof(value);
		}
		return NAN;
	}

	/// <summary>
	/// Returns the value as a string at the provided point in the file
	/// </summary>
	/// <param name="_header">Title of the header of the column</param>
	/// <param name="_line">The line number of the value</param>
	/// <returns> returns a string </returns>
	[[nodiscard]] std::string GetValueString(const char* _header, size_t _line)
	{
		if (csvData.empty())
		{
			return "";
		}
		size_t column = GetColumnFromHeader(_header);
		if (column > width)
		{
			return "";
		}
		if (_line > length)
		{
			return "";
		}
		return csvData[_line * width + column];
		
	}

private:
	
	
	std::vector<std::string> csvData;
	std::vector<std::string> columnHeaders;
	std::vector<std::unique_ptr<std::vector<std::string>>> tempData;
	std::string filename;
	size_t width = 0;
	size_t length = 0;
};
