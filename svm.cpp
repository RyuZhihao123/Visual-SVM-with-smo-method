#include "svm.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#define TOLERANCE 0.001

int Sample::MaxfeatureID = 0;

SVM::SVM()
{
    iterMax = 1000;
    C=0.05;
    sigma = 1;
    p = 3;

    sigma = 0.33;
    eps=0.001f;     // Epsilon


    this->m_kernelType = KernelType::_Gaussian;
}

double max(double a,double b){
    return a>b?a:b;
}

double min(double a,double b){
    return a<b?a:b;
}

double SVM::randFloat()
{
    return (rand()%1000000)/1000000.0;
}

void SVM::clearSamples()
{
    this->m_samples.clear();
    Sample::MaxfeatureID = 0;
}

void SVM::pushOneSample(const Sample &sample)
{
    this->m_samples.push_back(sample);

    if(sample.fID.back() > Sample::MaxfeatureID)
        Sample::MaxfeatureID = sample.fID.back();
}

int SVM::predict(const Sample &x)
{
    double res = -b;

    for(unsigned int j=0; j<m_samples.size(); j++)
        res += a[j]*m_samples[j].y*kernel(m_samples[j],x);

    return res>=0?+1:-1;
}

void SVM::supportVectorMachine()
{
    // init paramters
    if(m_samples.size() <2)
        return;

    a.fill(0.0f,m_samples.size());
    b = 0.0f;

    error_cache.fill(0.0f,m_samples.size());

    w.fill(0.0f,Sample::MaxfeatureID+1);  // for linear   kernel

    srand(time(NULL));

    int num_changed = 0;
    int examine_all = 1;

    int iterCount = 0;
    while((num_changed >0 || examine_all)&&iterCount < iterMax)
    {
        num_changed = 0;

        if(examine_all)
        {
            for(unsigned int i=0; i<m_samples.size(); i++)
                num_changed += examine_example(i);
        }
        else
        {
            for(unsigned int i=0; i<m_samples.size(); i++)
            {
                if(a[i]!=0 && a[i]!=C)
                    num_changed += examine_example(i);
            }
        }

        if(examine_all)
            examine_all = 0;
        else if(num_changed == 0)
            examine_all = 1;

        for(unsigned int i=0; i<m_samples.size(); i++)
        {
            if(a[i] < 1e-6)
                a[i] = 0.0;
        }

        iterCount += 1;
        qDebug()<<"ITER: "<<iterCount;
    }
}

int SVM::examine_example(double id1)
{
    double y1 = m_samples[id1].y;
    double alph1 = a[id1];
    double e1 = 0.0;

    if(alph1>0 && alph1<C)
        e1 = error_cache[id1];
    else
        e1 = modelFunction(id1)-y1;

    if((y1*e1<-TOLERANCE && alph1<C) || (y1*e1>TOLERANCE && alph1 >0))
    {
        int id2 = -1;
        double _max = 0.0;

        // Way1: Try argmax |E1-E2|
        for(unsigned int k =0;k<m_samples.size(); k++)
        {
            if(a[k]>0 && a[k]<C)
            {
                double e2 = error_cache[k];
                double tmp = fabs(e1-e2);

                if(tmp > _max)
                {
                    _max = tmp;
                    id2 = k;
                }
            }
        }

        if(id2 >= 0)
        {
            if(take_step(id1,id2))
                return 1;
        }

        // Way2: Try iterating through the non-bound examples;
        int k0 = (int)(randFloat()*m_samples.size());
        for(int k=k0; k<m_samples.size()+k0;k++)
        {
            id2 = k%m_samples.size();

            if(a[id2]>0 && a[id2]<C)
            {
                if(take_step(id1,id2))
                    return 1;
            }
        }

        // Way3: try iterating through the entire training set;
        k0 = (int)(randFloat()*m_samples.size());
        for(int k=k0;k<m_samples.size()+k0;k++)
        {
            id2 = k%m_samples.size();

            if(take_step(id1,id2))
                return 1;
        }
    }
    return 0;
}

