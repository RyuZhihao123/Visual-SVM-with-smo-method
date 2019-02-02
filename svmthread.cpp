#include "svmthread.h"

void SVMThread::run()
{
    m_svmEngine.supportVectorMachine();

    for(unsigned int x=0; x<m_backImage.width(); x++)
    {
        for(unsigned int y=0; y<m_backImage.height(); y++)
        {
            Sample tmp;
            tmp.fID.push_back(0);
            tmp.fID.push_back(1);
            tmp.x.push_back(x);
            tmp.x.push_back(y);

            int type = m_svmEngine.predict(tmp);

            if(type == -1)
                m_backImage.setPixel(x,y,qRgba(255,70,70,100));
            else
                m_backImage.setPixel(x,y,qRgba(70,70,250,100));
        }
    }

    emit sig_threadDone();
}
