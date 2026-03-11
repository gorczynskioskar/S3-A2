#include <iostream>
#include "math.h"
#include <complex>
#include <cmath>
#include <iomanip>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::complex<double>* FFT(double* fn, unsigned int n) {
	if (n <= 1)
		return new std::complex<double>(fn[0], 0);
	unsigned int k = n / 2;
	double* even = new double[k];
	double* odd = new double[k];
	for (unsigned int i = 0; i < k; i++)
	{
		even[i] = fn[i * 2];
		odd[i] = fn[i * 2 + 1];
	}
	std::complex<double>* q = FFT(even, k);
	std::complex<double>* r = FFT(odd, k);

	std::complex<double>* y = new std::complex<double>[n];
	for (unsigned int m = 0; m < k; m++)
	{
		double t = -2 * M_PI * m / n;
		std::complex<double> w(cos(t), sin(t));
		y[m] = q[m] + w * r[m];
		y[m + k] = q[m] - w * r[m];
	}
	delete[] even;
	delete[] odd;
	return y;
}
std::complex<double>* DFT(double* fn, unsigned int n) {
	std::complex<double>* y = new std::complex<double>[n];
	for (unsigned int m = 0; m < n; m++)
	{
		y[m] = std::complex<double>(0, 0);
		for (unsigned int k = 0; k < n; k++)
		{
			double t = -2 * M_PI * k * m / n;
			std::complex<double> w(cos(t), sin(t));
			y[m] += w * static_cast<double>(fn[k]);
		}
	}
	return y;
}
double err(std::complex<double>* FFT, std::complex<double>* DFT, unsigned int N) {
    double sum = 0.0;
    for (unsigned int i = 0; i < N; i++) {
        sum += std::abs(FFT[i] - DFT[i]);
    }
    return (sum / N);
}
int main()
{
	const int MAX_ORDER = 13;
	const bool PRINT_COEFS = true;
	
	for (int o = 0; o <= MAX_ORDER; o++) {
		const int N = 1 << o;
		std::cout << "N = " << N <<""<< std::endl;
		double* f = new double[N];
		for (int n = 0; n < N; n++) {
			f[n] = n/(double)N;
		}
		clock_t start, end;
		start = clock();
		std::complex<double>* DFT_result = DFT(f, N);
		end = clock();
		double dft_time = double(end - start) / CLOCKS_PER_SEC * 1000.0;
		start = clock();
		std::cout << "DFT time: " << dft_time << " ms" << std::endl;
		std::complex<double>* FFT_result = FFT(f, N);
		end = clock();
		double fft_time = double(end - start) / CLOCKS_PER_SEC * 1000.0;
		std::cout << "FFT time: " << fft_time << " ms" << std::endl;
		std::cout << "Mean error: " << err(FFT_result, DFT_result, N) << "\n-------------------\n";

		if (PRINT_COEFS && o==3) {
			for (int i = 0; i < N; i++) {
				std::cout << f[i] << "\t";
			}
			std::cout << "\tDFT:\t FFT\n";
			for (int i = 0; i < N; i++) {
				std::cout << std::scientific << std::setprecision(6);
				std::cout <<i<<".\t" << DFT_result[i] << "\t" << FFT_result[i] << "\n";
			}
			std::cout << std::defaultfloat << std::setprecision(6);
			std::cout << "-------------------\n";
		}
		delete[] f;
		delete[] DFT_result;
		delete[] FFT_result;
	}
}