int SVM::take_step(int id1, int id2)
{
    if(id1 == id2)
        return 0;

    double alph1 = a[id1];  // old values of alpha[id1]
    double alph2 = a[id2];  // old values of alpha[id2]
    float e1,e2;
    int y1   = m_samples[id1].y;
    int y2   = m_samples[id2].y;

    if(alph1>0 && alph2<C)
        e1 = error_cache[id1];
    else
        e1 = modelFunction(id1) - y1;

    if(alph2>0 && alph2<C)
        e2 = error_cache[id2];
    else
        e2 = modelFunction(id2) - y2;

    int s = y1*y2;

    double L,H;

    if(y1 == y2)
    {
        L = max (0, alph1+alph2-C);
        H = min (C, alph1+alph2);
    }
    else
    {
        L = max(0, alph1-alph2);
        H = min(C, C-alph1+alph2);
    }

    if(fabs(L-H)<1e-6)  // L==H
        return 0;

    float k11 = kernel(m_samples[id1],m_samples[id1]);
    float k12 = kernel(m_samples[id1],m_samples[id2]);
    float k22 = kernel(m_samples[id2],m_samples[id2]);
    double eta = 2*k12 - k11 - k22;

    float a1_new,a2_new;
    if(eta < 0)
    {
        a2_new = alph2 + y2*(e2-e1)/eta;

        if(a2_new < L)
            a2_new = L;
        else if(a2_new >H)
            a2_new = H;
    }
    else
    {
        double c1 = eta/2.0;
        double c2 = y2*(e1-e2)-eta*alph2;
        double Lobj  = c1*L*L + c2*L;
        double Hobj = c1*H*H + c2*H;

        if(Lobj > Hobj + eps)
            a2_new = L;
        else if(Lobj < Hobj-eps)
            a2_new = H;
        else
            a2_new = alph2;
    }

    // If too close.
    if(fabs(a2_new - alph2) < eps*(a2_new+alph2 +eps))
        return 0;

    a1_new = alph1 - s*(a2_new -alph2);

    if(a1_new <0)
    {
        a2_new += s*a1_new;
        a1_new = 0;
    }
    else if(a1_new > C)
    {
        a2_new += s*(a1_new -C);
        a1_new = C;
    }

    // Update b(threshold)
    double bnew;
    if(a1_new > 0 && a1_new <C)
        bnew = b + e1 + y1*(a1_new -alph1)*k11 + y2*(a2_new-alph2)*k12;
    else if(a2_new >0 && a2_new <C)
        bnew = b + e2 + y1*(a1_new -alph1)*k12 + y2*(a2_new-alph2)*k22;
    else
    {
        double b1 = b + e1 + y1*(a1_new -alph1)*k11 + y2*(a2_new-alph2)*k12;
        double b2 = b + e2 + y1*(a1_new -alph1)*k12 + y2*(a2_new-alph2)*k22;
        bnew = (b1+b2)*0.5;
    }

    double delta_b = bnew - b;

    b = bnew;

    float t1 = y1*(a1_new - alph1);
    float t2 = y2*(a2_new - alph2);

    // update weight vector if linear SVM
    if(this->m_kernelType == KernelType::_linear)
    {
        // w = w + t1*m_samples[id1] + t2*m_samples[id2]
        w = Sample::plus(w,Sample::multiplyNum(m_samples[id1],t1));
        w = Sample::plus(w,Sample::multiplyNum(m_samples[id2],t2));
    }

    // update Error cache
    for(unsigned int i=0; i<m_samples.size(); i++)
    {
        if(a[i] >0 && a[i]<C)
            error_cache[i] += t1*kernel(m_samples[id1],m_samples[i]) + t2*kernel(m_samples[id2],m_samples[i]) - delta_b;
    }

    error_cache[id1] = 0.0;
    error_cache[id2] = 0.0;

    a[id1] = a1_new;
    a[id2] = a2_new;

    return 1;
}

double SVM::modelFunction(int id)   // f(x);
{
   float s = 0;

   if(this->m_kernelType != KernelType::_linear)
   {
       s = Sample::product(w,m_samples[id]);
   }
   else
   {
       for(unsigned int i=0; i<m_samples.size(); i++)
       {
           if(a[i]>0)
               s += a[i]*m_samples[i].y*kernel(m_samples[i],m_samples[id]);
       }
   }

   s -= b;
   return s;
}

double SVM::kernel(const Sample &xi, const Sample &xj)
{
    switch ((int)this->m_kernelType) {
    case KernelType::_linear:
        return linearKernel(xi,xj);
    case KernelType::_polynomial:
        return polynomialKernel(xi,xj);
    case KernelType::_Gaussian:
        return gaussianKernel(xi,xj);
    default:
        return linearKernel(xi,xj);
    }
}

double SVM::linearKernel(const Sample &xi, const Sample &xj)
{
    return Sample::product(xi,xj);
}

double SVM::polynomialKernel(const Sample &xi, const Sample &xj)
{
    return pow(Sample::product(xi,xj)+1,p);
}

double SVM::gaussianKernel(const Sample &xi, const Sample &xj)
{
    return exp(-Sample::minus(xi,xj).modSquared()*sigma);
}
