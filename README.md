# SimpleCSV

A single header CSV reader written for C++.

# Examples

To load a CSV file into the reader you simply pass the path to the file to the reader, this must be a file ending in the .csv extension.

```C++
SimpleCSV reader = SimpleCSV();
reader.LoadCsv("/mycsv.csv");
```

The reader will return true if it manages to correctly load the contents of the file. The loading time is dependant on the size of the file being read.

The entire contents of the csv can be requested from the reader and returned in a std::vector<std::string> format.
```C++
std::vector<std::string> csv_data = reader.GetAllData();
```

Alternitively single lines of the csv can be requested by line number or multiple line numbers

```C++
std::vector<std::string> single_line = reader.GetLine(0);

// For multiple specific lines a vector on the line numbers can be passed
std::vector<size_t> lines = {0,15,42};
std::vector<std::string> multiple_lines = reader.GetLines(lines);

// Also a start line and an end line can be proved to get all the lines between these values including the start and end lines
std::vector<std::string> between_lines = reader.GetLines(0,15);
```

Specific Columns can be requested using the header of the column which is wanted or the value of the column
```C++
std::vector<std::string> column_values = reader.GetColumn("first header");

std::vector<std::string> column_values_by_id = reader.GetColumn(0);

// a max line to get data to can be provided if only some data is needed
std::vector<std::string> column_values_to_line = reader.GetColumn("first header", 15);

//similar to getting lines column data between two lines can also be requested
std::vector<std::string> column_values_between_lines = reader.GetColumn("first header",0,15);

std::vector<size_t> lines = {0,15,42};
std::vector<std::string> column_values_specific_lines = reader.GetColumn("first header",lines);
```

Rows containing a specific value can be searched and returned using the reader aswell
```C++
std::Vector<std::string> lines = reader.FindLines("Tuesday");

// it is important to remember that all data in the csv is stored as strings so numbers should be searched with this in mind
std::Vector<std::string> lines = reader.FindLines("42");
```

If the values in a specific column of the file are numbers then the sum of the column can be returned by the reader
```C++
float sum = reader.SumColumnf("first header");

// this can also be sumed using the index of the column
float sum_by_index = reader.SumColumnf(0);
```

Finally the value at a specific point in the file using the line number and column
```C++
// for known integers and floats
float value = reader.GetValue("first header",42);

// for strings
std::string string_value = reader.GetValueString("second header", 42);
```
