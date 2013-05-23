#ifndef PCL_HELPER_H
#define PCL_HELPER_H

#include <iostream>
#include <fstream>

#include <sensor_msgs/PointCloud2.h>
#include <pcl/common/io.h>


void
printHeader(const std::string &filename)
{
    //open file
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cout << "error opening file" << std::endl;
        return;
    }

    std::string line;
    for (size_t i = 0 ; i < 11 ; ++i)
    {
        std::getline(file, line);
        std::cout << line << std::endl;
    }

}


template <typename ScalarT>
std::vector<ScalarT>
readFieldToVector(const sensor_msgs::PointCloud2 &cloud,
                       const std::string &fieldname,
                       const uint count = 0) //some fields may have higher multiplicity than 1
{
    size_t n_points = cloud.height * cloud.width;
    std::vector<ScalarT> out_vector (n_points); //allocate n_points for this scalar

    size_t point_step = cloud.point_step;




    //get fields and related infos
    sensor_msgs::PointField field = cloud.fields.at( pcl::getFieldIndex(cloud, fieldname) );
    //uint field_count = field.count;
    uint field_size = pcl::getFieldSize(field.datatype);
    uint field_offset = field.offset;

    //cycle on the whole cloud

    for (int i = 0; i < n_points; ++i)
    {
        memcpy(out_vector.data() + i, cloud.data.data() + i * point_step + field_offset + count*field_size, field_size);
    }

    return out_vector;
}

template<typename ScalarT>
std::vector< std::vector<ScalarT> >
readCompleteFieldToVector(const sensor_msgs::PointCloud2 &cloud,
                          const std::string &fieldname)
{

    sensor_msgs::PointField field = cloud.fields.at( pcl::getFieldIndex(cloud, fieldname) );
    uint field_count = field.count;

    std::vector< std::vector<ScalarT> > out_vector(field_count);

    for (size_t c = 0; c < field_count; ++c)
    {
        out_vector[c] = readFieldToVector<ScalarT> (cloud, fieldname, c);
    }

    return out_vector;

}


sensor_msgs::PointCloud2
fromStdVectorToSensor(const std::vector<std::vector<float> > & std_field, const std::string field_name )
{
    sensor_msgs::PointCloud2 out_cloud;




    uint scalar_size =  sizeof(float);
    uint scalar_type;

    if (scalar_size == 4) //see io.h of pcl
        scalar_type = 7;
    else
        scalar_type = 8;


    uint field_count = std_field.size();
    uint n_points = std_field.at(0).size();

    sensor_msgs::PointField field;
    field.count = field_count;
    field.datatype = scalar_type;
    field.name = field_name.c_str();
    field.offset = 0; //always zero we are converting only 1 field

    out_cloud.fields.push_back(field);


    out_cloud.is_bigendian = false;
    out_cloud.width = n_points;
    out_cloud.height = 1;
    out_cloud.is_dense = false;
    out_cloud.point_step = scalar_size * field.count;
    out_cloud.row_step = n_points * out_cloud.point_step; //the same

    //now put data in data
    out_cloud.data.resize(n_points *  out_cloud.point_step);

//    out_cloud.header.

    std_msgs::Header header;
    header.seq = 0;
    header.stamp = 0;
    out_cloud.header = header;



    int component_id = 0;
    for (auto &component : std_field) //for each component in the std_field vector
    {
        int point_id = 0;
        for (auto &p: component) //for each value in this component
        {
            memcpy(out_cloud.data.data() + point_id * out_cloud.point_step + component_id * scalar_size, &p, scalar_size  );
            point_id++;
        }

        component_id++;

    }


    return out_cloud;


}




#endif // PCL_HELPER_H