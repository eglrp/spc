#ifndef VOMBAT_KERNELSMOOTHING2_H
#define VOMBAT_KERNELSMOOTHING2_H

#include <spc/time_series/sparse_time_series.h>
#include <spc/time_series/equally_spaced_time_series.h>

//#include <flann/flann.hpp>
#include <flann/flann.hpp>

#include <pcl/console/print.h>
#include <boost/foreach.hpp>

using namespace flann;

namespace spc
{




template <typename ScalarT>
class KernelSmoothing2
{

public:
    typedef SparseTimeSeries<ScalarT> SparseT;
    typedef boost::shared_ptr<SparseT> SparsePtrT;

    typedef EquallySpacedTimeSeries<ScalarT> EquallyT;
    typedef boost::shared_ptr<EquallyT> EquallyPtrT;



    typedef typename flann::L2_Simple<ScalarT> distType;
    typedef typename flann::Index<distType> FLANNIndex;
    typedef typename flann::Matrix<ScalarT> FLANNMat;


    KernelSmoothing2();

    void setInputSeries(SparsePtrT in)
    {
        sparse_ = in;
    }

    void setOutputSeriesBlank(EquallyPtrT out)
    {
        out_series_ = out;
    }


    void setStep(ScalarT step)
    {
        step_ = step;
    }

    void setBandwidth(ScalarT bandwidth)
    {
        bandwidth_ = bandwidth;
    }


    EquallyPtrT getOutputSeries() const
    {
        return out_series_;
    }

    int compute();

protected:

    void initFlann();

    SparsePtrT sparse_;
    EquallyPtrT out_series_;

    flann::KDTreeSingleIndexParams pars_;

    ScalarT step_;
    ScalarT bandwidth_;


    boost::shared_ptr<FLANNIndex> flann_index_;

    //compute gaussian weights on a vector
    inline void
    gaussian(const std::vector<ScalarT> &values, std::vector<ScalarT> &gaussian_values)
    {
        BOOST_FOREACH (float val, values)
        {
            gaussian_values.push_back(gaussian(val));
        }
    }


    //compute gaussian weights on a single numeric value
    inline ScalarT
    gaussian(const ScalarT &value)
    {
        return 1.0/sqrt(2.0*M_PI) * exp(-0.5*value*value);
    }

    void
    initKDTree();

    int
    radiusSearch(const ScalarT &position,const ScalarT &radius, std::vector<int> &ids, std::vector<ScalarT> &distances);

    int
    evaluateKS(const ScalarT &position, ScalarT &value);





};

}//end nspace

#endif // KERNELSMOOTHING2_H