#ifndef MAJORITYRULEWITHABSTDATA_H
#define MAJORITYRULEWITHABSTDATA_H


#include "algorithmwrapper.h"
#include "useabstractdata.h"
#include "QString"

class MajorityRuleWithAbstData : public AlgorithmWrapper
{
    // using AlgorithmWrapper::AlgorithmWrapper;
public:
    MajorityRuleWithAbstData(const procon::Field& field, int final_turn, bool side);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);
    void setParams(std::vector<std::pair<QString, double>> params);
    void setRandomParams(std::vector<std::pair<QString, double>> params,bool flag);

private:

    std::vector<std::vector<double>> params = {
        /*
        {0.66, 0.62, 0.43, 0.62, 0.62, 0.47 ,0.62, 0.46, 0.46, 0.65},
        {0.585501, 0.936013, 0.750115, 0.136817, 0.649388, 0.589486, 0.882378, 0.090591, 0.946676, 0.339145},
        {0.424304, 0.870152, 0.969697, 0.994353, 0.79771, 0.339376, 0.567848, 0.212827, 0.662733, 0.526067},
        {0.751296, 0.665754, 0.556531, 0.205988, 0.606986, 0.87203, 0.808129, 0.282799, 0.703115, 0.371682},
        {0.659504, 0.610229, 0.103188, 0.237939, 0.979475, 0.337169, 0.937114, 0.351038, 0.584697, 0.322641},
        {0.787281, 0.883428, 0.667984, 0.828937, 0.868376, 0.101065, 0.72732, 0.130173, 0.862691, 0.387112},
        */
        {0.959878, 0.891539, 0.705233, 0.963502, 0.921754, 0.789983, 0.92909, 0.137433, 0.685138, 0.297063},

        {0.995438, 0.886426, 0.770964, 0.451759, 0.915314, 0.15052, 0.703539, 0.36145, 0.436819, 0.333864},
        {0.940388, 0.545044, 0.761245, 0.258604, 0.859307, 0.489434, 0.79707, 0.385547, 0.461812, 0.266846},
        {0.862653, 0.951908, 0.972523, 0.643066, 0.672186, 0.587352, 0.840007, 0.428247, 0.559942, 0.395511},
        {0.930899, 0.201778, 0.100995, 0.838671, 0.97018, 0.829642, 0.781157, 0.340226, 0.561981, 0.336083},
        {0.943376, 0.253442, 0.00372481, 0.749402, 0.964857, 0.85009, 0.885714, 0.376453, 0.434016, 0.315276}



    };

    std::vector<GeneticAgent> agents;
};

#endif // MAJORITYRULEWITHABSTDATA_H
