#include <stdio.h>
#include <QApplication>


#include "voSystem.h"
#include "Optimization.h"
#include "ImgIO.h"
#include "Frame.h"

voSystem::voSystem():sysNeedUpdate(false)
{
}

bool voSystem::running()
{
    imgIO = std::make_shared<ImgIO>();
    optimizer = std::make_shared<Optimizater>();

    trackThread = new boost::thread(std::bind(&voSystem::tracking, this));
    optThread = new boost::thread(std::bind(&voSystem::optimize, this));

    return true;
}

std::shared_ptr<Frame> &voSystem::getCurrentFrame()
{
    return currentFrame;
}

const cv::Mat &voSystem::getImage(void)
{
    boost::shared_lock<boost::shared_mutex> lock(currentFrameMutex);
    if(currentFrame.get() != nullptr) {
        return currentFrame->RGBImg();
    }
    else {
        printf("currentFrame.get() == nullptr!\n");
        return cv::Mat();
    }
}

int voSystem::getPoints(std::shared_ptr<std::vector<std::shared_ptr<Eigen::Vector3f>>> &points)
{
    if(points.get() == nullptr) {
        return -1;
    }

    points->clear();
    for (int var = 0; var < 20; ++var) {
        points->push_back(std::make_shared<Eigen::Vector3f>(10.0+1.2*var, 12.0 - 3*var, 1.0));
    }

    return -1;
}

void voSystem::tracking()
{
    printf("tracking!\n");
    while(true) {
        {
            boost::unique_lock<boost::shared_mutex> lock(currentFrameMutex);
            std::shared_ptr<Frame>& frame = imgIO->getFrame();
            if(frame.get() == nullptr){
                continue;
            }

            if(!sysNeedUpdate){
                currentFrame.swap(frame);
                sysNeedUpdate = true;
            }
        }
    }
}

void voSystem::optimize()
{
    printf("optimize!\n");
}

