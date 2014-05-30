#ifndef SPC_CALIBRATION_DATA_FILTER_H
#define SPC_CALIBRATION_DATA_FILTER_H

#include <spc/methods/IntensityCalibrationDataEstimator.h>

namespace spc
{

class CalibrationDataFilter
{
public:
    SPC_OBJECT(CalibrationDataFilter)

    CalibrationDataFilter()
    {
    }
    void setInputCalibrationDataDB(DataDB data)
    {
        db_ = data;
    }

    void fixUniqueNormals()
    {

        std::vector<size_t> unique_cores = db_.getVectorOfUniqueCorePoints();
        spcForEachMacro(size_t id, unique_cores)
        {

            // just some verbosity
            if (id % 100 == 0) {
                pcl::console::print_info("core # %i\n", id);
            }

            std::vector<CorePoint::Ptr> current_core_points
                = db_.getDataForCorePointID(id);

            size_t best_id
                = estimateBestCorePointForNormals(current_core_points);

            // extract the normal for the best core point
            Eigen::Vector3f best_normal = current_core_points.at(best_id)->value
                                          <Eigen::Vector3f>("normal");

            // now we force ALL the other core points to have the same normal
            spcForEachMacro(CorePoint::Ptr core_meas, current_core_points)
            {
                core_meas->value<Eigen::Vector3f>("normal") = best_normal;
            }
        }
    }

    //! secondary parameter is the scattering angle for now
    /** \note this will overwrite any existing parameter with the same name
     */
    void recomputeScatteringAngles()
    {
        spcForEachMacro(CorePoint::Ptr core, db_.getDataDB())
        {
            Eigen::Vector3f normal = core->value<Eigen::Vector3f>("normal");
            Eigen::Vector3f ray = core->value<Eigen::Vector3f>("ray");
            // overwrite old measure
            core->value("angle")
                = CalibrationDataEstimator::getMinimumAngleBetweenVectors(
                    normal, ray);
        }
    }

    // this is a temptative to get the best normal for a given core point
    size_t estimateBestCorePointForNormals(const std::vector
                                           <CorePoint::Ptr> &data)
    {
        std::vector<size_t> n_neighbors = extractPropertyAsVector
            <size_t>(data, "n_neighbors");
        std::vector<size_t>::iterator it
            = std::max_element(n_neighbors.begin(), n_neighbors.end());
        size_t position = std::distance(n_neighbors.begin(), it);
        return position;
    }

private:
    DataDB db_;
};

} // end nspace

#endif