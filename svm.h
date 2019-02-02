#ifndef SVM_H
#define SVM_H
#include "sample.h"

class SVM
{
public:
    SVM();

    enum KernelType
    {
        _linear,
        _polynomial,
        _Gaussian,

    }m_kernelType;    // Kernel Function Type

    void supportVectorMachine();   // TRAIN
    int predict(const Sample& x);  // PREDICT

    void clearSamples();
    void pushOneSample(const Sample& sample);
    void popOneSample();

    // SVM parameters
    double C;         // penality paramters
    double sigma;     // sigma = 1.0/(2*sigma*sigma)  exp(-sigma*|x-j|^2))
    double eps;       // Epsilon
    int p;            // Polynomial Kernel index: (xT*x)^p


private:
    QVector<Sample> m_samples;    // ALL samples

    double b;               // threshold
    QVector<double> a;      // alpha (Lagrangian multipliers)
    QVector<double> w;      // linear weight vector;

    int iterMax;

    QVector<double> error_cache;
    bool is_linear_kernel;

    int examine_example(double id1);
    int take_step(int id1,int id2);

    double modelFunction(int id);
    double randFloat();

    // Some kernel functions:
    double kernel(const Sample& xi,const Sample& xj);
    double linearKernel(const Sample& xi,const Sample& xj);
    double polynomialKernel(const Sample& xi,const Sample& xj);
    double gaussianKernel(const Sample& xi,const Sample& xj);
};

#endif // SVM_H
