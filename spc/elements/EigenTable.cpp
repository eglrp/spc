#include "EigenTable.h"



namespace spc
{

DtiClassType EigenTable::Type
= DtiClassType("EigenTable", &ElementBase::Type);

EigenTable::EigenTable(const EigenTable &other, bool only_structure)
{
    names_to_col_ = other.names_to_col_;
    col_to_dim_ = other.col_to_dim_;
    cols_to_name_ = other.cols_to_name_;

    if (only_structure)
        mat_.resizeLike(other.mat());
    else
        mat_ = other.mat_;
}

void EigenTable::addNewComponent(const std::string &name, size_t dimensionality)
{
    if (names_to_col_.find(name) != names_to_col_.end())
        pcl::console::print_error("A column with this name (%s) yet exists. "
                                  "Cannot create a new one.\n", name.c_str());
    else {

        size_t next_id = getNumberOfColumns(); // correspond to the next "free" column

        names_to_col_[name]
                = next_id;

        col_to_dim_[next_id] = dimensionality;

        cols_to_name_[next_id] = name;

        if (dimensionality != 1)
        {
            for (int i = 0; i < dimensionality; ++i)
            {
                std::string newname = getNameOfComponentAtDimension(name, i);
                names_to_col_[newname] = next_id + i;
                col_to_dim_[next_id + i] = 1;
                cols_to_name_[next_id + i] = newname;
            }
        }

        mat_.conservativeResize(getNumberOfRows(),
                    getNumberOfColumns() + dimensionality);

        mat_.block(0, next_id, getNumberOfRows(), dimensionality) = Eigen::MatrixXf::Zero(getNumberOfRows(), dimensionality);
    }
}

const float EigenTable::atScalar(const std::string &name, const size_t &row, const size_t &dimension) const
{
    size_t id = getColumnId(name);
    return mat_(row, id + dimension);
}

Eigen::Block<Eigen::Matrix<float, -1, -1>, 1, -1> EigenTable::atVector(const std::string &name, const size_t &row)
{
    size_t id = getColumnId(name);
    size_t dim = getColumnDimensionality(name);

    //        return mat_.block(row, id, 1, dim);

    return Eigen::Block<Eigen::Matrix<float, -1, -1>, 1, -1>(
                mat_, row, id, 1, dim);
}

size_t EigenTable::getColumnId(const std::string &name) const
{
    std::vector<std::string> splitted;
    boost::split(splitted,name,boost::is_any_of("@"));

    if( splitted.size() > 1) // we found a good split
    {
        std::string basename = splitted.at(0);
        size_t dim_id = boost::lexical_cast<size_t> (splitted.at(splitted.size() - 1));

        if (names_to_col_.find(basename) != names_to_col_.end())
        {
            size_t col_id = names_to_col_.at(basename) + dim_id;
            return col_id;
        }
        else
            return -1; // not found

    }
    else if (splitted.size() == 1)
    {
        if (names_to_col_.find(name) != names_to_col_.end())
            return names_to_col_.at(name);
        else
            return -1;
    }
}

size_t EigenTable::getColumnDimensionality(const std::string &name) const
{
    size_t dim = getColumnId(name);
    return col_to_dim_.at(dim);
}

void EigenTable::resize(const size_t &rows)
{
    size_t old_rows = getNumberOfRows();
    mat_.conservativeResize(rows, getNumberOfColumns());
    mat_.block(old_rows, 0, rows - old_rows, getNumberOfColumns())   =Eigen::MatrixXf::Zero(rows - old_rows, getNumberOfColumns());
}

Eigen::Block<Eigen::Matrix<float, -1, -1>, -1, 1, true> EigenTable::column(const std::string &col_name)
{
    size_t id = getColumnId(col_name);
    if (id == -1)
        pcl::console::print_error(
                    "Cannot find such a column. Is it the names set and right?\n");
    else
        return column(id);
}

Eigen::Block<Eigen::Matrix<float, -1, -1>, -1, -1> EigenTable::getVectorField(const std::string &name)
{
    size_t id = getColumnId(name);
    size_t dim = getColumnDimensionality(name);
    return Eigen::Block<Eigen::Matrix<float, -1, -1>, -1, -1>(
                mat_, 0, id, this->getNumberOfRows(), dim);
}




std::string EigenTable::getColumnName (const size_t &id) const
{
    if (cols_to_name_.find(id) != cols_to_name_.end())
        return cols_to_name_.at(id);
}

std::vector<std::string> EigenTable::getScalarColumnsNames() const
{
    std::vector<std::string> names;

    size_t n_cols = getNumberOfColumns();

    for (size_t i = 0; i < n_cols; ++i)
    {
        names.push_back(getColumnName(i));
    }

    return names;
}

}


