#pragma once

double random_value();

double Sigmoid(double x);
double Sigmoid_Derivation(double x);

double Softmax(double * inputs, int index, int length);
double Softmax_Derivation(double * inputs, int index, int length);
