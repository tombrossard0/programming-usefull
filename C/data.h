#pragma once

/* --------- DATA --------- */

struct Data {
    double input[786];
	double expected_output[10]; // Output nodes wanted
	char label[10];
};

/* --------- DATA SET --------- */

struct DataSet {
	int length;
	struct Data * data_set;
};
