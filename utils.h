#pragma once
#include "data_structure.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <memory>
#include <unordered_map>
#include <stdexcept>


static const std::unordered_map<int, std::unordered_map<int, std::string>> codeMap = {
		{0, {{2, "HEADER Release # "}}},
		{1, {{2, "BGNLIB"}}},
		{2, {{6, "LIBNAME = "}}},
		{32, {{6, "FONTS = "}}},
		{34, {{2, "GENERATIONS"}}},
		{3, {{5, "UNITS = "}}},
		{5, {{2, "BGNSTR"}}},
		{6, {{6, "STRNAME = "}}},
		{8, {{0, "BOUNDARY"}}},
		{13, {{2, "LAYER = "}}},
		{14, {{2, "DATATYPE = "}}},
		{16, {{3, "XY = "}}},
		{17, {{0, "ENDEL"}}},
		{7, {{0, "ENDSTR"}}},
		{4, {{0, "ENDLIB"}}},
		{10, {{0, "SREF"}}},
		{18, {{6, "SNAME"}}},
		{26, {{1, "STRANS"}}},
		{12, {{0, "TEXT"}}},
		{22, {{2, "TEXTTYPE"}}},
		{23, {{1, "PRESENTATION"}}},
		{27, {{5, "MAG"}}},
		{25, {{6, "STRING"}}},
		{33, {{2, "PATHTYPE"}}},
		{15, {{3, "WIDTH"}}},
		{9, {{0, "PATH"}}},
		{28, {{5, "ANGLE"}}}
};


std::string CodeID(int hi, int lo)
{
	//std::cout << "COdeID  hi = " << hi << " lo = " << lo << std::endl;
	auto hiIt = codeMap.find(hi);
	if (hiIt != codeMap.end()) {
		auto loIt = hiIt->second.find(lo);
		if (loIt != hiIt->second.end()) {
			return loIt->second;
		}
	}
	return ""; // Return empty string if not found


	//string str = "";
	////if ( hi == 00 && lo == 02 ) str = "HEADER release # ";
	//if (hi == 00 && lo == 02) str = "HEADER Release # ";
	//if (hi == 01 && lo == 02) str = "BGNLIB"; // 0101
	//if (hi == 02 && lo == 06) str = "LIBNAME = "; // 0206
	//if (hi == 32 && lo == 06) str = "FONTS = "; // 2006
	//if (hi == 34 && lo == 02) str = "GENERATIONS"; // 2202
	//if (hi == 03 && lo == 05) str = "UNITS = "; // 0305
	//if (hi == 05 && lo == 02) str = "BGNSTR"; // 0502
	//if (hi == 06 && lo == 06) str = "STRNAME = "; // 0606
	//if (hi == 8 && lo == 00) str = "BOUNDARY"; // 0800
	//if (hi == 13 && lo == 02) str = "LAYER = "; // 0d02
	//if (hi == 14 && lo == 02) str = "DATATYPE = "; // 0e02
	//if (hi == 16 && lo == 03) str = "XY = "; // 1003
	//if (hi == 17 && lo == 0) str = "ENDEL"; // 1100
	//if (hi == 7 && lo == 0) str = "ENDSTR"; // 0700
	//if (hi == 4 && lo == 0) str = "ENDLIB"; // 0400
	//if (hi == 10 && lo == 0) str = "SREF"; // 0a00
	//if (hi == 18 && lo == 6) str = "SNAME"; // 1206
	//if (hi == 26 && lo == 1) str = "STRANS"; // 1a01
	//if (hi == 12 && lo == 0) str = "TEXT"; // 0c00
	//if (hi == 22 && lo == 2) str = "TEXTTYPE"; // 1602
	//if (hi == 23 && lo == 1) str = "PRESENTATION"; // 1701
	//if (hi == 27 && lo == 5) str = "MAG"; // 1b05
	//if (hi == 25 && lo == 6) str = "STRING"; // 1906
	//if (hi == 33 && lo == 2) str = "PATHTYPE"; // 2102
	//if (hi == 15 && lo == 3) str = "WIDTH"; // 0f03
	//if (hi == 9 && lo == 0) str = "PATH"; // 0900
	//if (hi == 28 && lo == 5) str = "ANGLE"; // 1C05
	//return str;
}


void print_str(char* str) {
	std::cout << str[0] << " " << str[1] << " " << str[2] << " " << str[3] << std::endl;
};

void print_int(int a, int b, int c, int d) {
	std::cout << a << " " << b << " " << c << " " << d << std::endl;
};


int Numb_Bytes(int hi, int lo)
{
	//std::cout << "hi = " << hi << " lo = " << lo << std::endl;
	//int numb;
	if (hi < 0) hi = hi + 256;
	if (lo < 0) lo = lo + 256;
	//numb = (hi << 8) + lo;
	return (hi << 8) + lo;
}


unsigned Int_2u(unsigned hi, unsigned lo)
{
	//int numb;
	if ((int)hi < 0) hi = hi + 256;
	if ((int)lo < 0) lo = lo + 256;
	//numb = (hi << 8) + lo;
	return (hi << 8) + lo;
}


int Int_Sign4(int first, int second, int third, int forth)
{
	//int numb;
	if (second < 0) second = second + 256;
	if (third < 0) third = third + 256;
	if (forth < 0) forth = forth + 256;
	//numb = (first << 24) + (second << 16) + (third << 8) + forth;
	return (first << 24) + (second << 16) + (third << 8) + forth;
}


