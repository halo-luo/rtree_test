#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>

#include "data_structure.h"
#include "utils.h"


using namespace std;


int main(int argc, char** argv) {


	if (argc != 7) {
		printf("Usage: %s <input_gds_file> <ACC/ECC> <threshold> <pattern_width> <pattern_height> <output_gds_file>\n", argv[0]);
		return -1;
	};
	Work work;
	// 打印参数
	cout << "参数总数：" << argc << endl;
	cout << "参数1 input gds：" << argv[1] << endl;
	cout << "参数2 mode：" << argv[2] << endl;
	cout << "参数3 threshold：" << argv[3] << endl;
	cout << "参数4 pattern width：" << argv[4] << endl;
	cout << "参数5 pattern height：" << argv[5] << endl;
	cout << "参数6 output gds：" << argv[6] << endl;
	

	if (strcmp(argv[2], "ACC") == 0) {
		work.mode = MODE::ACC;
		work.acc = std::atof(argv[3]);
	}
	else if (strcmp(argv[2], "ECC") == 0) {
		work.mode = MODE::ECC;
		work.ecc = std::atoi(argv[3]);
	}
    else
    {
        printf("\"%s\" in command line is not recognized\n", argv[2]);
        exit(-1);
    }

	work.pHeight = atof(argv[5]);
	work.pWidth = atof(argv[4]);
	strcpy_s(work.output_gds, argv[6]);


	clock_t start = clock();
	// 读取gds文件
	Layout layoutLib[2];
	vector<int> layer_list;

	//read_gds(argv[1], layoutLib);
	//read_gds("D:\\Code\\C++\\geom_cluster\\input\\testcase1.gds", layoutLib);
	
	//read_gds("D:\\Code\\Python\\geometroy_cluster\\regression\\input\\layout\\marked_testcase4_scaled.gds", layoutLib);
	//read_gds("D:\\Code\\Python\\geometroy_cluster\\regression\\input\\layout\\marked_testcase4_scaled.gds", layoutLib);
	//read_gds("D:\\Code\\Python\\geometroy_cluster\\regression\\input\\layout\\marked_testcase4_scaled.gds", layoutLib);
	read_gds("D:\\Code\\Python\\geometroy_cluster\\regression\\input\\layout\\marked_testcase4_scaled.gds", layoutLib);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < layoutLib[i].polygons.size(); j++) {
			layoutLib[i].polygons[j]->initLLRU();
		}
	}

	work.test_index(layoutLib);

	//work.ext_clip(layoutLib);

	//work.test();

	//work.test2();

	//work.test3();
	

	
	clock_t end = clock();
	cout << "The run time is: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
	return 0;
}
