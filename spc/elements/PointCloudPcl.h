#ifndef VOMBAT_SPCPCLCLOUD_H
#define VOMBAT_SPCPCLCLOUD_H

#include <spc/elements/PointCloudBase.h>

namespace spc
{

template <typename PointT> class PointCloudPcl : public PointCloudBase
{

    SPC_OBJECT(PointCloudPcl<PointT>)
    EXPOSE_TYPE

public:
    //    typedef spcSharedPtrMacro<spcPCLCloud<PointT> > Ptr;
    //    typedef spcSharedPtrMacro<const spcPCLCloud<PointT> > ConstPtr;

    typedef boost::shared_ptr<pcl::PointCloud<PointT>> CloudPtrT;

    PointCloudPcl(CloudPtrT cloud);

    PointCloudPcl(const pcl::PointCloud<PointT> &cloud);

    virtual void getPoint(const int id, float &x, float &y, float &z) const
        override;

    virtual void setPoint(const int id, const float x, const float y,
                          const float z) override;

    //// we assume here that is a float the value to be extracted
    virtual void getFieldValue(const int id, const std::string fieldname,
                               float &val) override;

    virtual bool hasField(const std::string fieldname) override;

    virtual int size() const override;

    virtual void resize(size_t s) override;

protected:
    // the actual data
    CloudPtrT cloud_;

    // PointCloudBase interface
public:
    virtual std::vector<std::string> getFieldNames()
    {
//        std::vector<pcl::PCLPointField> fields;
        std::string names = pcl::getFieldsList(*cloud_);
        std::vector<std::string> fields;
        boost::split(fields, names, boost::is_any_of(" "), boost::token_compress_on);

        return  fields;
    }

    // ElementBase interface
};

} // end nspace
#endif // SPCPCLCLOUD_H