double Real_8(int X0, int X1, int X2, int X3, int X4, int X5, int X6, int X7)
{
	if (X0 < 0) X0 = X0 + 256;
	if (X1 < 0) X1 = X1 + 256;
	if (X2 < 0) X2 = X2 + 256;
	if (X3 < 0) X3 = X3 + 256;
	if (X4 < 0) X4 = X4 + 256;
	if (X5 < 0) X5 = X5 + 256;
	if (X6 < 0) X6 = X6 + 256;
	if (X7 < 0) X7 = X7 + 256;
	int exp, i, bit, testbit, scalebit;
	exp = (X0 & 127); // will bit mask to = 01111111
	exp = 0;
	for (i = 1; i < 8; i++)
	{
		bit = (int)pow(2, 7 - i);
		exp = exp + (X0 & bit);
	}
	exp = exp - 65; // add offset
	double matval, matvalb, value;
	matval = 0;
	for (i = 0; i < 8; i++)
	{
		bit = pow(2, 7 - i);
		scalebit = pow(2, i);
		testbit = (X1 & bit);
		matvalb = (double)8 * (X1 & bit) / (bit * scalebit);
		matval = matval + matvalb;
		matvalb = (double)8 * (X2 & bit) / (256 * bit * scalebit);
		matval = matval + matvalb;
		matvalb = (double)8 * (X3 & bit) / (256 * 256 * bit * scalebit);
		matval = matval + matvalb;
	}
	value = matval * pow(16, exp);
	return value;
}




class GdsReadError :public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};


void read_gds(const char* inputfile, Layout* layoutLib)
{
	
	std::cout << inputfile << " is the file being read \n";
	std::ifstream file(inputfile, ios::in | ios::binary | ios::ate);
	if (!file) {
		std::cout << "Unable to open file" << inputfile << std::endl;
		throw GdsReadError("Unable to open file");
	}

	//getPointer = file.tellg(); //Get position of the get pointer
	
	const auto file_size = file.tellg();
	//memblock = new char[getPointer];
	
	std::cout << "memblock size = " << file_size << endl;
	file.seekg(0, ios::beg); // repositions get pointer to beginning

	// 一次性读取所有文件
	std::vector<char> file_buffer(file_size);
	if (!file.read(file_buffer.data(), file_size)) {
		throw GdsReadError("file read error.");
	}

	set<pint> layer_dt_set;
	std::vector<Point> point_vector;
	//Polygon* polygon = new Polygon();
	std::unique_ptr<Polygon> polygon = std::make_unique<Polygon>();

	const char* ptr = file_buffer.data();
	const char* end = ptr + file_size;
	int temp_layer = 0, temp_datatype = 0;


	string ID_str, refl_str;
	int numb = 0, val, point_x, point_y;
	double realVal;
	int first, second, third, forth;

	while (ptr < end) {
		//first read numb bytes in block
		first = ptr[0];
		second = ptr[1];
		third = ptr[2];
		forth = ptr[3];
		ptr += 4;
		numb = Numb_Bytes(first, second);
		numb -= 4;
		ID_str = CodeID(third, forth);
		/*std::cout << first << " " << second << " " << third << " " << forth << " " << numb << endl;
		std::cout << "ID_str = " << ID_str << " numb = " << numb << endl;
		std::cout << (void*)ptr << "  " << (void*)end << endl;*/
		

		// 根据记录类型处理数据
		switch (third) {

		case 4: {
			std::cout << "End of Library" << endl;
			ptr = end;
			break;
		}

		// unit and percesion
		case 3: {
			if (forth == 5) {

				realVal = Real_8(ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
				std::cout << "Units: " << realVal << '\n';

				realVal = Real_8(ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15]);
				std::cout << "Precision: " << realVal << '\n';

			}
			break;
		}

		// cell name  // STRNAME
		case 6: {

			if (forth == 6) {
				std::string cell_name(ptr, ptr + numb);
				cell_name += '\0';
				std::cout << "Cell name: " << cell_name << '\n';
			}
			break;
		}

		// layer
		case 13: {

			if (forth == 2) {
				temp_layer = Int_2u(ptr[0], ptr[1]);
			}
			break;
		}
		
		// datatype
		case 14: {  
			
			if (forth == 2) {
				temp_datatype = Int_2u(ptr[0], ptr[1]);
				const pint key{ temp_layer, temp_datatype };
				layer_dt_set.insert(key);
				layoutLib[layer_dt_set.size() - 1].layer = temp_layer;
				layoutLib[layer_dt_set.size() - 1].datatype = temp_datatype;
			}

			break;
		}

		case 16: {  // XY intinates

			if (forth == 3) {
				const size_t num_points = numb / 8;

				polygon->points.reserve(num_points);

				for (size_t i = 0; i < num_points; ++i) {
					
					point_x = Int_Sign4(ptr[0 + 8 * i], ptr[1 + 8 * i], ptr[2 + 8 * i], ptr[3 + 8 * i]);
					point_y = Int_Sign4(ptr[4 + 8 * i], ptr[5 + 8 * i], ptr[6 + 8 * i], ptr[7 + 8 * i]);

					polygon->points.emplace_back(Point{ point_x, point_y });
					
				}

				layoutLib[layer_dt_set.size() - 1].polygons.emplace_back(std::move(polygon));
				polygon = std::make_unique<Polygon>();

			}
			
			break;
		}
		// 其他记录类型的处理...
		default:
			break;
		}
		
		ptr += numb;
	}



	std::cout << "layer_dt_set size = " << layer_dt_set.size() << endl;
	for (set<pint>::iterator it = layer_dt_set.begin(); it != layer_dt_set.end(); ++it) {
		cout << it->first << " " << it->second << endl;
	};

};
