
#include <cmath>

#define M_PI (acos(0.0f)*2)

void IIR_LPF(float fc, float Q, float a[], float b[])
{
	fc = tan(M_PI * fc) / (2.0f * M_PI);
  
	a[0] = 1.0f + 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc;
	a[1] = (8.0f * M_PI * M_PI * fc * fc - 2.0f) / a[0];
	a[2] = (1.0f - 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc) / a[0];
	b[0] = 4.0f * M_PI * M_PI * fc * fc / a[0];
	b[1] = 8.0f * M_PI * M_PI * fc * fc / a[0];
	b[2] = 4.0f * M_PI * M_PI * fc * fc / a[0];
  
	a[0] = 1.0f;
}

void IIR_HPF(float fc, float Q, float a[], float b[])
{
	fc = tan(M_PI * fc) / (2.0f * M_PI);
  
	a[0] = 1.0f + 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc;
	a[1] = (8.0f * M_PI * M_PI * fc * fc - 2.0f) / a[0];
	a[2] = (1.0f - 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc) / a[0];
	b[0] = 1.0f / a[0];
	b[1] = -2.0f / a[0];
	b[2] = 1.0f / a[0];
  
	a[0] = 1.0f;
}

void IIR_BPF(float fc1, float fc2, float a[], float b[])
{
	fc1 = tan(M_PI * fc1) / (2.0f * M_PI);
	fc2 = tan(M_PI * fc2) / (2.0f * M_PI);
  
	a[0] = 1.0f + 2.0f * M_PI * (fc2 - fc1) + 4.0f * M_PI * M_PI * fc1 * fc2;
	a[1] = (8.0f * M_PI * M_PI * fc1 * fc2 - 2.0f) / a[0];
	a[2] = (1.0f - 2.0f * M_PI * (fc2 - fc1) + 4.0f * M_PI * M_PI * fc1 * fc2) / a[0];
	b[0] = 2.0f * M_PI * (fc2 - fc1) / a[0];
	b[1] = 0.0f;
	b[2] = -2.0f * M_PI * (fc2 - fc1) / a[0];
  
	a[0] = 1.0f;
}

void IIR_BEF(float fc1, float fc2, float a[], float b[])
{
	fc1 = tan(M_PI * fc1) / (2.0f * M_PI);
	fc2 = tan(M_PI * fc2) / (2.0f * M_PI);
  
	a[0] = 1.0f + 2.0f * M_PI * (fc2 - fc1) + 4.0f * M_PI * M_PI * fc1 * fc2;
	a[1] = (8.0f * M_PI * M_PI * fc1 * fc2 - 2.0f) / a[0];
	a[2] = (1.0f - 2.0f * M_PI * (fc2 - fc1) + 4.0f * M_PI * M_PI * fc1 * fc2) / a[0];
	b[0] = (4.0f * M_PI * M_PI * fc1 * fc2 + 1.0f) / a[0];
	b[1] = (8.0f * M_PI * M_PI * fc1 * fc2 - 2.0f) / a[0];
	b[2] = (4.0f * M_PI * M_PI * fc1 * fc2 + 1.0f) / a[0];
  
	a[0] = 1.0;
}

void IIR_resonator(float fc, float Q, float a[], float b[])
{
	fc = tan(M_PI * fc) / (2.0f * M_PI);
  
	a[0] = 1.0f + 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc;
	a[1] = (8.0f * M_PI * M_PI * fc * fc - 2.0f) / a[0];
	a[2] = (1.0f - 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc) / a[0];
	b[0] = 2.0f * M_PI * fc / Q / a[0];
	b[1] = 0.0f;
	b[2] = -2.0f * M_PI * fc / Q / a[0];
  
	a[0] = 1.0;
}

void IIR_notch(float fc, float Q, float a[], float b[])
{
	fc = tan(M_PI * fc) / (2.0f * M_PI);
  
	a[0] = 1.0f + 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc;
	a[1] = (8.0f * M_PI * M_PI * fc * fc - 2.0f) / a[0];
	a[2] = (1.0f - 2.0f * M_PI * fc / Q + 4 * M_PI * M_PI * fc * fc) / a[0];
	b[0] = (4.0f * M_PI * M_PI * fc * fc + 1.0f) / a[0];
	b[1] = (8.0f * M_PI * M_PI * fc * fc - 2.0f) / a[0];
	b[2] = (4.0f * M_PI * M_PI * fc * fc + 1.0f) / a[0];
  
	a[0] = 1.0;
}

void IIR_low_shelving(float fc, float Q, float g, float a[], float b[])
{
	fc = tan(M_PI * fc) / (2.0f * M_PI);
  
	a[0] = 1.0f + 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc;
	a[1] = (8.0f * M_PI * M_PI * fc * fc - 2.0f) / a[0];
	a[2] = (1.0f - 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc) / a[0];
	b[0] = (1.0f + sqrt(1.0f + g) * 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc * (1.0f + g)) / a[0];
	b[1] = (8.0f * M_PI * M_PI * fc * fc * (1.0f + g) - 2.0f) / a[0];
	b[2] = (1.0f - sqrt(1.0f + g) * 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc * (1.0f + g)) / a[0];
  
	a[0] = 1.0f;
}

void IIR_high_shelving(float fc, float Q, float g, float a[], float b[])
{
	fc = tan(M_PI * fc) / (2.0f * M_PI);
  
	a[0] = 1.0f + 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc;
	a[1] = (8.0f * M_PI * M_PI * fc * fc - 2.0f) / a[0];
	a[2] = (1.0f - 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc) / a[0];
	b[0] = ((1.0f + g) + sqrt(1.0f + g) * 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc) / a[0];
	b[1] = (8.0f * M_PI * M_PI * fc * fc - 2.0f * (1.0f + g)) / a[0];
	b[2] = ((1.0f + g) - sqrt(1.0f + g) * 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc) / a[0];
  
	a[0] = 1.0f;
}

void IIR_peaking(float fc, float Q, float g, float a[], float b[])
{
	fc = tan(M_PI * fc) / (2.0f * M_PI);
  
	a[0] = 1.0f + 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc;
	a[1] = (8.0f * M_PI * M_PI * fc * fc - 2.0f) / a[0];
	a[2] = (1.0f - 2.0f * M_PI * fc / Q + 4.0f * M_PI * M_PI * fc * fc) / a[0];
	b[0] = (1.0f + 2.0f * M_PI * fc / Q * (1.0f + g) + 4.0f * M_PI * M_PI * fc * fc) / a[0];
	b[1] = (8.0f * M_PI * M_PI * fc * fc - 2.0f) / a[0];
	b[2] = (1.0f - 2.0f * M_PI * fc / Q * (1.0f + g) + 4.0f * M_PI * M_PI * fc * fc) / a[0];
  
	a[0] = 1.0f;
}