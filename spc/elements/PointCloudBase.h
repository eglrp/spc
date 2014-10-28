#ifndef GENERIC_CLOUD_H
#define GENERIC_CLOUD_H

#include <spc/elements/ElementBase.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/io.h>

#include <spc/methods/common.h>
#include <pcl/search/impl/flann_search.hpp>
#include <boost/make_shared.hpp>

#include <spc/elements/OrientedSensor.h>


namespace spc
{

class PointCloudBase : public ElementBase
{

public:
    SPC_OBJECT(PointCloudBase)
    EXPOSE_TYPE
    PointCloudBase();

    virtual int getNearestPointID(const Eigen::Vector3f query,
                                  float &sq_distance);


    //! sensor info access
    virtual OrientedSensor getSensor() const  = 0;

    virtual void setSensor(const OrientedSensor &sensor) const = 0;

    virtual void getPoint(const int id, float &x, float &y, float &z) const
    {
        getFieldValue(id, "x", x);
        getFieldValue(id, "y", y);
        getFieldValue(id, "z", z);
    }

    virtual void setPoint(const int id, const float x, const float y,
                          const float z)
    {
        setFieldValue(id, "x", x);
        setFieldValue(id, "y", y);
        setFieldValue(id, "z", z);
    }

    /// a generic cloud must implement these method
    virtual void getNormal(const int id, float &x, float &y, float &z) const
    {
        getFieldValue(id, "normal_x", x);
        getFieldValue(id, "normal_y", y);
        getFieldValue(id, "normal_z", z);
    }

    virtual void setNormal(const int id, const float x, const float y,
                           const float z)
    {
        setFieldValue(id, "normal_x", x);
        setFieldValue(id, "normal_y", y);
        setFieldValue(id, "normal_z", z);
    }

    virtual pcl::PointCloud<pcl::PointXYZ>::Ptr getAsPclXyz()
    {
        if (!xyz_representation_)
            updateXYZRepresentation();

        return xyz_representation_;
    }

    void updateFlannSearcher();

    virtual void updateXYZRepresentation();

    pcl::search::FlannSearch<pcl::PointXYZ>::Ptr getFlannSearcher()
    {
        if (!searcher_)
            updateFlannSearcher();

        return searcher_;
    }

    virtual void getFieldValue(const int id, const std::string fieldname,
                               float &val) const = 0;

    virtual void setFieldValue(const int id, const std::string fieldname,
                               const float &val) = 0;

    virtual void addField(const std::string &name) = 0;

    virtual void addFields(const std::vector<std::string> field_names,
                           const Eigen::MatrixXf &data);

    virtual int size() const = 0;

    virtual bool hasField(const std::string fieldname) const = 0;

    virtual std::vector<std::string> getFieldNames() const = 0;

    virtual void resize(size_t s) = 0;

    virtual Eigen::Vector3f getPoint(const int id) const;

    virtual Eigen::Vector3f getNormal(const int id) const;

    virtual std::vector<float> getField(const std::string fieldname,
                                        std::vector<int> indices);

    virtual std::vector<float> getField(const std::string fieldname);

    virtual bool getField(const std::string fieldname, Eigen::VectorXf &vector);

    pcl::PointCloud<pcl::PointXYZ>
    applyTransform(const Eigen::Transform
                   <float, 3, Eigen::Affine, Eigen::AutoAlign> &T);


    virtual pcl::PCLPointCloud2Ptr asPCLData() const;

    //! reurns true if ALL the fields exists
    bool hasFields(const std::vector<std::string> &field_names) const;

protected:
    pcl::search::FlannSearch<pcl::PointXYZ>::Ptr searcher_;

    pcl::PointCloud<pcl::PointXYZ>::Ptr xyz_representation_;




};// end class


class PointCloudBaseWithSensor: public PointCloudBase
{

public:
    SPC_OBJECT(PointCloudBaseWithSensor)
    EXPOSE_TYPE
    PointCloudBaseWithSensor(): sensor_(new OrientedSensor)
    {
    }

    // PointCloudBase interface
public:
    virtual OrientedSensor getSensor() const
    {
        return *sensor_;
    }

    virtual void setSensor(const OrientedSensor &sensor) const
    {
        *sensor_ = sensor;
    }

protected:
    OrientedSensor::Ptr sensor_;
};


} // end nspace

#endif // GENERIC_CLOUD_H
