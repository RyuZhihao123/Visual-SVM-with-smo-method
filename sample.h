#ifndef SAMPLE_H
#define SAMPLE_H
#include <QVector>
#include <QDebug>

struct Sample
{
    QVector<double> x;     // feature vector
    QVector<double> fID;   // feature ID

    double y;

    static int MaxfeatureID; // 总共有(MaxFeatureID+1)个feature[从0开始]

    double modSquared()
    {
        double res = 0;
        for(unsigned int i=0; i<x.size(); i++)
        {
            res += x[i]*x[i];
        }
        return res;
    }

    static double product(const Sample& x1,const Sample& x2)
    {
        double res=0;

        int i=0;
        int j=0;

        while(true)
        {
            if(i>=x1.fID.size() || j>=x2.fID.size())
                break;
            if(x1.fID[i] == x2.fID[j]){
                res += (x1.x[i]*x2.x[j]);
                i++;
                j++;
            }
            else{
                if(x1.fID[i] > x2.fID[j])
                    j++;
                else if(x1.fID[i]<x2.fID[j])
                    i++;
            }
        }

        return res;
    }

    static double product(const QVector<double>& w, const Sample& x)
    {
        double res = 0;

        for(unsigned int i=0; i<x.fID.size(); i++)
        {
            int id = x.fID[i];

            res += w[id]*x.x[i];
        }

        return res;
    }

    static QVector<double> plus(const QVector<double>& w, const Sample& x)
    {
        QVector<double> res;
        res.resize(w.size());
        for(unsigned int i=0; i<x.fID.size(); i++)
        {
            int id = x.fID[i];

            res[id] = w[id] + x.x[i];
        }

        return res;
    }

    static Sample multiplyNum(const Sample& x, float t)
    {
        Sample res;

        for(unsigned int i=0; i<x.x.size(); i++)
        {
            res.x.push_back(t*x.x[i]);
            res.fID.push_back(x.fID[i]);
        }

        return res;
    }

    static Sample minus(const Sample& x1,const Sample& x2)
    {
        Sample tmp;

        int i=0;
        int j=0;

        while(true)
        {
            if(i>=x1.fID.size() || j>=x2.fID.size())
                break;

            if(x1.fID[i] < x2.fID[j])
            {
                tmp.x.push_back(x1.x[i]);
                tmp.fID.push_back(x1.fID[i]);
                i++;
            }
            else if(x1.fID[i] > x2.fID[j])
            {
                tmp.x.push_back(-x2.x[j]);
                tmp.fID.push_back(x2.fID[j]);
                j++;
            }
            else if(x1.fID[i] == x2.fID[j])
            {
                tmp.x.push_back(x1.x[i]-x2.x[j]);
                tmp.fID.push_back(x1.fID[i]);
                i++;
                j++;
            }
        }

        if(i < x1.fID.size())
        {
            for(;i<x1.fID.size();i++)
            {
                tmp.x.push_back(x1.x[i]);
                tmp.fID.push_back(x1.fID[i]);
            }
        }
        if(j < x2.fID.size())
        {
            for(;j<x2.fID.size();j++)
            {
                tmp.x.push_back(-x2.x[j]);
                tmp.fID.push_back(x2.fID[j]);
            }
        }

        return tmp;
    }


};


#endif // SAMPLE_H
